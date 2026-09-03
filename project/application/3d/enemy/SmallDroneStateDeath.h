#pragma once
#include "SmallDrone.h"

namespace Engine {

class BulletManager;
class TimeManager;
/// <summary>
/// 小型ドローンの状態：死亡
/// </summary>
class SmallDroneStateDeath : public BaseEnemyState {
public:
	SmallDroneStateDeath(BaseEnemy* smallDrone);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
private:
	// ポインタ
	SmallDrone* smallDrone_ = nullptr;
	TimeManager* timeManager_ = nullptr;
	// 死亡用タイマー
	float kDethTimer = 2.0f;
	float dethTimer_ = 0.0f;
};


} // namespace Engine
