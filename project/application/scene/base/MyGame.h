#pragma once
#include "Framework.h"
#include "BaseScene.h"

namespace Engine {

/// <summary>
/// ゲーム本体
/// </summary>
class MyGame : public Framework{
public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
private://メンバ変数
	std::string pipelineStateName_ = "";
};


} // namespace Engine
