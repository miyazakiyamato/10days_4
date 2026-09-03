#include "PlayerStateRoot.h"
#include "Player.h"
#include "Input.h"
#include "TimeManager.h"
#include "PlayerStateShoot.h"
#include "PlayerStateChargeShoot.h"

namespace Engine {

// 初期化
void PlayerStateRoot::Initialize(){
	input_ = Input::GetInstance();
	timeManager_ = TimeManager::GetInstance();
	player_ = static_cast<Player*>(character_);
}

// 更新
void PlayerStateRoot::Update(){
	player_->Move();
	player_->ReticleUpdate();

	if (player_->GetIsCpuMode()) { return; }

	if (input_->PushKey(DIK_SPACE) || input_->PushControllerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		player_->GetWeapon()->Charge();
		if (player_->GetWeapon()->GetChargeCount() >= player_->GetWeapon()->GetKChargeTime()) {
			player_->ChangeState(std::make_unique<PlayerStateChargeShoot>(character_));
			return;
		}
	} else {
		if (player_->GetWeapon()->GetChargeCount() != 0.0f) {
			player_->ChangeState(std::make_unique<PlayerStateShoot>(character_));
			return;
		}
	}
}

} // namespace Engine
