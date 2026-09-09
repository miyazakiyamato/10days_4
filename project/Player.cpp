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
	audio_ = Audio::GetInstance();
	seJump_ = audio_->LoadWave("se_character_jump.mp3");

	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 7.0f / 2.0f;
	worldTransform_.UpdateMatrix();
}

void Player::Update() {
	// 地面に埋まってしまった場合の即死判定
	if (isAlive_ && CheckIsBuriedInGround()) {
		isAlive_ = false;
	}
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
	isOnCustomBlock_ = false;

	// 1. 移動とジャンプ入力を処理
	ProcessMoveAndJump();

	currentRideBlock_ = nullptr;

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

	if (isOnCustomBlock_) {
		onGround_ = true;
	}

	// ── 着地した瞬間のトリガー ──
	if (!wasOnGround_ && onGround_) {
		// 着地アニメーション（kLand）を予約・強制移行（他のアニメーションをロックする）
		animState_ = AnimState::kLand;
		animTimeCount_ = 0.15f; // 例: 0.15秒間はこの着地アニメーションを優先再生
		worldTransform_.scale_ = {1.4f, 0.6f, 1.4f};
		targetScale_ = {1.0f, 1.0f, 1.0f};
	}

	// ── アニメーションのステート処理 ──
	ProcessAnimation();

	// ── スケールの滑らかな補間 ──
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
	// すでにカスタムブロックに乗っているフラグが立っているなら、空中落ちは絶対にさせない
	if (isOnCustomBlock_) {
		onGround_ = true;
		velocity_.y = 0.0f;
		return;
	}

	// 1. マップの底面衝突で着地
	if (info.landing) {
		onGround_ = true;
		velocity_.x *= (1.0f - kAttenuationLanding);
		velocity_.y = 0.0f;
		return;
	}

	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {
			if (currentRideBlock_ != nullptr) {
				return;
			}

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

			// ★【重要】カスタムブロックに乗っていることを明示する
			onGround_ = true;
			isOnCustomBlock_ = true;

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

		audio_->PlayWave(seJump_);
	}
}

void Player::ProcessAnimation() {
	// 1. 排他制御が必要なアニメーション（着地中など）のタイマー処理
	if (animState_ == AnimState::kLand) {
		animTimeCount_ -= 1.0f / 60.0f;
		// 着地アニメーション中は目標スケールを1.0に保ちつつ、補間に任せる（あるいは固定）
		targetScale_ = {1.0f, 1.0f, 1.0f};

		// 再生時間が終了したら通常のステートに戻れるようにする
		if (animTimeCount_ <= 0.0f) {
			animState_ = AnimState::kIdle;
		}
		return; // ロック中は他のアニメーション判定を行わない
	}

	// 2. 空中の判定（ジャンプ・落下）
	if (!onGround_) {
		if (velocity_.y > 0.0f) {
			animState_ = AnimState::kJump;
		} else {
			animState_ = AnimState::kJump; // または落下用ステート
		}
	} else {
		// 3. 地上の判定（移動中か待機中か）
		bool isMoving = fabsf(velocity_.x) > 0.01f || currentRideBlock_ != nullptr;
		if (isMoving) {
			animState_ = AnimState::kRun;
		} else {
			animState_ = AnimState::kIdle;
		}
	}

	// 4. ステートごとのスケール計算（switch文）
	switch (animState_) {
	case AnimState::kIdle:
		animTimer_ += 0.07f;
		{
			float wave = sinf(animTimer_) * 0.04f;
			targetScale_ = {1.0f - wave, 1.0f + wave, 1.0f - wave};
		}
		break;

	case AnimState::kRun:
		animTimer_ += 0.3f;
		{
			float wave = sinf(animTimer_) * 0.15f;
			targetScale_ = {1.0f + wave, 1.0f - wave, 1.0f + wave};
		}
		break;

	case AnimState::kJump:
		if (velocity_.y > 0.0f) {
			targetScale_ = {0.8f, 1.2f, 0.8f};
		} else {
			targetScale_ = {1.1f, 0.9f, 1.1f};
		}
		break;

	case AnimState::kLand:
		// 上記の排他制御で弾かれるためここには基本的に来ない
		break;
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

Vector3 Player::GetWorldPosition() const{
	Vector3 pos;
	pos.x = worldTransform_.matWorld_.m[3][0];
	pos.y = worldTransform_.matWorld_.m[3][1];
	pos.z = worldTransform_.matWorld_.m[3][2];
	return pos;
}

AABB Player::GetAABB() const{
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}

AABB Player::GetCrushAABB() {
	AABB aabb = GetAABB();
	// 外側を少し削って、中心付近の「ちっちゃいAABB」にする（例: マージンを0.4f〜0.5fほど縮める）
	float margin = 0.5f;
	aabb.min.x += margin;
	aabb.max.x -= margin;
	aabb.min.y += margin;
	aabb.max.y -= margin;
	return aabb;
}
bool Player::CheckIsBuriedInGround() const {
	if (!mapChipField_)
		return false;

	// プレイヤーの中心位置、あるいは足元の位置を取得
	Vector3 worldPos = GetWorldPosition();

	// その位置のマップチップのインデックスを取得
	MapChipField::IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldPos);

	// インデックスがマップの範囲内かチェック
	if (indexSet.xIndex >= mapChipField_->GetNumBlockHorizontal() || indexSet.yIndex >= mapChipField_->GetNumBlockVirtical()) {
		return false;
	}

	// その場所のマップチップのタイプを取得
	MapChipType type = mapChipField_->GetMapChipData(indexSet.xIndex, indexSet.yIndex);

	// もしそこが通常のブロック（kBlock）であり、かつプレイヤーのAABBがそのブロックのRectと深く重なっているなら「埋まっている」
	if (type == MapChipType::kBlock) {
		MapChipField::Rect rect = mapChipField_->GatRectByIndex(indexSet.xIndex, indexSet.yIndex);
		AABB playerAABB = GetAABB();

		// プレイヤーのAABBがブロックのRectの内部に完全に食い込んでいる（ガッツリ埋まっている）か判定
		bool isOverlapX = (playerAABB.min.x < rect.right && playerAABB.max.x > rect.left);
		bool isOverlapY = (playerAABB.min.y < rect.top && playerAABB.max.y > rect.bottom);

		if (isOverlapX && isOverlapY) {
			return true; // 地面に埋まっている！
		}
	}

	return false;
}