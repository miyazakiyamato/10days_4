#pragma once

namespace Engine {

/// <summary>
/// シーン状態基底クラス
/// </summary>
template <typename T>
class BaseSceneState {
public:
	virtual ~BaseSceneState() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(T* scene) {
		scene_ = scene;
	}

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

protected:
	T* scene_ = nullptr;
};

} // namespace Engine
