#include "TitleSceneStateMain.h"
#include "TitleScene.h"
#include "Input.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "PostEffectManager.h"

#include "TitleUI.h"
#include "ReticleUI.h"

namespace Engine {

void TitleSceneStateMain::Initialize(TitleScene* scene) {
	BaseSceneState<TitleScene>::Initialize(scene);
	
	uiManager_ = std::make_unique<UIManager>();

	// Title UI
	std::unique_ptr<TitleUI> titleUI = std::make_unique<TitleUI>();
	titleUI->Initialize();
	uiManager_->AddUI(std::move(titleUI));

	// Reticle UI (for Play Demo)
	std::unique_ptr<ReticleUI> reticleUI = std::make_unique<ReticleUI>();
	reticleUI->Initialize();
	
	// PlayerにReticleをセット
	Player* player = scene_->GetStageManager()->GetPlayer();
	if (player) {
		player->SetReticleUI(reticleUI.get());
	}
	
	uiManager_->AddUI(std::move(reticleUI));
}

void TitleSceneStateMain::Update() {
	// 各メンバの更新
	uiManager_->Update();

	// カメラの回転
	// RailCameraが制御するため不要

	// シーン遷移
	if (Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->TriggerControllerButton(XINPUT_GAMEPAD_A)) {
		SceneManager::GetInstance()->ChangeScene("GAME");
		SceneManager::GetInstance()->ChangeTransition("FADE");
	}
}

void TitleSceneStateMain::Draw() {
	uiManager_->Draw();
}

} // namespace Engine
