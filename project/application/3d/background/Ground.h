#pragma once
#include <memory>
#include <Object3d.h>

namespace Engine {

/// <summary>
/// 地面
/// </summary>
class Ground{
public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
private://メンバ変数
	std::unique_ptr<Object3d> object3d;
};


} // namespace Engine
