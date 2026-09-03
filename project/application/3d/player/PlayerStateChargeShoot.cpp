#include "PlayerStateChargeShoot.h"
#include "Player.h"
#include "Input.h"
#include "TimeManager.h"
#include "PlayerStateRoot.h"
#include "PlayerBullet.h"
#include "EmitterSphere.h"
#include "ParticleSystem.h"

namespace Engine {

// 初期化
void PlayerStateChargeShoot::Initialize(){
	input_ = Input::GetInstance();
	timeManager_ = TimeManager::GetInstance();
	player_ = static_cast<Player*>(character_);
}

// 更新
void PlayerStateChargeShoot::Update(){
	player_->Move();
	player_->ReticleUpdate();

	// チャージエフェクト更新
	EmitterSphere* chargeEffect = static_cast<EmitterSphere*>(player_->GetParticleSystem()->FindEmitter("chargeEffect"));
	chargeEffect->SetTranslate(player_->GetWeapon()->GetGunBarrel()->GetCenterPosition());
	chargeEffect->SetIsEmitUpdate(true);

	const BaseWeapon::AttackData& attackData = player_->GetWeapon()->GetAttackData(AttackType::kCharge);
	// チャージ継続
	if ((input_->PushKey(DIK_SPACE) || input_->PushControllerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) &&
		attackData.bulletCount == 0) {
		player_->GetWeapon()->Charge();
		return;
	}

	// 発射
	player_->GetWeapon()->Shoot(AttackType::kCharge);

	// 全弾発射したら状態遷移
	if (attackData.bulletCount == 0 ) {
		chargeEffect->SetIsEmitUpdate(false);
		player_->ChangeState(std::make_unique<PlayerStateRoot>(character_));
	}
}

} // namespace Engine
