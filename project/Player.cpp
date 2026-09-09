#define NOMINMAX
#include "Player.h"
#include "Enemy.h"
#include "MapChipField.h"
#include "MovingBlock.h"
#include "goalBlock.h"
#include <DebugText.h>
#include <Input.h>
#include <algorithm>
#include <cassert>
#include <numbers>
#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG
#include <BrokenBlock.h>

void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	input_ = Input::GetInstance();
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 7.0f / 2.0f;
	worldTransform_.UpdateMatrix();
}

void Player::Update() {
	if (!isAlive_) {
		return;
	}
	if (currentRideBlock_) {
		worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, currentRideBlock_->GetVelocity());
	}

#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::Checkbox("isPush", &isPush_);
	ImGui::DragFloat3("enemyVelocity", &enemyVelocity_.x);
	ImGui::End();
#endif // _DEBUG

	// 前フレームの接地状態を記録
	wasOnGround_ = onGround_;

	// 1. 移動とジャンプ入力を処理
	ProcessMoveAndJump(); 

	// 当たり判定に備えて一度リセットする
	currentRideBlock_ = nullptr;
	// 2. 衝突判定とマップ移動処理
	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.amountMovement = velocity_;

	MapCollision(collisionMapInfo);
	MoveToReflectCollision(collisionMapInfo);
	if (!isPush_) {
		OnCeilingCollision(collisionMapInfo);
	}
	onHitWall(collisionMapInfo);

	if (!isPush_) {
		GroundingSwitch(collisionMapInfo);
	}

	// ★【共通化】「直前まで空中（wasOnGround_ == false）」かつ「今接地した（onGround_ == true）」の瞬間だけ必ず潰れアニメーションを発動
	if (!wasOnGround_ && onGround_) {
		worldTransform_.scale_ = {1.4f, 0.6f, 1.4f};
		targetScale_ = {1.0f, 1.0f, 1.0f};
	}

	// 3. アニメーション（スケール計算）を処理
	ProcessAnimation();

	// 4. スケールの滑らかな補間を処理
	UpdateScaleInterpolation();

	// 旋回制御
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;
		float destinationRotationYTable[] = {std::numbers::pi_v<float> * 3.0f / 2.0f, std::numbers::pi_v<float> / 2.0f};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		worldTransform_.rotation_.y = destinationRotationY + (destinationRotationY - turnFirstRotationY_) * (sinf(((turnTimer_ / kTimeTurn) * std::numbers::pi_v<float>) / 2));
	}

	if (worldTransform_.translation_.y <= 0.0f) {
		isAlive_ = false;
	}
	worldTransform_.UpdateMatrix();
}

void Player::Move() {
	if (onGround_) {
		if (input_->PushKey(DIK_RIGHT) || input_->PushKey(DIK_D) || input_->PushKey(DIK_LEFT) || input_->PushKey(DIK_A)) {
			Vector3 acceleration = {};
			if (input_->PushKey(DIK_RIGHT) || input_->PushKey(DIK_D)) {
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = std::numbers::pi_v<float> / 2.0f;
					turnTimer_ = kTimeTurn;
				}
			} else if (input_->PushKey(DIK_LEFT) || input_->PushKey(DIK_A)) {
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = std::numbers::pi_v<float> * 3.0f / 2.0f;
					turnTimer_ = kTimeTurn;
				}
			}
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
			velocity_ = MyMtVector3::Add(velocity_, acceleration);
		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}
	} else {
		velocity_ = MyMtVector3::Add(velocity_, Vector3(0, -kGravityAcceleration, 0));
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
	preSpace_ = input_->PushKey(DIK_SPACE);
}

void Player::MapCollision(CollisionMapInfo& info) {
	MapCollisionLeft(info);
	MapCollisionRight(info);
	MapCollisionBottom(info);
	MapCollisionTop(info);
}

void Player::MapCollisionLeft(CollisionMapInfo& info) {
	if (info.amountMovement.x >= 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement));
		MapChipField::Rect rect = mapChipField_->GatRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.amountMovement.x = std::min(0.0f, (rect.right - worldTransform_.translation_.x) + (kWidth / 2.0f + kBlank));
		info.hitWall = true;
	}
}

