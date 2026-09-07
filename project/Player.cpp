#define NOMINMAX
#include "Player.h"
#include <cassert>
#include <numbers>
#include <algorithm>
#include <Input.h>
#include "MapChipField.h"
#include <DebugText.h>
#include "Enemy.h"
#include "goalBlock.h"
#include "MovingBlock.h"
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
		// 移動入力
		Move();
		
	}
	// 当たり判定に備えて一度リセットする
	currentRideBlock_ = nullptr;

	// 衝突判定
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.amountMovement = velocity_;

	// マップ衝突チェック
	MapCollision(collisionMapInfo);

	// 移動
	// worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, velocity_);
	MoveToReflectCollision(collisionMapInfo);
	if (!isPush_) {
		// 天井に接触してる
		OnCeilingCollision(collisionMapInfo);
	}
	// 壁接触している時の処理
	onHitWall(collisionMapInfo);
	if (!isPush_) {
		// 着地状態切り替え
		GroundingSwitch(collisionMapInfo);
	}
	// 旋回制御
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;
		float destinationRotationYTable[] = {std::numbers::pi_v<float> * 3.0f / 2.0f,std::numbers::pi_v<float> / 2.0f};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		worldTransform_.rotation_.y = destinationRotationY + (destinationRotationY - turnFirstRotationY_) * (sinf(((turnTimer_ / kTimeTurn) * std::numbers::pi_v<float>) / 2));
	}
	if (worldTransform_.translation_.y <= 0.0f) {
		isAlive_ = false;
	}
	worldTransform_.UpdateMatrix();
}

void Player::Draw() {
	if (!isAlive_) {
		return;
	}
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}

void Player::Move() {
	// 移動入力
	// 接地状態
	if (onGround_) {
		if (input_->PushKey(DIK_RIGHT) || input_->PushKey(DIK_D) || input_->PushKey(DIK_LEFT) || input_->PushKey(DIK_A)) {
			// 左右加速
			Vector3 acceleration = {};
			if (input_->PushKey(DIK_RIGHT) || input_->PushKey(DIK_D)) {
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
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
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = std ::numbers::pi_v<float> * 3.0f / 2.0f;
					turnTimer_ = kTimeTurn;
				}
			}
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
			velocity_ = MyMtVector3::Add(velocity_, acceleration);
		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}
		if (input_->PushKey(DIK_UP) || input_->PushKey(DIK_W)) {
			// ジャンプ初速
			velocity_ = MyMtVector3::Add(velocity_, Vector3(0, kJumpAcceleration, 0));
		}
		/*if (input_->TriggerKey(DIK_SPACE)) {
			isPushSpace_ = true;
		}
		if (input_->PushKey(DIK_SPACE)) {
			pushTime += 1.0f / 60.f;
			if (pushTime > maxPushTime && isPushSpace_) {
				isMoveBlock_ = true;
				isPushSpace_ = false;
				if (lastEnemy) {
					lastEnemy->SetIsLast(true);
				}
			}
		}
		if (!input_->PushKey(DIK_SPACE)) {
			if (preSpace_) {
				isPushSpace_ = false;
				if (pushTime <= maxPushTime) {
					isMakeBlock = true;
				}
				pushTime = 0;
			}
		}*/
		
		
	}
	else {
		// 落下速度
		velocity_ = MyMtVector3::Add(velocity_, Vector3(0, -kGravityAcceleration, 0));
		// 落下速度制限
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
	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 右の当たり判定
	bool hit = false;
	// 右上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GatRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.amountMovement.x = std::min(0.0f, (rect.right - worldTransform_.translation_.x) + (kWidth / 2.0f + kBlank));
		// 地面に当たったことを記録する
		info.hitWall = true;
	}
}

void Player::MapCollisionRight(CollisionMapInfo& info) {
	if (info.amountMovement.x <= 0) {
		return;
	}
	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 右の当たり判定
	bool hit = false;
	// 右上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GatRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.amountMovement.x = std::max(0.0f, (rect.left - worldTransform_.translation_.x) - (kWidth / 2.0f + kBlank));
		// 地面に当たったことを記録する
		info.hitWall = true;
	}
}

void Player::MapCollisionBottom(CollisionMapInfo& info) {
	if (info.amountMovement.y >= 0) {
		return;
	}
	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//真下の当たり判定
	bool hit = false;
	//左下の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	//右下の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GatRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.amountMovement.y = std::min(0.0f, (rect.top - worldTransform_.translation_.y) + (kHeight / 2.0f + kBlank));
		//地面に当たったことを記録する
		info.landing = true;
	}
}

