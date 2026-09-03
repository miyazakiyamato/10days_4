#include "OperationUI.h"
#include "TextureManager.h"
#include "Input.h"
#include "Player.h"
#include "BaseWeapon.h"
#include "WinApp.h"
#include <string>
#ifdef USE_IMGUI
#include <imgui.h>
#endif

namespace Engine {

namespace {
	constexpr float kPadding = 20.0f;
	const Vector2 kDefaultAnchor = { 0.5f, 0.5f };
	constexpr float kCenterY = 650.0f;
}

void OperationUI::Initialize(Player* player) {
	player_ = player;

	// UI Sprites (LST, RST, RB, Hold, Pause)
	std::string uiTextures[] = { "LST.dds", "RST.dds", "RB.dds", "Hold.dds", "pauseButton.dds" };
	// 画像サイズを取得して総幅を計算
	float totalWidth = 0.0f;
	float padding = kPadding;
	std::vector<Vector2> textureSizes;
	for (const auto& textureName : uiTextures) {
		TextureManager::GetInstance()->LoadTexture(textureName);
		const auto& metadata = TextureManager::GetInstance()->GetMetaData(textureName);
		Vector2 size = { static_cast<float>(metadata.width), static_cast<float>(metadata.height) };
		textureSizes.push_back(size);
		totalWidth += size.x;
	}
	totalWidth += padding * (std::size(uiTextures) - 1);

	// センタリングの開始位置計算
	float startX = (WinApp::kClientWidth - totalWidth) / 2.0f;
	float currentX = startX;
	// Y位置を中心基準で設定 (画面下部付近)
	float centerY = kCenterY;

	for (size_t i = 0; i < std::size(uiTextures); ++i) {
		std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
		sprite->Initialize(uiTextures[i]);

		// 画像サイズに合わせる
		sprite->SetSize(textureSizes[i]);
		// アンカーポイントを中心にする
		sprite->SetAnchorPoint(kDefaultAnchor);

		// 位置設定（アンカーが中心なので、Xは現在位置+幅の半分、Yは固定の中心Y）
		sprite->SetPosition({ currentX + textureSizes[i].x / 2.0f, centerY });

		sprites_.push_back(std::move(sprite));

		// 次の位置へ
		currentX += textureSizes[i].x + padding;
	}
}

void OperationUI::Update() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Update();
	}

	// Sprite Input Feedback (Alpha Change)
	// Indices: 0:LST, 1:RST, 2:RB, 3:Hold (Based on initialization order)
	Input* input = Input::GetInstance();
	Vector4 defaultIsColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4 activeColor = { 1.0f, 1.0f, 1.0f, 0.5f };
	if (sprites_.size() >= 4) {

		// 0: LST (Left Stick)
		float lx = input->GetControllerStickLX();
		float ly = input->GetControllerStickLY();
		if (lx != 0.0f || ly != 0.0f) {
			sprites_[0]->SetColor(activeColor);
		} else {
			sprites_[0]->SetColor(defaultIsColor);
		}

		// 1: RST (Right Stick)
		float rx = input->GetControllerStickRX();
		float ry = input->GetControllerStickRY();
		if (rx != 0.0f || ry != 0.0f) {
			sprites_[1]->SetColor(activeColor);
		} else {
			sprites_[1]->SetColor(defaultIsColor);
		}

		bool isRbPressed = input->PushControllerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER);

		if (isRbPressed) {
			BaseWeapon* weapon = player_->GetWeapon();
			bool isCharged = false;
			if (weapon) {
				isCharged = weapon->GetChargeCount() >= weapon->GetKChargeTime();
			}

			if (isCharged) {
				// Charge Complete -> Trigger Hold UI
				sprites_[2]->SetColor(defaultIsColor);
				sprites_[3]->SetColor(activeColor);
			} else {
				// Charging -> Trigger RB UI
				sprites_[2]->SetColor(activeColor);
				sprites_[3]->SetColor(defaultIsColor);
			}
		} else {
			// Not Pressed
			sprites_[2]->SetColor(defaultIsColor);
			sprites_[3]->SetColor(defaultIsColor);
		}
	}

	// 4: Pause (pauseButton.dds)
	if (sprites_.size() >= 5) {
		if (input->PushKey(DIK_P) || input->PushControllerButton(XINPUT_GAMEPAD_START)) {
			sprites_[4]->SetColor(activeColor);
		} else {
			sprites_[4]->SetColor(defaultIsColor);
		}
	}
}

void OperationUI::Draw() {
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}

void OperationUI::ImGuiUpdate() {
#ifdef USE_IMGUI
	uint32_t spriteIDIndex = 0;
	if (ImGui::TreeNode("OperationUI")) {
		for (std::unique_ptr<Sprite>& sprite : sprites_) {
			std::string spriteName = ("OpSprite" + std::to_string(spriteIDIndex)).c_str();
			sprite->ImGuiUpdate(spriteName);
			++spriteIDIndex;
		}
		ImGui::TreePop();
	}
#endif
}

} // namespace Engine
