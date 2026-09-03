#pragma once
#include "BaseSceneState.h"

namespace Engine {

class PauseScene;

class PauseStateEnd : public BaseSceneState<PauseScene> {
public:
	PauseStateEnd(int nextAction) : nextAction_(nextAction) {}

	void Initialize(PauseScene* scene) override;
	void Update() override;
	void Draw() override {}

private:
	int nextAction_ = -1; // 0: Resume, 2: Title
	float animationTimer_ = 0.0f;
	const float kAnimationDuration_ = 30.0f;
	bool isTransitioning_ = false;
};

} // namespace Engine
