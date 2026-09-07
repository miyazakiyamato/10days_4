#include "goalBlock.h"
#include <cassert>
#include <Player.h>

void GoalBlock::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();
}

void GoalBlock::Update() {
	worldTransform_.UpdateMatrix();
}

void GoalBlock::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); }

void GoalBlock::OnCollision(const Player* player) {
	(void)player;
}

Vector3 GoalBlock::GetWorldPosition() {
	Vector3 pos;
	pos.x = worldTransform_.matWorld_.m[3][0];
	pos.y = worldTransform_.matWorld_.m[3][1];
	pos.z = worldTransform_.matWorld_.m[3][2];
	return pos;
}

AABB GoalBlock::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}
