#include "GameOverSceneStateMain.h"
#include "GameOverScene.h"
#include "Input.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "GameOverUI.h"

namespace Engine {

void GameOverSceneStateMain::Initialize(GameOverScene* scene) {
	BaseSceneState<GameOverScene>::Initialize(scene);

	// GameOver UI
	uiManager_ = std::make_unique<UIManager>();
	
	std::unique_ptr<GameOverUI> gameOverUI = std::make_unique<GameOverUI>();
	gameOverUI->Initialize();
	uiManager_->AddUI(std::move(gameOverUI));
}

void GameOverSceneStateMain::Update() {
	auto input = Input::GetInstance();
	auto sceneManager = SceneManager::GetInstance();

	if (input->TriggerKey(DIK_SPACE) || input->TriggerControllerButton(XINPUT_GAMEPAD_A)) {
		sceneManager->ChangeScene("TITLE");
		sceneManager->ChangeTransition("FADE");
	}

	//カメラの更新
	CameraManager::GetInstance()->GetCamera()->Update();

	//天球の更新
	scene_->GetSkydome()->Update();
	//地面の更新
	scene_->GetGround()->Update();

	uiManager_->Update();
}

void GameOverSceneStateMain::Draw() {
	scene_->GetSkydome()->Draw();
	scene_->GetGround()->Draw();

	uiManager_->Draw();
}

} // namespace Engine