void Player::MapCollisionRight(CollisionMapInfo& info) {
	if (info.amountMovement.x <= 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement));
		MapChipField::Rect rect = mapChipField_->GatRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.amountMovement.x = std::max(0.0f, (rect.left - worldTransform_.translation_.x) - (kWidth / 2.0f + kBlank));
		info.hitWall = true;
	}
}

void Player::MapCollisionBottom(CollisionMapInfo& info) {
	if (info.amountMovement.y >= 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement));
		MapChipField::Rect rect = mapChipField_->GatRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.amountMovement.y = std::min(0.0f, (rect.top - worldTransform_.translation_.y) + (kHeight / 2.0f + kBlank));
		info.landing = true;
	}
}

void Player::MapCollisionTop(CollisionMapInfo& info) {
	if (info.amountMovement.y <= 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement));
		MapChipField::Rect rect = mapChipField_->GatRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.amountMovement.y = std::max(0.0f, (rect.bottom - worldTransform_.translation_.y) - (kHeight / 2.0f + kBlank));
		info.isCeilingCollision = true;
	}
}

void Player::MoveToReflectCollision(CollisionMapInfo& info) { worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, info.amountMovement); }

void Player::OnCeilingCollision(const CollisionMapInfo& info) {
	if (info.isCeilingCollision) {
		velocity_.y = 0.0f;
	}
}

void Player::GroundingSwitch(const CollisionMapInfo& info) {
	// 1. マップの底面衝突（MapCollisionBottom）で着地フラグが立ったら接地
	if (info.landing) {
		onGround_ = true;
		velocity_.x *= (1.0f - kAttenuationLanding);
		velocity_.y = 0.0f;
		return;
	}

	// 2. すでに接地している場合の足場維持・離脱判定
	if (onGround_) {
		if (velocity_.y > 0.0f) {
			// 上昇中は強制的に空中へ
			onGround_ = false;
		} else {
			// 動くブロックに乗っている場合は接地維持
			if (currentRideBlock_ != nullptr) {
				return;
			}

			// マップチップの足元判定
			std::array<Vector3, kNumCorner> positionsNew;
			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement), static_cast<Corner>(i));
			}

			bool hitMap = false;
			MapChipField::IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(MyMtVector3::Add(positionsNew[kLeftBottom], Vector3(0, -0.1f, 0)));
			if (mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock) {
				hitMap = true;
			}
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(MyMtVector3::Add(positionsNew[kRightBottom], Vector3(0, -0.1f, 0)));
			if (mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock) {
				hitMap = true;
			}

			// マップの足場がなく、かつ動くブロックにも乗っていない場合
			// （さらに、落下し始めている、または完全に足場から外れたとき）
			if (!hitMap) {
				onGround_ = false;
			}
		}
	}
}

void Player::onHitWall(const CollisionMapInfo& info) {
	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };
	return MyMtVector3::Add(center, offsetTable[static_cast<uint32_t>(corner)]);
}

void Player::OnCollision(const Enemy* enemy) {
	(void)enemy;
	if (!enemy->GetIsMove()) {
		return;
	}
	isPush_ = true;
	enemyVelocity_.x = enemy->GetVelocity().x;
	velocity_ = MyMtVector3::Add(velocity_, enemy->GetVelocity());
}

void Player::OnCollision(const GoalBlock* goalBlock) {
	(void)goalBlock;
	isClear = true;
	isAlive_ = false;
}

void Player::OnCollision(MovingBlock* movingBlock) { ResolveBlockCollision(movingBlock->GetAABB(), true, movingBlock); }

void Player::OnCollision(BrokenBlock* brokenBlock) { ResolveBlockCollision(brokenBlock->GetAABB(), false); }

