#include "PlayerBullet.h"
#include "TimeManager.h"
#include <CollisionTypeIdDef.h>
#include "TextureManager.h"

namespace Engine {

void PlayerBullet::Initialize(const Vector3& position, const Vector3& velocity){
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet));
	BaseBullet::Initialize(position, velocity);
	object3d_->SetModel("sphere/sphere.obj");
	TextureManager::GetInstance()->LoadTexture("white.dds");
	object3d_->SetTexture("white.dds");
	object3d_->SetEnableLighting(false,0);
	object3d_->SetBlendMode(BlendMode::kAdd);
	Collider::SetRadius(0.5f);
	object3d_->SetScale({ 0.5f, 0.5f, 0.5f });
	object3d_->SetColor({ 0.05f, 1.0f, 0.0f, 1.0f },0);
	Rotate(velocity_);
}

void PlayerBullet::Update() {
	Collider::SetRadius(object3d_->GetScale().x);
	BaseBullet::Update();
}

void PlayerBullet::Draw() { 
	BaseBullet::Draw();
}

void PlayerBullet::OnCollision(Collider* other){
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	//衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
		isAlive_ = false;
	}
}

} // namespace Engine
