#pragma once
#include "BaseScene.h"
#include "BaseSceneState.h"
#include "Sprite.h"
#include "Skydome.h"
#include "Ground.h"
#include <vector>
#include "StageManager.h"
#include "ParticleSystem.h"

namespace Engine {

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public BaseScene {
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
	void ChangeState(std::unique_ptr<BaseSceneState<TitleScene>> newState);

private://メンバ変数
	//ステージマネージャ
	std::unique_ptr<StageManager> stageManager_;
	//パーティクルシステム
	std::unique_ptr<ParticleSystem> particleSystem_;
	//カメラ
	Camera* camera_ = nullptr;

	//ステート
	std::unique_ptr<BaseSceneState<TitleScene>> state_;
public: // ゲッター
	Skydome* GetSkydome() { return stageManager_->GetSkydome(); }
	Ground* GetGround() { return stageManager_->GetGround(); }
	StageManager* GetStageManager() { return stageManager_.get(); }

};

} // namespace Engine
