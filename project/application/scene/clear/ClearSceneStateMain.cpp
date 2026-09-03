#include "ClearSceneStateMain.h"
#include "ClearScene.h"
#include "Input.h"
#include "SceneManager.h"
#include "CameraManager.h"

#include "PlayerStateClear.h"
#include "ClearUI.h"

namespace Engine {

void ClearSceneStateMain::Initialize(ClearScene* scene) {
	BaseSceneState<ClearScene>::Initialize(scene);

	//プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize();
	player_->ChangeState(std::make_unique<PlayerStateClear>(player_.get()));

	// Clear UI
	uiManager_ = std::make_unique<UIManager>();

	std::unique_ptr<ClearUI> clearUI = std::make_unique<ClearUI>();
	clearUI->Initialize();
	uiManager_->AddUI(std::move(clearUI));
}

void ClearSceneStateMain::Update() {
	auto input = Input::GetInstance();
	auto sceneManager = SceneManager::GetInstance();

	if (input->TriggerKey(DIK_SPACE) || input->TriggerControllerButton(XINPUT_GAMEPAD_A)){
		sceneManager->ChangeScene("TITLE");
		sceneManager->ChangeTransition("FADE");
	}

	//カメラの更新
	CameraManager::GetInstance()->GetCamera()->Update();

	//天球の更新
	scene_->GetSkydome()->Update();
	//地面の更新
	scene_->GetGround()->Update();
	//プレイヤーの更新
	player_->Update();

	uiManager_->Update();
}

void ClearSceneStateMain::Draw() {
	scene_->GetSkydome()->Draw();
	scene_->GetGround()->Draw();
	player_->Draw();
	
	uiManager_->Draw();
}

} // namespace Engine
