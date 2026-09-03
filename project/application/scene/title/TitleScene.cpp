#include "TitleScene.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "CameraManager.h"
#include "PostEffectManager.h"
#include "GlobalVariables.h"
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI
#include "LightManager.h"
#include "EmitterSphere.h"

#include "TitleSceneStateMain.h"

namespace Engine {

void TitleScene::ChangeState(std::unique_ptr<BaseSceneState<TitleScene>> newState) {
	state_ = std::move(newState);
	state_->Initialize(this);
}

void TitleScene::Initialize(){
	BaseScene::Initialize();

	TextureManager::GetInstance()->LoadTexture("AStart.dds");
	TextureManager::GetInstance()->LoadTexture("circle2.dds");
	TextureManager::GetInstance()->LoadTexture("gradationLine.dds");
	TextureManager::GetInstance()->LoadTexture("flash.dds");

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

	//ステージマネージャ
	stageManager_ = std::make_unique<StageManager>();
	stageManager_->Initialize("level1", nullptr, particleSystem_.get(), false, true);

	camera_ = stageManager_->GetRailCamera()->GetCamera();

	ChangeState(std::make_unique<TitleSceneStateMain>());
}



void TitleScene::Update(){

	BaseScene::Update();

	if (state_) {
		state_->Update();
	}

	stageManager_->Update();
}

void TitleScene::Draw(){
	// 背景(ステージ)
	stageManager_->Draw();
	// パーティクル描画
	particleSystem_->Draw();

	if (state_) {
		state_->Draw();
	}
}

} // namespace Engine
