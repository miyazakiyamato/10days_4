#define NOMINMAX
#include "Enemy.h"
#include "Player.h"
#include <cassert>
#include <algorithm>
#include "MapChipField.h"
#include <DebugText.h>

 uint32_t Enemy::NextEnemyNum = 0;

void Enemy::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position) {
	++NextEnemyNum;
	enemyNum_ = NextEnemyNum;

	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
	worldTransform_.UpdateMatrix();
	velocity_ = {0, 0, 0};
	walkTimer_ = 0.0f;
	mapXIndex = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_).xIndex;
	mapYIndex = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_).yIndex;
}

void Enemy::Update() {

	if (isMove) {
		//worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, velocity_);
		// 衝突判定
		CollisionMapInfo collisionMapInfo;
		// 移動量に速度の値をコピー
		collisionMapInfo.amountMovement = velocity_;

		// マップ衝突チェック
		MapCollision(collisionMapInfo);

		// 移動
		// worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, velocity_);
		MoveToReflectCollision(collisionMapInfo);
		// 壁接触している時の処理
		onHitWall(collisionMapInfo);
		// タイマーを加算
		walkTimer_ += 1.0f / 60.0f;
		if (walkTimer_ > 2.0f) {
		    walkTimer_ = 0.0f;
		}
		// 回転アニメーション
		float param = std::sin(2 * std::numbers::pi_v<float> * walkTimer_ / kWalkMotionTime);
		float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;

		worldTransform_.rotation_.x = radian;
	} else {
		if (player_->GetIsMoveBlock()) {
			if (isLast_) {
				velocity_ = MyMtVector3::Multiply(1.0f / 30.0f, MyMtVector3::Subtract(player_->GetWorldPosition(), GetWorldPosition()));
				velocity_.y = 0.0f;
				isMove = true;
				mapChipField_->SetMapChipData(mapXIndex, mapYIndex, MapChipType::kBlank);
			}
		}
		worldTransform_.rotation_.x = 0.0f;
	}
	
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); }

void Enemy::OnCollision(const Player* player) {
	(void)player;
	if (!GetIsMove()) {
		return;
	}
	isAlive = false;
}

void Enemy::MapCollision(CollisionMapInfo& info) {
	MapCollisionLeft(info);
	MapCollisionRight(info);
	MapCollisionBottom(info);
	MapCollisionTop(info);
}

void Enemy::MapCollisionLeft(CollisionMapInfo& info) {
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

void Enemy::MapCollisionRight(CollisionMapInfo& info) {
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

void Enemy::MapCollisionBottom(CollisionMapInfo& info) {
	if (info.amountMovement.y >= 0) {
		return;
	}
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
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
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
		info.amountMovement.y = std::min(0.0f, (rect.top - worldTransform_.translation_.y) + (kHeight / 2.0f + kBlank));
		// 地面に当たったことを記録する
		info.landing = true;
	}
}

void Enemy::MapCollisionTop(CollisionMapInfo& info) {
	if (info.amountMovement.y <= 0) {
		return;
	}
	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(MyMtVector3::Add(worldTransform_.translation_, info.amountMovement), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真上の当たり判定
	bool hit = false;
	// 左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
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
		info.amountMovement.y = std::max(0.0f, (rect.bottom - worldTransform_.translation_.y) - (kHeight / 2.0f + kBlank));
		// 天井に当たったことを記録する
		info.isCeilingCollision = true;
	}
}

void Enemy::MoveToReflectCollision(CollisionMapInfo& info) { worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, info.amountMovement); }

void Enemy::onHitWall(const CollisionMapInfo& info) {
	// 壁接触による減速
	if (info.hitWall) {
		velocity_ = {0, 0, 0};
		player_->SetIsMoveBlock(false);
		isMove = false;
		mapXIndex = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_).xIndex;
		mapYIndex = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_).yIndex;
		mapChipField_->SetMapChipData(mapXIndex, mapYIndex, MapChipType::kBlock);
		worldTransform_.translation_ = mapChipField_->GetMapChipPositionByIndex(mapXIndex, mapYIndex);
		isLast_ = false;
	}
}

Vector3 Enemy::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}  //  kLeftTop
	};
	return MyMtVector3::Add(center, offsetTable[static_cast<uint32_t>(corner)]);
}
Vector3 Enemy::GetWorldPosition() {
	Vector3 pos;
	pos.x = worldTransform_.matWorld_.m[3][0];
	pos.y = worldTransform_.matWorld_.m[3][1];
	pos.z = worldTransform_.matWorld_.m[3][2];
	return pos;
}

AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}