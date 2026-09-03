#pragma once
#include "Input.h"

namespace Engine {

class SceneManager;
/// <summary>
/// シーン基底クラス
/// </summary>
class BaseScene{
public://メンバ関数
	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~BaseScene() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;
protected://継承
	SceneManager* sceneManager_ = nullptr;
	Input* input_ = nullptr;
public://ゲッターセッター
	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }
};


} // namespace Engine
