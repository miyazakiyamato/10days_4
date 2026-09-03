#include "SmallDroneStateDeath.h"
#include "SmallDrone.h"
#include "Player.h"
#include "BulletManager.h"
#include "TimeManager.h"
#include "Easing.h"
#include "EnemyBullet.h"

namespace Engine {

SmallDroneStateDeath::SmallDroneStateDeath(BaseEnemy* smallDrone)
	: BaseEnemyState("SmallDroneStateDeath", smallDrone){}

void SmallDroneStateDeath::Initialize(){
	smallDrone_ = static_cast<SmallDrone*>(enemy_);
	timeManager_ = TimeManager::GetInstance();
}

void SmallDroneStateDeath::Update(){
	dethTimer_ += TimeManager::deltaTime_;
	smallDrone_->GetObject3d()->SetTranslate(smallDrone_->GetObject3d()->GetTranslate() + Vector3(0, -10, 0) * TimeManager::deltaTime_);
	if (dethTimer_ >= kDethTimer) {
		smallDrone_->SetIsAlive(false);
	}
}

} // namespace Engine
