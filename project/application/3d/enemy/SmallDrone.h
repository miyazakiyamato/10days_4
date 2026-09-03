#pragma once
#include "BaseEnemy.h"

namespace Engine {

/// <summary>
/// 小型ドローン（敵）
/// </summary>
class SmallDrone : public BaseEnemy{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 衝突時コールバック
	/// </summary>
	/// <param name="other">衝突相手</param>
	void OnCollision([[maybe_unused]] Collider* other) override;
};


} // namespace Engine
