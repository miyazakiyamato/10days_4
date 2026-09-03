#include "SmallDroneStateShoot.h"
#include <sstream>
#include <Windows.h>
#include "SmallDrone.h"
#include "Player.h"
#include "BulletManager.h"
#include "TimeManager.h"
#include "Easing.h"
#include "EnemyBullet.h"
#include "SmallDroneStateDeath.h"
#include "SplineRail.h"

namespace Engine {

SmallDroneStateShoot::SmallDroneStateShoot(BaseEnemy* smallDrone)
	: BaseEnemyState("SmallDroneStateShoot", smallDrone){}

void SmallDroneStateShoot::Initialize(){
	smallDrone_ = static_cast<SmallDrone*>(enemy_);
	player_ = enemy_->GetPlayer();
	bulletManager_ = enemy_->GetBulletManager();
	timeManager_ = TimeManager::GetInstance();
	// レール取得
	rail_ = enemy_->GetRail(name_);
	
	// 発射開始
	timedCalls_.push_back(std::make_unique<TimedCall>(std::bind_front(&SmallDroneStateShoot::FireTimed, this), smallDrone_->GetFireInterval()));
}

void SmallDroneStateShoot::Update(){
	// 死亡処理Stateへ移行
	if (smallDrone_->GetHP() <= 0) {
		FireCancel();
		enemy_->ChangeState(std::make_unique<SmallDroneStateDeath>(enemy_));
		return;
	}

	// レール移動
	if (rail_) {
		float length = rail_->GetLength();
		// 速度調整 (全長に対する割合で進む必要があるため、速度/全長を加算)
		if (length > 0.0f) {
			param_ += (speed_ * TimeManager::deltaTime_) / length; // lengthで割ることで一定速度にする
			if (param_ >= 1.0f) {
				param_ = 1.0f;
				// レール終了時に消滅（死亡ステートへ移行）
				FireCancel();
				enemy_->ChangeState(std::make_unique<SmallDroneStateDeath>(enemy_));
				return;
			}
		}
		Vector3 pos = rail_->GetPosition(param_);
		smallDrone_->SetPosition(pos);
	}

	// 時間経過
	timedCalls_.remove_if([](const std::unique_ptr<TimedCall>& timedCall) {
		return timedCall->IsFinished();
		});
	for (const auto& timedCall : timedCalls_) {
		timedCall->Update();
	}
}

void SmallDroneStateShoot::Fire() {
	assert(player_);
	//
	const float kBulletSpeed = 30.0f;

	Vector3 velocity{ player_->GetWorldPosition() - smallDrone_->GetObject3d()->GetCenterPosition() };
	velocity = Vector3::Multiply(kBulletSpeed, velocity.Normalize());

	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(smallDrone_->GetObject3d()->GetCenterPosition(), velocity);
	bulletManager_->AddBullet(std::move(newBullet));
}
void SmallDroneStateShoot::FireTimed() {
	Fire();
	//
	timedCalls_.push_back(std::make_unique<TimedCall>(std::bind_front(&SmallDroneStateShoot::FireTimed, this), smallDrone_->GetFireInterval()));
}

void SmallDroneStateShoot::FireCancel() {
	timedCalls_.clear();
}

} // namespace Engine
