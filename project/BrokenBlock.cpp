#include "BrokenBlock.h"
#include "GameScene.h"
#include "MapChipField.h"
#include "MovingBlock.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <Collision.h>

void BrokenBlock::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();

	// 生成時の位置を元の位置（帰還目標）として記録
	targetPos_ = position;
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();

	objectColor_ = new ObjectColor();
	objectColor_->Initialize();
	// ※ GameSceneで定義されている GetColorById が使用可能である前提
	objectColor_->SetColor(GetColorById(id_));
}

void BrokenBlock::Update(const std::vector<BrokenBlock*>& brokenBlocks, const std::vector<MovingBlock*>& movingBlocks) {
	Vector3 prePos = worldTransform_.translation_;

	if (isActive_) {
		// アクティブ時：元の位置（targetPos_）に向かって移動する
		Vector3 diff = {targetPos_.x - worldTransform_.translation_.x, targetPos_.y - worldTransform_.translation_.y, targetPos_.z - worldTransform_.translation_.z};

		float length = std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
		if (length < kReturnSpeed) {
			// 目標地点に到着
			worldTransform_.translation_ = targetPos_;
		} else {
			// 移動計算
			diff.x /= length;
			diff.y /= length;
			diff.z /= length;
			worldTransform_.translation_.x += diff.x * kReturnSpeed;
			worldTransform_.translation_.y += diff.y * kReturnSpeed;
			worldTransform_.translation_.z += diff.z * kReturnSpeed;
		}
	} else {
		// 非アクティブ時：重力で落下する
		velocity_.y -= kGravity;
		velocity_.y = (std::max)(velocity_.y, -kLimitFallSpeed); // 落下速度制限
		worldTransform_.translation_.y += velocity_.y;

		// ステージブロックとの簡易当たり判定（下方向のみ）
		if (mapChipField_) {
			Vector3 bottomPos = worldTransform_.translation_;
			bottomPos.y -= (kHeight / 2.0f);

			MapChipField::IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(bottomPos);
			MapChipType type = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

			if (type == MapChipType::kBlock) {
				MapChipField::Rect rect = mapChipField_->GatRectByIndex(indexSet.xIndex, indexSet.yIndex);
				if (worldTransform_.translation_.y - (kHeight / 2.0f) < rect.top) {
					worldTransform_.translation_.y = rect.top + (kHeight / 2.0f);
					velocity_.y = 0.0f;
				}
			}
		}

		AABB myAABB = GetAABB();
		constexpr float kMargin = 0.05f; 

		// ① 他の壊れたブロックとの判定
		for (BrokenBlock* other : brokenBlocks) {
			if (other == this) continue;
			AABB otherAABB = other->GetAABB();
			bool isIntersectX = (myAABB.min.x < otherAABB.max.x - kMargin) && (myAABB.max.x > otherAABB.min.x + kMargin);
			bool isIntersectZ = (myAABB.min.z < otherAABB.max.z - kMargin) && (myAABB.max.z > otherAABB.min.z + kMargin);
			bool isIntersectY = (myAABB.min.y < otherAABB.max.y) && (myAABB.max.y > otherAABB.min.y);

			if (isIntersectX && isIntersectY && isIntersectZ) {
				if (prePos.y >= other->GetWorldPosition().y) {
					worldTransform_.translation_.y = otherAABB.max.y + (kHeight / 2.0f);
					velocity_.y = 0.0f;

					// アクティブなブロックの上なら、そのブロックの移動量をそのまま自分に足す！
					if (other->GetIsActive()) {
						worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, other->GetVelocity());
					}

					myAABB = GetAABB();
				}
			}
		}

		// ② 動くブロックとの判定 ＋ 運ばれる処理（追加）
		for (MovingBlock* mBlock : movingBlocks) {
			AABB movingAABB = mBlock->GetAABB();
			bool isIntersectX = (myAABB.min.x < movingAABB.max.x - kMargin) && (myAABB.max.x > movingAABB.min.x + kMargin);
			bool isIntersectZ = (myAABB.min.z < movingAABB.max.z - kMargin) && (myAABB.max.z > movingAABB.min.z + kMargin);
			bool isIntersectY = (myAABB.min.y < movingAABB.max.y) && (myAABB.max.y > movingAABB.min.y);

			if (isIntersectX && isIntersectY && isIntersectZ) {
				if (prePos.y >= mBlock->GetWorldPosition().y) {
					// 動くブロックの上に載せる
					worldTransform_.translation_.y = movingAABB.max.y + (kHeight / 2.0f);
					velocity_.y = 0.0f;

					// ★【重要】動くブロックが動いた分だけ、自分も一緒に移動させる
					worldTransform_.translation_.x += mBlock->GetVelocity().x * 2.0f;
					
					myAABB = GetAABB();
				}
			}
		}

		// ステージ外（Y座標の閾値）に出たら落下を止める
		if (worldTransform_.translation_.y < -10.0f) {
			worldTransform_.translation_.y = -10.0f;
			velocity_.y = 0.0f;
		}
	}

	// 今回のフレームでの実際の移動量を速度として記録
	velocity_ = {worldTransform_.translation_.x - prePos.x, worldTransform_.translation_.y - prePos.y, worldTransform_.translation_.z - prePos.z};

	worldTransform_.UpdateMatrix();
}

void BrokenBlock::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_, objectColor_); }

void BrokenBlock::OnCollision(const Player* player) { (void)player; }

Vector3 BrokenBlock::GetWorldPosition() {
	Vector3 pos;
	pos.x = worldTransform_.matWorld_.m[3][0];
	pos.y = worldTransform_.matWorld_.m[3][1];
	pos.z = worldTransform_.matWorld_.m[3][2];
	return pos;
}

AABB BrokenBlock::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}