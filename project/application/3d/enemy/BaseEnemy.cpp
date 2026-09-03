#include "BaseEnemy.h"
#include <CollisionTypeIdDef.h>
#include "ParticleSystem.h"
#include "PlayerBullet.h"
#include "BulletManager.h"
#include "EnemyBullet.h"
#include "Player.h"
#include <cassert>
#include "EmitterSphere.h"
#include "TimeManager.h"
#include "SmallDroneStateShoot.h"
#include "SplineRail.h"

namespace Engine {

void BaseEnemy::Initialize(){
	BaseCharacter::Initialize();
	Collider::Initialize();
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));
	Collider::SetRadius(1.0f);
	ChangeState(std::make_unique<SmallDroneStateShoot>(this));
}

void BaseEnemy::Update() {
	if (state_) {
		state_->Update();
	}
	BaseCharacter::Update();
}

void BaseEnemy::OnCollision(Collider* other){
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	//衝突相手がプレイヤーの弾なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet)) {
		PlayerBullet* playerBullet = static_cast<PlayerBullet*>(other);
		Vector3 distance = playerBullet->GetCenterPosition() - object3d_->GetCenterPosition();
		Damage(int(playerBullet->GetObject3d()->GetScale().x * 4.0f), distance.Normalize());
	}
}

void BaseEnemy::ChangeState(std::unique_ptr<BaseEnemyState> state){
	state_ = std::move(state);
	state_->Initialize();
}

void BaseEnemy::Damage(int damage, const Vector3& hitDirection) {
	static_cast<EmitterSphere*>(particleSystem_->FindEmitter("hitEffect"))->SetTranslate(object3d_->GetCenterPosition() + hitDirection * GetRadius());
	particleSystem_->Emit("hitEffect");
	DamageKnockbackStart(
		hitDirection,     // ダメージ方向
		30.0f,      // どれくらい倒すか（度数）
		0.3f       // 戻るまでの時間
	);
	hp_ -= damage;
	if (hp_ <= 0) {
		hp_ = 0;
	}
}

void BaseEnemy::AddRail(const std::string& name, std::unique_ptr<SplineRail> rail){
	rails_[name] = std::move(rail);
}

const SplineRail* BaseEnemy::GetRail(const std::string& name) const{
	if (rails_.contains(name)) {
		return rails_.at(name).get();
	}
	return nullptr;
}

} // namespace Engine