void Player::ResolveBlockCollision(const AABB& b, bool isMovingBlock, MovingBlock* movingBlock) {
	AABB p = GetAABB();

	float overlapLeft = p.max.x - b.min.x;
	float overlapRight = b.max.x - p.min.x;
	float overlapBottom = p.max.y - b.min.y;
	float overlapTop = b.max.y - p.min.y;

	float minOverlapX = std::min(overlapLeft, overlapRight);
	float minOverlapY = std::min(overlapTop, overlapBottom);

	if (minOverlapX < minOverlapY) {
		if (overlapLeft < overlapRight) {
			worldTransform_.translation_.x -= overlapLeft;
		} else {
			worldTransform_.translation_.x += overlapRight;
		}
		velocity_.x = 0.0f;
	} else {
		if (overlapBottom < overlapTop) {
			// 頭をぶつけた場合
			worldTransform_.translation_.y -= overlapBottom;
			if (velocity_.y > 0.0f) {
				velocity_.y = 0.0f;
			}
		} else {
			// 床として踏んだ場合
			worldTransform_.translation_.y += overlapTop;
			if (velocity_.y < 0.0f) {
				velocity_.y = 0.0f;
			}
			onGround_ = true;
			// 動くブロックの場合は乗っているブロックとして登録
			if (isMovingBlock) {
				currentRideBlock_ = movingBlock;
			}
		}
	}
	worldTransform_.UpdateMatrix();
}
void Player::ProcessMoveAndJump() {
	isMakeBlock = false;
	if (isPush_) {
		if (currentRideBlock_) {
			onGround_ = true;
			velocity_.y = 0.0f;
		} else {
			velocity_ = MyMtVector3::Add(velocity_, enemyVelocity_);
			velocity_.y = 0;
			if (fabsf(enemyVelocity_.x) > 0.001f) {
				enemyVelocity_.x *= 0.5f;
			} else {
				isPush_ = false;
			}
		}
	} else {
		Move();
	}

	if (onGround_ && (input_->TriggerKey(DIK_UP) || input_->TriggerKey(DIK_W))) {
		velocity_.y = kJumpAcceleration;
		onGround_ = false;

		worldTransform_.scale_ = {0.6f, 1.5f, 0.6f};
		targetScale_ = {1.0f, 1.0f, 1.0f};
	}
}

void Player::ProcessAnimation() {
	targetScale_ = {1.0f, 1.0f, 1.0f};

	if (!onGround_) {
		if (velocity_.y > 0.0f) {
			targetScale_ = {0.8f, 1.2f, 0.8f}; // 上昇中
		} else {
			targetScale_ = {1.1f, 0.9f, 1.1f}; // 下降中
		}
	} else {
		// 地上（ステージ床、動くブロック、壊れたブロックの上すべて共通）
		bool isMovingHorizontally = fabsf(velocity_.x) > 0.01f;

		if (isMovingHorizontally || currentRideBlock_ != nullptr) {
			// 移動中、または動くブロックに乗って運ばれているとき
			animTimer_ += 0.3f;
			float wave = sinf(animTimer_) * 0.15f;
			targetScale_ = {1.0f + wave, 1.0f - wave, 1.0f + wave};
		} else {
			// 待機中（普通の床でも、壊れたブロックの上でも共通のプニプニ待機）
			animTimer_ += 0.07f;
			float wave = sinf(animTimer_) * 0.04f;
			targetScale_ = {1.0f - wave, 1.0f + wave, 1.0f - wave};
		}
	}
}
void Player::UpdateScaleInterpolation() {
	worldTransform_.scale_.x += (targetScale_.x - worldTransform_.scale_.x) * 0.25f;
	worldTransform_.scale_.y += (targetScale_.y - worldTransform_.scale_.y) * 0.25f;
	worldTransform_.scale_.z += (targetScale_.z - worldTransform_.scale_.z) * 0.25f;
}

void Player::Draw() {
	if (!isAlive_) {
		return;
	}
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}

Vector3 Player::GetWorldPosition() {
	Vector3 pos;
	pos.x = worldTransform_.matWorld_.m[3][0];
	pos.y = worldTransform_.matWorld_.m[3][1];
	pos.z = worldTransform_.matWorld_.m[3][2];
	return pos;
}

AABB Player::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}