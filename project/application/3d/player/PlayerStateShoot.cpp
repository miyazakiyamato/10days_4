#include "PlayerStateShoot.h"
#include "Player.h"
#include "Input.h"
#include "TimeManager.h"
#include "PlayerStateRoot.h"

namespace Engine {

// 初期化
void PlayerStateShoot::Initialize(){
	input_ = Input::GetInstance();
	timeManager_ = TimeManager::GetInstance();
	player_ = static_cast<Player*>(character_);
}

// 更新
void PlayerStateShoot::Update(){
	player_->Move();
	player_->ReticleUpdate();
	
	// 発射
	const BaseWeapon::AttackData& attackData = player_->GetWeapon()->GetAttackData(AttackType::kRoot);
	player_->GetWeapon()->Shoot(AttackType::kRoot);

	// 全弾発射したら状態遷移
	if (attackData.bulletCount == 0) {
		player_->ChangeState(std::make_unique<PlayerStateRoot>(character_));
	}
}

} // namespace Engine
