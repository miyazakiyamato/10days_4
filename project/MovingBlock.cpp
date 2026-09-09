#include "MovingBlock.h"
#include "BrokenBlock.h"
#include "Collision.h"
#include "GameScene.h"
#include <algorithm>
#include <cassert>
#include <cmath>

void MovingBlock::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();

	// 生成された位置を「始まりの位置（5_0）」として記録
	startPos_ = position;
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();

	objectColor_ = new ObjectColor();
	objectColor_->Initialize();
	objectColor_->SetColor(GetColorById(id_));
}

void MovingBlock::Update(const std::vector<BrokenBlock*>& brokenBlocks) {
	Vector3 prePos = worldTransform_.translation_;

	// アクティブかつ、終わりの位置が設定されている場合のみ往復移動を行う
	if (isActive_ && hasEndPos_) {
		// 現在向かうべきターゲット（往路ならendPos_、復路ならstartPos_）
		Vector3 target = isReturning_ ? startPos_ : endPos_;

		Vector3 dir = {target.x - worldTransform_.translation_.x, target.y - worldTransform_.translation_.y, target.z - worldTransform_.translation_.z};

		float length = std::sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);

		if (length < kSpeed) {
			// ターゲットに到着した場合
			worldTransform_.translation_ = target;

			// 目的地に着いたら方向を逆にする
			isReturning_ = !isReturning_;
		} else {
			// 通常の移動計算
			dir.x /= length;
			dir.y /= length;
			dir.z /= length;

			worldTransform_.translation_.x += dir.x * kSpeed;
			worldTransform_.translation_.y += dir.y * kSpeed;
			worldTransform_.translation_.z += dir.z * kSpeed;

			// 移動中に壊れたブロック（BrokenBlock）と衝突したかチェック
			AABB myAABB = GetAABB();
			bool hasCollided = false;

			for (BrokenBlock* bBlock : brokenBlocks) {
				// 落下中または停止中の壊れたブロックのみ対象にする
				if (bBlock->GetIsActive())
					continue;

				AABB otherAABB = bBlock->GetAABB();
				if (Collision::IsCollision(myAABB, otherAABB)) {
					if (otherAABB.min.y >= myAABB.max.y - 0.2f) {
						continue;
					}
					hasCollided = true;
					break;
				}
			}

			if (hasCollided) {
				// 1. 位置を一度戻す
				worldTransform_.translation_ = prePos;

				// 2. 進行方向を反転させる
				isReturning_ = !isReturning_;

				// 3. 【重要】反転させた直後のターゲットに向かって、今度は逆向きに一歩分進めて障害物から離す
				target = isReturning_ ? startPos_ : endPos_;
				Vector3 escapeDir = {target.x - worldTransform_.translation_.x, target.y - worldTransform_.translation_.y, target.z - worldTransform_.translation_.z};
				float escapeLen = std::sqrt(escapeDir.x * escapeDir.x + escapeDir.y * escapeDir.y + escapeDir.z * escapeDir.z);
				if (escapeLen > 0.001f) {
					escapeDir.x /= escapeLen;
					escapeDir.y /= escapeLen;
					escapeDir.z /= escapeLen;
					// 逆向きへ強制的に kSpeed 分だけ動かして重なりを解消する
					worldTransform_.translation_.x += escapeDir.x * kSpeed;
					worldTransform_.translation_.y += escapeDir.y * kSpeed;
					worldTransform_.translation_.z += escapeDir.z * kSpeed;
				}
			}
		}
	} else if (!isActive_) {
		//その場で待機
	}

	// 実際の移動量を速度として記録（上に乗るプレイヤーや他のブロックのため）
	velocity_ = {worldTransform_.translation_.x - prePos.x, worldTransform_.translation_.y - prePos.y, worldTransform_.translation_.z - prePos.z};
	worldTransform_.UpdateMatrix();

	if (player_ && player_->GetIsAlive()) {
		AABB myAABB = GetAABB();
		AABB playerAABB = player_->GetCrushAABB();

		if (Collision::IsCollision(myAABB, playerAABB)) {
			// 1. 各方向のガッツリ重なっている深さを正確に計算
			float overlapLeft = playerAABB.max.x - myAABB.min.x;   // プレイヤーが右からブロックに食い込んでいる深さ
			float overlapRight = myAABB.max.x - playerAABB.min.x;  // ブロックが右からプレイヤーに食い込んでいる深さ
			float overlapBottom = playerAABB.max.y - myAABB.min.y; // プレイヤーが下からブロックに食い込んでいる深さ
			float overlapTop = myAABB.max.y - playerAABB.min.y;    // ブロックが上からプレイヤーに食い込んでいる深さ

			// 2. 「上に乗っている」安全な状態の判定
			bool isRidingOnTop = (playerAABB.min.y >= myAABB.max.y - 0.3f) && (overlapTop > 0.0f && overlapTop < 0.25f);

			if (!isRidingOnTop) {
				// ★「誰が見てもガッツリ重なっている」と判断する深さの閾値（0.8f）
				constexpr float kFullCrushThreshold = 0.8f;

				// どの方向であっても、十分に深く食い込んでいる場合のみ死亡
				if (overlapLeft > kFullCrushThreshold || overlapRight > kFullCrushThreshold || overlapBottom > kFullCrushThreshold || overlapTop > kFullCrushThreshold) {

					player_->SetIsAlive(false);
				}
			}
		}
	}
}

void MovingBlock::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_, objectColor_); }

void MovingBlock::OnCollision(const Player* player) { (void)player; }

Vector3 MovingBlock::GetWorldPosition() {
	Vector3 pos;
	pos.x = worldTransform_.matWorld_.m[3][0];
	pos.y = worldTransform_.matWorld_.m[3][1];
	pos.z = worldTransform_.matWorld_.m[3][2];
	return pos;
}

AABB MovingBlock::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}