#include "PauseStateStart.h"
#include "PauseScene.h"
#include "PauseStateSelect.h"
#include "Easing.h"

namespace Engine {

void PauseStateStart::Initialize(PauseScene* scene) {
	BaseSceneState<PauseScene>::Initialize(scene);
	animationTimer_ = 0.0f;
}

void PauseStateStart::Update() {
	animationTimer_ += 1.0f / kAnimationDuration_;
	if (animationTimer_ >= 1.0f) {
		animationTimer_ = 1.0f;
		// 完了したらSelectステートへ
		scene_->ChangeState(std::make_unique<PauseStateSelect>());
		return; 
	}

	// アニメーション更新
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
		pauseTitle->SetSize({ 400.0f * scale, 100.0f * scale }); // 4:1 ratio (200x50 original)
		pauseTitle->Update();
	}

	auto& buttons = scene_->GetButtonSprites();
	int selection = scene_->GetSelectionIndex();
	for (int i = 0; i < buttons.size(); ++i) {
		float baseW = 240.0f;
		float baseH = 50.0f;
		if (i == selection) {
			// Start animation targets base size, Selection state handles pulsing
			baseW = 240.0f; 
			baseH = 50.0f;
		}
		buttons[i]->SetSize({ baseW * scale, baseH * scale });
		buttons[i]->Update();
	}
}

void PauseStateStart::Draw() {
	
}

} // namespace Engine
