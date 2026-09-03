#pragma once
#include "BaseCharacterState.h"
#include "Vector3.h"

namespace Engine {

class Input;
class TimeManager;
class Player;
/// <summary>
/// プレイヤーの状態：離脱演出
/// </summary>
class PlayerStateLeave : public BaseCharacterState{
public:
	// コンストラクタ
	PlayerStateLeave(BaseCharacter* character) : BaseCharacterState("PlayerRoot", character) {};
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

	Vector3 leaveVelocity_ = { 0.0f, 0.0f, 60.0f };
};


} // namespace Engine
