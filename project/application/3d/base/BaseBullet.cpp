#include "BaseBullet.h"
#include "TimeManager.h"
#include "CollisionTypeIdDef.h"

namespace Engine {

void BaseBullet::Initialize(const Vector3& position, const Vector3& velocity){
	Collider::Initialize();
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetTranslate(position);
	velocity_ = velocity;
}

void BaseBullet::Update(){
	Rotate(velocity_);
	object3d_->SetTranslate(Vector3::Add(object3d_->GetTranslate(), velocity_ * TimeManager::GetInstance()->deltaTime_));
	object3d_->Update();
	deathTimer_ -= TimeManager::GetInstance()->deltaTime_;
	if (deathTimer_ <= 0.0f) {
		isAlive_ = false;
	}
}

void BaseBullet::Draw(){
	object3d_->Draw();
}

void BaseBullet::OnCollision(Collider* other){
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	//衝突相手
	/*if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
		isAlive_ = false;
	}*/
}

void BaseBullet::Rotate(const Vector3& velocity){
	Vector3 rotate = object3d_->GetRotate();
	rotate.y = std::atan2f(velocity.x, velocity.z);
	Vector3 velocityZ = Matrix4x4::Transform(velocity, Matrix4x4::MakeRotateYMatrix(-rotate.y));
	rotate.x = std::atan2f(-velocityZ.y, velocityZ.z);
	object3d_->SetRotate(rotate);
}

Vector3 BaseBullet::GetCenterPosition() const{
	return object3d_->GetCenterPosition();
}

} // namespace Engine
