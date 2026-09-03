#pragma once
#include "BaseCharacterState.h"

namespace Engine {

class Input;
class TimeManager;
class Player;
/// <summary>
/// プレイヤーの状態：射撃
/// </summary>
class PlayerStateShoot : public BaseCharacterState{
public:
	// コンストラクタ
	PlayerStateShoot(BaseCharacter* character) : BaseCharacterState("PlayerRoot", character) {};
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
};


} // namespace Engine
