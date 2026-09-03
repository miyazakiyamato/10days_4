#include "ClearUI.h"
#include "TextureManager.h"
#include "TimeManager.h"
#include "Easing.h"
#include <cmath>

namespace Engine {

namespace {
	const Vector2 kClearSpritePos = { 640.0f, 260.0f };
	const Vector2 kClearSpriteSize = { 360.0f, 140.0f };
	const Vector2 kClearSpriteAnchor = { 0.5f, 0.5f };

	const Vector2 kTitleSpritePos = { 640.0f, 460.0f };
	const Vector2 kTitleSpriteSize = { 160.0f, 50.0f };
	const Vector2 kTitleSpriteAnchor = { 0.5f, 0.5f };
}

void ClearUI::Initialize() {
	TextureManager::GetInstance()->LoadTexture("clear.dds");
	TextureManager::GetInstance()->LoadTexture("ATitle.dds");

	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
	sprite->Initialize("clear.dds");
	sprite->SetPosition(kClearSpritePos);
	sprite->SetSize(kClearSpriteSize);
	sprite->SetAnchorPoint(kClearSpriteAnchor);
	sprites_.push_back(std::move(sprite));

	// ATitle sprite
	std::unique_ptr<Sprite> spriteTitle = std::make_unique<Sprite>();
	spriteTitle->Initialize("ATitle.dds");
	spriteTitle->SetPosition(kTitleSpritePos);
	spriteTitle->SetSize(kTitleSpriteSize);
	spriteTitle->SetAnchorPoint(kTitleSpriteAnchor);
	sprites_.push_back(std::move(spriteTitle));
}

void ClearUI::Update() {
	animationTime_ += TimeManager::GetInstance()->deltaTime_;
	float alpha = (std::sin(animationTime_ * 2.0f) + 1.0f) / 2.0f;

	if (sprites_.size() > 1) {
		sprites_[1]->SetColor({ 1.0f,1.0f,1.0f, alpha });
		sprites_[1]->Update();
	}

	scaleCount_ += TimeManager::GetInstance()->deltaTime_;
	if (scaleCount_ > 1.0f) {
		scaleCount_ = 0.0f;
	}
	float scale{};
	if (scaleCount_ <= 0.5f) {
		scale = Easing::EaseInOutSine(scaleCount_ * 2.0f, 0.5f, 1.0f);
	}
	else {
		scale = Easing::EaseInOutSine(scaleCount_ * 2.0f - 1.0f, 1.0f, 0.5f);
	}
	
	if (!sprites_.empty()) {
		sprites_[0]->SetSize(kClearSpriteSize * scale);
		sprites_[0]->Update();
	}
}

void ClearUI::Draw() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}

} // namespace Engine
