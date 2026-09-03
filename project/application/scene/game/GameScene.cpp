#include "GameScene.h"
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI

#include "SceneManager.h"
#include "Input.h"
#include "CameraManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "AudioManager.h"
#include "ParticleManager.h"
#include "GlobalVariables.h"
#include "TimeManager.h"
#include "Line3D.h"
#include "EmitterSphere.h"
#include "PostEffectManager.h"
#include <numbers>
#include <Easing.h>
#include "PlayerStateLeave.h"
#include <ChargeGun.h>
#include "SmallDrone.h"
#include "HpUI.h"
#include "OperationUI.h"
#include "GameSceneStateStart.h"
#include "../pause/PauseScene.h"

namespace Engine {

void GameScene::ChangeState(std::unique_ptr<BaseSceneState<GameScene>> newState) {
	state_ = std::move(newState);
	state_->Initialize(this);
}

void GameScene::Initialize(){
	BaseScene::Initialize();
#ifdef USE_IMGUI
	//開発用のUIの処理。
	// ウインドウのサイズを固定する
	ImGui::SetNextWindowSize(ImVec2(1280, 40));
	// ウインドウの位置を設定する
	ImGui::SetNextWindowPos(ImVec2(0, 0));
#endif // USE_IMGUI

	CameraManager::GetInstance()->SetCamera("Camera2");
	CameraManager::GetInstance()->FindCamera("Camera2");
	CameraManager::GetInstance()->GetCamera()->SetRotate({ 0.3f,-0.8f,0.0f });
	CameraManager::GetInstance()->GetCamera()->SetTranslate({ 8.0f,4.0f,-8.0f });

	CameraManager::GetInstance()->FindCamera("default");

	AudioManager::GetInstance()->LoadWave("maou_se_system48.wav");
	//AudioManager::GetInstance()->LoadMP3("audiostock_1420737.mp3");

	//衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();

	TextureManager::GetInstance()->LoadTexture("circle2.dds");
	TextureManager::GetInstance()->LoadTexture("gradationLine.dds");
	TextureManager::GetInstance()->LoadTexture("reticle.dds");
	TextureManager::GetInstance()->LoadTexture("flash.dds");
	TextureManager::GetInstance()->LoadTexture("rostock_laage_airport_4k.dds");

	//パーティクルシステムの生成
	particleSystem_ = std::make_unique<ParticleSystem>();

	std::unique_ptr<EmitterSphere> emitterHit = std::make_unique<EmitterSphere>();
	emitterHit->Initialize("emitterHit", 100);
	emitterHit->SetTranslate({ 1.0f,1.0f,0.0f });
	emitterHit->SetTexture("flash.dds");
	particleSystem_->SetParticleEmitter(std::move(emitterHit));

	std::unique_ptr<BaseParticleEmitter> hitEffect = std::make_unique<EmitterSphere>();
	hitEffect->Initialize("hitEffect", 100);
	hitEffect->SetPosition({ 1.0f,1.0f,0.0f });
	hitEffect->SetTexture("circle2.dds");
	particleSystem_->SetParticleEmitter(std::move(hitEffect));

	std::unique_ptr<EmitterSphere> airEffect = std::make_unique<EmitterSphere>();
	airEffect->Initialize("airEffect", 1000);
	airEffect->SetTranslate({ 0.0f,0.0f,0.0f });
	airEffect->SetTexture("circle2.dds");
	particleSystem_->SetParticleEmitter(std::move(airEffect));

	std::unique_ptr<EmitterSphere> chargeEffect = std::make_unique<EmitterSphere>();
	chargeEffect->Initialize("chargeEffect", 100);
	chargeEffect->SetTranslate({ 0.0f,0.0f,0.0f });
	chargeEffect->SetTexture("gradationLine.dds");
	chargeEffect->SetRing(16, 0.5f, 0.0f);
	particleSystem_->SetParticleEmitter(std::move(chargeEffect));
	//バレットマネージャーの生成
	bulletManager_ = std::make_unique<BulletManager>();

	//ステージマネージャ
	stageManager_ = std::make_unique<StageManager>();
	stageManager_->Initialize("level1", bulletManager_.get(), particleSystem_.get(), true);
	player_ = stageManager_->GetPlayer();

	// UIマネージャの初期化
	uiManager_ = std::make_unique<UIManager>();

	// Play Reticle UI
	std::unique_ptr<ReticleUI> reticleUI = std::make_unique<ReticleUI>();
	reticleUI->Initialize();
	player_->SetReticleUI(reticleUI.get());
	uiManager_->AddUI(std::move(reticleUI));

	// HP UI
	std::unique_ptr<HpUI> hpUI = std::make_unique<HpUI>();
	hpUI->Initialize(player_);
	uiManager_->AddUI(std::move(hpUI));

	// Operation UI
	std::unique_ptr<OperationUI> opUI = std::make_unique<OperationUI>();
	opUI->Initialize(player_);
	uiManager_->AddUI(std::move(opUI));

	ChangeState(std::make_unique<GameSceneStateStart>());
}



void GameScene::Update() {
	BaseScene::Update();

	// UIの更新 (死活監視含む)
	uiManager_->Update();

	// ポーズ遷移 (全ステート共通)
	if (Input::GetInstance()->TriggerKey(DIK_P) || Input::GetInstance()->TriggerControllerButton(XINPUT_GAMEPAD_START)) {
		SceneManager::GetInstance()->ChangeSceneToPause(std::make_unique<PauseScene>());
		return;
	}

	if (state_) {
		state_->Update();
	}
}

void GameScene::Draw(){
	if (state_) {
		state_->Draw();
	}
	// UIの描画
	uiManager_->Draw();
}

void GameScene::DrawGame3D() {
	//Object3dの描画
	//ステージ
	stageManager_->Draw();
	bulletManager_->Draw();

	//当たり判定の表示
	collisionManager_->Draw();

	//ラインの描画
	Line3dManager::GetInstance()->Draw();

	//Particleの描画
	particleSystem_->Draw();
}



} // namespace Engine
