#pragma once
#include "BaseCharacterState.h"
#include "Vector3.h"

namespace Engine {

class Input;
class TimeManager;
class Player;
/// <summary>
/// プレイヤーの状態：クリア演出
/// </summary>
class PlayerStateClear : public BaseCharacterState{
public:
	// コンストラクタ
	PlayerStateClear(BaseCharacter* character) : BaseCharacterState("PlayerRoot", character) {};
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
private:
	Input* input_ = nullptr;
	TimeManager* timeManager_ = nullptr;
	Player* player_ = nullptr;

	// clear毎フレーム更新する角度（公転）
	float revolveAngle_ = 0.0f;
};


} // namespace Engine
