#include "GameOverUI.h"
#include "TextureManager.h"
#include "TimeManager.h"
#include <cmath>

namespace Engine {

namespace {
	const Vector2 kGameOverSpritePos = { 640.0f, 260.0f };
	const Vector2 kGameOverSpriteSize = { 720.0f, 140.0f };
	const Vector2 kGameOverSpriteAnchor = { 0.5f, 0.5f };

	const Vector2 kTitleSpritePos = { 640.0f, 460.0f };
	const Vector2 kTitleSpriteSize = { 160.0f, 50.0f };
	const Vector2 kTitleSpriteAnchor = { 0.5f, 0.5f };
	constexpr float kFlashSpeed = 2.0f;
}

void GameOverUI::Initialize() {
	TextureManager::GetInstance()->LoadTexture("gameOver.dds");
	TextureManager::GetInstance()->LoadTexture("ATitle.dds");

	//スプライトの初期化
	// game over sprite
	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
	sprite->Initialize("gameOver.dds");
	sprite->SetPosition(kGameOverSpritePos);
	sprite->SetSize(kGameOverSpriteSize);
	sprite->SetAnchorPoint(kGameOverSpriteAnchor);
	sprites_.push_back(std::move(sprite));

	// ATitle sprite
	std::unique_ptr<Sprite> spriteTitle = std::make_unique<Sprite>();
	spriteTitle->Initialize("ATitle.dds");
	spriteTitle->SetPosition(kTitleSpritePos);
	spriteTitle->SetSize(kTitleSpriteSize);
	spriteTitle->SetAnchorPoint(kTitleSpriteAnchor);
	sprites_.push_back(std::move(spriteTitle));
}

void GameOverUI::Update() {
	animationTime_ += TimeManager::GetInstance()->deltaTime_;
	float alpha = (std::sin(animationTime_ * kFlashSpeed) + 1.0f) / 2.0f;
	
	if (sprites_.size() > 1) {
		sprites_[1]->SetColor({ 1.0f,1.0f,1.0f, alpha });
	}

	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Update();
	}
}

void GameOverUI::Draw() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}

} // namespace Engine
