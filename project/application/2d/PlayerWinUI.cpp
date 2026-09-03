#include "PlayerWinUI.h"
#include "TextureManager.h"

namespace Engine {

void PlayerWinUI::Initialize() {
	
}

void PlayerWinUI::Update() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Update();
	}
}

void PlayerWinUI::Draw() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}

void PlayerWinUI::SetSpriteSize(const Vector2& size) {
	if (!sprites_.empty()) {
		sprites_[0]->SetSize(size);
	}
}

void PlayerWinUI::SetColor(const Vector4& color) {
	if (!sprites_.empty()) {
		sprites_[0]->SetColor(color);
	}
}

} // namespace Engine
