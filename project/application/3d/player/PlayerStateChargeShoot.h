#pragma once
#include "BaseCharacterState.h"

namespace Engine {

class Input;
class TimeManager;
class Player;
/// <summary>
/// プレイヤーの状態：溜め撃ち
/// </summary>
class PlayerStateChargeShoot : public BaseCharacterState{
public:
	// コンストラクタ
	PlayerStateChargeShoot(BaseCharacter* character) : BaseCharacterState("PlayerRoot", character) {};
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
