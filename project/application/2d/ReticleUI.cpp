#include "ReticleUI.h"
#include "TextureManager.h"
#include "WinApp.h"
#ifdef USE_IMGUI
#include <imgui.h>
#endif

namespace Engine {

void ReticleUI::Initialize() {
	sprite_ = std::make_unique<Sprite>();
	TextureManager::GetInstance()->LoadTexture("reticle.dds");
	sprite_->Initialize("reticle.dds");
	
	// 初期場所設定　中央
	sprite_->SetPosition({ WinApp::kClientWidth / 2.0f, WinApp::kClientHeight / 2.0f });
	sprite_->SetAnchorPoint({ 0.5f, 0.5f });
	sprite_->SetSize({ 64.0f, 64.0f });
	sprite_->Update();
}

void ReticleUI::Update() {
	if (sprite_) {
		sprite_->Update();
	}
}

void ReticleUI::Draw() {
	if (sprite_) {
		sprite_->Draw();
	}
}

void ReticleUI::ImGuiUpdate() {
#ifdef USE_IMGUI
	if (sprite_) {
		if (ImGui::TreeNode("ReticleUI")) {
			sprite_->ImGuiUpdate("ReticleSprite");
			ImGui::TreePop();
		}
	}
#endif
}

Vector2 ReticleUI::GetPosition() const {
	if (sprite_) {
		return sprite_->GetPosition();
	}
	return { 0.0f, 0.0f };
}

void ReticleUI::SetPosition(const Vector2& position) {
	if (sprite_) {
		sprite_->SetPosition(position);
	}
}

} // namespace Engine
