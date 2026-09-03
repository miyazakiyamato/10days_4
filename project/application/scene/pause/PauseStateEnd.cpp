#include "PauseStateEnd.h"
#include "PauseScene.h" // Full definition needed
#include "SceneManager.h"
#include "Easing.h"

namespace Engine {

void PauseStateEnd::Initialize(PauseScene* scene) {
	BaseSceneState<PauseScene>::Initialize(scene);
	animationTimer_ = 1.0f; // Start from full scale
}

void PauseStateEnd::Update() {
	animationTimer_ -= 1.0f / kAnimationDuration_;
	
	if (animationTimer_ <= 0.0f) {
		animationTimer_ = 0.0f;
		
		// Action (Only trigger if not already transitioning)
		if (!isTransitioning_) {
			if (nextAction_ == 0) {
				// Resume
				isTransitioning_ = true;
				SceneManager::GetInstance()->ReturnScene();
			}
			else if (nextAction_ == 2) {
				// Title with Fade
				isTransitioning_ = true;
				SceneManager::GetInstance()->ChangeTransition("FADE");
				SceneManager::GetInstance()->ChangeScene("TITLE");
			}
		}
	}

	// Update Sprites
	float t = Easing::EaseOutSine(animationTimer_, 0.0f, 1.0f);
	float alpha = t * 0.5f;
	float scale = t;

	auto bgSprite = scene_->GetBgSprite();
	if (bgSprite) {
		bgSprite->SetColor({ 0.0f, 0.0f, 0.0f, alpha });
		bgSprite->SetSize({ 1280.0f, 720.0f });
		bgSprite->Update();
	}

	auto pauseTitle = scene_->GetPauseTitleSprite();
	if (pauseTitle) {
		pauseTitle->SetSize({ 400.0f * scale, 100.0f * scale });
		pauseTitle->Update();
	}

	auto& buttons = scene_->GetButtonSprites();
	int selection = scene_->GetSelectionIndex();
	for (int i = 0; i < buttons.size(); ++i) {
		float baseW = 240.0f;
		float baseH = 50.0f;
		// End animation uses base size to shrink from
		buttons[i]->SetSize({ baseW * scale, baseH * scale });
		buttons[i]->Update();
	}
}

} // namespace Engine
