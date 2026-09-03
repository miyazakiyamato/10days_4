#include "GameStartUI.h"
#include "TextureManager.h"
#include "TimeManager.h"
#include <cmath>

namespace Engine {

namespace {
	const Vector2 kDefaultSpriteSize = { 180.0f, 144.0f };
	const Vector2 kDefaultPosition = { 640.0f, 360.0f };
	const Vector2 kDefaultAnchor = { 0.5f, 0.5f };
	const Vector2 kInitialUVTranslate = { 0.3f, 0.0f };
	const Vector2 kInitialUVScale = { 0.1f, 1.0f };
	constexpr float kUVStep = 0.1f;
	constexpr float kPi = 3.14159265f;
}

void GameStartUI::Initialize(float duration) {
	duration_ = duration;
	spriteSize_ = kDefaultSpriteSize; 
	
	TextureManager::GetInstance()->LoadTexture("num.dds");
	TextureManager::GetInstance()->LoadTexture("go.dds");

	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
	sprite->Initialize("num.dds");
	sprite->SetPosition(kDefaultPosition);
	sprite->SetSize(spriteSize_);
	sprite->SetAnchorPoint(kDefaultAnchor);
	sprite->SetUVTranslate(kInitialUVTranslate);
	sprite->SetUVScale(kInitialUVScale);
	sprite->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	sprites_.push_back(std::move(sprite));
}

void GameStartUI::Update() {
	counter_ += TimeManager::GetInstance()->kFlamTime_;

	if (!sprites_.empty()) {
		sprites_[0]->SetSize(spriteSize_ * counter_ * 2);
		if (counter_ < duration_) {
			float t = (counter_ / duration_) * kPi;
			float alpha = std::sinf(t);
			alpha = std::abs(alpha);
			sprites_[0]->SetColor({ 1.0f, 1.0f, 1.0f, alpha });
		}
		else {
			counter_ = 0.0f;
			sprites_[0]->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
			if (sprites_[0]->GetUVTranslate().x - kUVStep >= kUVStep) {
				sprites_[0]->SetUVTranslate({ sprites_[0]->GetUVTranslate().x - kUVStep, 0.0f });
			}
			else if (sprites_[0]->GetUVTranslate().x - kUVStep >= 0.0f) {
				sprites_[0]->SetTexture("go.dds");
				sprites_[0]->SetUVTranslate({ 0.0f, 0.0f });
				spriteSize_ = kDefaultSpriteSize;
				sprites_[0]->SetUVScale({ 1.0f, 1.0f });
				isFinished_ = true;
			}
			else {
				shouldClose_ = true;
			}
		}

		sprites_[0]->Update();
	}
}

void GameStartUI::Draw() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}



} // namespace Engine
