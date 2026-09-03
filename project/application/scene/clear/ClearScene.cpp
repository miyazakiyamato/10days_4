#include "ClearScene.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "CameraManager.h"
#include "Easing.h"
#include "TimeManager.h"
#include "PlayerStateClear.h"

#include "ClearSceneStateMain.h"

namespace Engine {

void ClearScene::ChangeState(std::unique_ptr<BaseSceneState<ClearScene>> newState) {
	state_ = std::move(newState);
	state_->Initialize(this);
}

void ClearScene::Initialize(){
	BaseScene::Initialize();

	CameraManager::GetInstance()->FindCamera("default");
	CameraManager::GetInstance()->GetCamera()->SetRotate({ 0.0f,0.0f,0.0f });
	CameraManager::GetInstance()->GetCamera()->SetTranslate({ 8.0f,4.0f,-8.0f });

	ModelManager::GetInstance()->LoadModel("skydome/skydome.obj");
	ModelManager::GetInstance()->LoadModel("ground/ground.obj");
	ModelManager::GetInstance()->LoadModel("airship/airship.obj");
	TextureManager::GetInstance()->LoadTexture("clear.dds");

	//天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();
	//地面
	ground_ = std::make_unique<Ground>();
	ground_->Initialize();

	ChangeState(std::make_unique<ClearSceneStateMain>());
}



void ClearScene::Update() {
	BaseScene::Update();


	if (state_) {
		state_->Update();
	}
}

void ClearScene::Draw(){
	if (state_) {
		state_->Draw();
	}
}

} // namespace Engine
