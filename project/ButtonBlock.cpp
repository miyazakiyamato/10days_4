#include "ButtonBlock.h"
#include <Player.h>
#include <cassert>
#include "GameScene.h"

void ButtonBlock::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();

	objectColor_ = new ObjectColor();
	objectColor_->Initialize();
	objectColor_->SetColor(GetColorById(id_));
	objectColor_->TransferMatrix();
}

void ButtonBlock::Update() {
	if (isCollidingThisFrame_ && !wasCollidingLastFrame_) {
		isActive_ = !isActive_;
	}
	wasCollidingLastFrame_ = isCollidingThisFrame_;
	isCollidingThisFrame_ = false;

	
	Vector4 color = GetColorById(id_);
	color.w = isActive_ ? 0.5f : 1.0f;
	objectColor_->SetColor(color);

	objectColor_->TransferMatrix();
	worldTransform_.UpdateMatrix();
}

void ButtonBlock::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_,objectColor_); }

void ButtonBlock::OnCollision(const Player* player) {
	(void)player;
	isCollidingThisFrame_ = true;
}

Vector3 ButtonBlock::GetWorldPosition() {
	Vector3 pos;
	pos.x = worldTransform_.matWorld_.m[3][0];
	pos.y = worldTransform_.matWorld_.m[3][1];
	pos.z = worldTransform_.matWorld_.m[3][2];
	return pos;
}

AABB ButtonBlock::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}

void ButtonBlock::SetId(int id) {
	id_ = id;
	objectColor_->SetColor(GetColorById(id_));
	objectColor_->TransferMatrix();
}