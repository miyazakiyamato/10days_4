#pragma once
#include "BaseScene.h"
#include "BaseSceneState.h"
#include "Sprite.h"
#include "Skydome.h"
#include "Ground.h"
#include "Player.h"
#include "ClearUI.h"

namespace Engine {

/// <summary>
/// クリアシーン
/// </summary>
class ClearScene : public BaseScene {
public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	/// <summary>
	/// ステート変更
	/// </summary>
	void ChangeState(std::unique_ptr<BaseSceneState<ClearScene>> newState);

private://メンバ変数
	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//地面
	std::unique_ptr<Ground> ground_ = nullptr;

	float scaleCount_ = 0.0f;

	//ステート
	std::unique_ptr<BaseSceneState<ClearScene>> state_;
public:
	Skydome* GetSkydome() { return skydome_.get(); }
	Ground* GetGround() { return ground_.get(); }
};

} // namespace Engine
