#include "BaseWeapon.h"
#include "TimeManager.h"
#include "CollisionTypeIdDef.h"
#include "BulletManager.h"
#include "PlayerBullet.h"

namespace Engine {

void BaseWeapon::Initialize(){
	Collider::Initialize();
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	gunBarrel_ = std::make_unique<Object3d>();
	gunBarrel_->Initialize();

	attackData_.push_back(AttackData{});
	attackData_.push_back(AttackData{});
}

void BaseWeapon::Update(){
	object3d_->Update();
	gunBarrel_->Update();
}

void BaseWeapon::Draw(){
	object3d_->Draw();
	gunBarrel_->Draw();
}

void BaseWeapon::OnCollision([[maybe_unused]] Collider* other){
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	//衝突相手
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kDefault)) {
		
	}
}

void BaseWeapon::Shoot(AttackType attackType){
	// 発射
	AttackData& attackData = attackData_[static_cast<uint32_t>(attackType)];
	attackData.coolTimeCount += TimeManager::GetInstance()->deltaTime_;
	if (attackData.bulletCount == 0 ||
		attackData.coolTimeCount >= attackData.kCoolTime) {
		// 弾の速度
		Vector3 velocity(0, 0, attackData.kBulletSpeed);
		velocity = Vector3::Subtract(target_->GetCenterPosition(), gunBarrel_->GetCenterPosition());
		velocity = Vector3::Multiply(attackData.kBulletSpeed, Vector3::Normalize(velocity));
		// 弾の生成
		std::unique_ptr<BaseBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(gunBarrel_->GetCenterPosition(), velocity);
		newBullet->GetObject3d()->SetScale({ attackData.bulletSize,attackData.bulletSize,attackData.bulletSize });
		bulletManager_->AddBullet(std::move(newBullet));

		// 次の弾への初期化
		attackData.bulletCount++;
		attackData.coolTimeCount = 0.0f;
	}

	// 全弾発射したら状態遷移
	if (attackData.bulletCount >= attackData.kBulletCount) {
		attackData.bulletCount = 0;
		attackData.coolTimeCount = 0.0f;
		chargeCount_ = 0.0f;
	}
}

void BaseWeapon::Charge(){
	chargeCount_ += TimeManager::GetInstance()->deltaTime_;
}

void BaseWeapon::Rotate(const Vector3& velocity){
	Vector3 rotate = object3d_->GetRotate();
	rotate.y = std::atan2f(velocity.x, velocity.z);
	Vector3 velocityZ = Matrix4x4::Transform(velocity, Matrix4x4::MakeRotateYMatrix(-rotate.y));
	rotate.x = std::atan2f(-velocityZ.y, velocityZ.z);
	object3d_->SetRotate(rotate);
}

Vector3 BaseWeapon::GetCenterPosition() const{
	return object3d_->GetCenterPosition();
}

} // namespace Engine