void Player::MapCollisionTop(CollisionMapInfo& info) {
	if (info.amountMovement.y <= 0) {
		return;
	}
	//移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(MyMtVector3::Add(worldTransform_.translation_ , info.amountMovement), static_cast<Corner>(i));
	}
	

	MapChipType mapChipType;
	//真上の当たり判定
	bool hit = false;
	//左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	//右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	//ブロックにヒット？
	if (hit) {
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement));
		//めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GatRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.amountMovement.y = std::max(0.0f, (rect.bottom - worldTransform_.translation_.y) - (kHeight / 2.0f + kBlank));
		//天井に当たったことを記録する
		info.isCeilingCollision = true;
	}
}

void Player::MoveToReflectCollision(CollisionMapInfo& info) {
	worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, info.amountMovement);
}

void Player::OnCeilingCollision(const CollisionMapInfo& info) {
	//天井に当たった
	if (info.isCeilingCollision) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0.0f;
	}
}

void Player::GroundingSwitch(const CollisionMapInfo& info) {
	//自キャラ接地状態？
	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {
			// 移動後の4つの角の座標
			std::array<Vector3, kNumCorner> positionsNew;
			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement), static_cast<Corner>(i));
			}

			MapChipType mapChipType;
			// 真下の当たり判定
			bool hit = false;
			// 左下の判定
			MapChipField::IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(MyMtVector3::Add(positionsNew[kLeftBottom],Vector3(0,-0.1f,0)));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			// 右下の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(MyMtVector3::Add(positionsNew[kRightBottom], Vector3(0, -0.1f, 0)));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			//落下開始
			if (!hit) {
				//空中状態に切り替えsる
				onGround_ = false;
			}
		}
	} else {
		// 着地フラグ
		if (info.landing) {
			// 着地状態に切り替える
			onGround_ = true;
			// 着地時にx速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);
			// Y速度をゼロにする
			velocity_.y = 0.0f;
		}
	}
}

void Player::onHitWall(const CollisionMapInfo& info) {
	//壁接触による減速
	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}  //  kLeftTop
	};
	return MyMtVector3::Add(center, offsetTable[static_cast<uint32_t>(corner)]);
}

void Player::OnCollision(const Enemy* enemy) {
	(void)enemy;
	if (!enemy->GetIsMove()) {
		return;
	}
	//ジャンプ開始
	isPush_ = true;
	enemyVelocity_.x = enemy->GetVelocity().x;
	velocity_ = MyMtVector3::Add(velocity_, enemy->GetVelocity());
}

void Player::OnCollision(const GoalBlock* goalBlock) {
	(void)goalBlock;
	isClear = true;
	isAlive_ = false;
}

void Player::OnCollision(MovingBlock* movingBlock) {
	AABB p = GetAABB();
	AABB b = movingBlock->GetAABB();

	// 4方向のめり込み量を計算
	float overlapLeft = p.max.x - b.min.x;
	float overlapRight = b.max.x - p.min.x;
	float overlapBottom = p.max.y - b.min.y;
	float overlapTop = b.max.y - p.min.y;

	// X軸とY軸の最もめり込みが小さい（近い）面を判定
	float minOverlapX = std::min(overlapLeft, overlapRight);
	float minOverlapY = std::min(overlapTop, overlapBottom);

	if (minOverlapX < minOverlapY) {
		// X軸方向の押し戻し（壁としての判定）
		if (overlapLeft < overlapRight) {
			worldTransform_.translation_.x -= overlapLeft;
		} else {
			worldTransform_.translation_.x += overlapRight;
		}
		velocity_.x = 0.0f;
	} else {
		// Y軸方向の押し戻し（床・天井としての判定）
		if (overlapBottom < overlapTop) {
			// 頭をぶつけた場合
			worldTransform_.translation_.y -= overlapBottom;
			if (velocity_.y > 0.0f) {
				velocity_.y = 0.0f;
			}
		} else {
			// 着地した場合
			worldTransform_.translation_.y += overlapTop;
			if (velocity_.y < 0.0f) {
				velocity_.y = 0.0f;
			}
			onGround_ = true; // ジャンプ可能にする
			// 現在乗っているブロックとして登録
			currentRideBlock_ = movingBlock;
		}
	}
	worldTransform_.UpdateMatrix();
}


void Player::OnCollision(BrokenBlock* brokenBlock) {
	AABB p = GetAABB();
	AABB b = brokenBlock->GetAABB();

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
			worldTransform_.translation_.y -= overlapBottom;
			if (velocity_.y > 0.0f) {
				velocity_.y = 0.0f;
			}
		} else {
			worldTransform_.translation_.y += overlapTop;
			if (velocity_.y < 0.0f) {
				velocity_.y = 0.0f;
			}
			onGround_ = true;
		}
	}
	worldTransform_.UpdateMatrix();
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
