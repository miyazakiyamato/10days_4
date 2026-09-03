#include "EnemyBullet.h"
#include "TimeManager.h"
#include <CollisionTypeIdDef.h>

namespace Engine {

void EnemyBullet::Initialize(const Vector3& position, const Vector3& velocity){
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemyBullet));
	BaseBullet::Initialize(position, velocity);
	object3d_->SetModel("sphere/sphere.obj");
	Collider::SetRadius(0.5f);
	object3d_->SetScale({ 0.5f, 0.5f, 0.5f });
	object3d_->SetColor({ 2.2f, 2.2f, 0.2f, 0.7f },0);
	object3d_->SetTexture("white.dds");
	Rotate(velocity_);
}

void EnemyBullet::Update() {
	BaseBullet::Update();
}

void EnemyBullet::Draw() {
	BaseBullet::Draw();
}

void EnemyBullet::OnCollision(Collider* other){
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	//衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayer)) {
		isAlive_ = false;
	}
}

} // namespace Engine
