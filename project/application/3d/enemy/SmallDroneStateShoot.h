#pragma once
#include <memory>
#include "SmallDrone.h"

namespace Engine {

class BulletManager;
class TimeManager;
class SplineRail;

/// <summary>
/// 小型ドローンの状態：射撃
/// </summary>
class SmallDroneStateShoot : public BaseEnemyState {
public:
	SmallDroneStateShoot(BaseEnemy* smallDrone);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
private:
	// 攻撃
	void Fire();
	void FireTimed();
	void FireCancel();
	// ポインタ
	SmallDrone* smallDrone_ = nullptr;
	Player* player_ = nullptr;
	BulletManager* bulletManager_ = nullptr;
	TimeManager* timeManager_ = nullptr;
	// 時限発射のリスト
	std::list<std::unique_ptr<TimedCall>> timedCalls_;

	// レール移動用
	const SplineRail* rail_ = nullptr;
	float param_ = 0.0f;
	float speed_ = 5.0f; // 0.1fだと遅すぎるので修正
};

} // namespace Engine
