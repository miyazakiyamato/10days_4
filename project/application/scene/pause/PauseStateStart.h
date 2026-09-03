#pragma once
#include "BaseSceneState.h"

namespace Engine {

class PauseScene;

class PauseStateStart : public BaseSceneState<PauseScene> {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(PauseScene* scene) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:
	float animationTimer_ = 0.0f;
	const float kAnimationDuration_ = 30.0f;
};

} // namespace Engine
