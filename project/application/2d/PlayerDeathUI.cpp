#include "PlayerDeathUI.h"
#include "TextureManager.h"

namespace Engine {

namespace {
	const Vector2 kDefaultPosition = { 640.0f, 360.0f };
	const Vector2 kDefaultSize = { 90.0f, 144.0f };
	const Vector2 kDefaultAnchor = { 0.5f, 0.5f };
	const Vector2 kInitialUVTranslate = { 0.3f, 0.0f };
	const Vector2 kInitialUVScale = { 0.1f, 1.0f };
}

void PlayerDeathUI::Initialize() {
	TextureManager::GetInstance()->LoadTexture("num.dds");

	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
	sprite->Initialize("num.dds");
	sprite->SetPosition(kDefaultPosition);
	sprite->SetSize(kDefaultSize);
	sprite->SetAnchorPoint(kDefaultAnchor);
	sprite->SetUVTranslate(kInitialUVTranslate);
	sprite->SetUVScale(kInitialUVScale);
	sprite->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	sprites_.push_back(std::move(sprite));
}

void PlayerDeathUI::Update() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Update();
	}
}

void PlayerDeathUI::Draw() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		//sprite->Draw();
	}
}

void PlayerDeathUI::SetSpriteSize(const Vector2& size) {
	if (!sprites_.empty()) {
		sprites_[0]->SetSize(size);
	}
}

void PlayerDeathUI::SetColor(const Vector4& color) {
	if (!sprites_.empty()) {
		sprites_[0]->SetColor(color);
	}
}

} // namespace Engine
