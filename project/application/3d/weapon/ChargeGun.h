#pragma once
#include "BaseWeapon.h"

namespace Engine {

class Input;
class TimeManager;
/// <summary>
/// チャージ銃
/// </summary>
class ChargeGun : public BaseWeapon {
public:
	// コンストラクタ / デストラクタ
	ChargeGun() {};
	~ChargeGun() override {}

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

	/// <summary>
	/// チャージ処理
	/// </summary>
	void Charge() override;
private:
	Input* input_ = nullptr;
	TimeManager* timeManager_ = nullptr;
};


} // namespace Engine
