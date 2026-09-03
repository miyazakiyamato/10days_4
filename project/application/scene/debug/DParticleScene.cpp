#include "DParticleScene.h"
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI

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

namespace Engine {

void DParticleScene::Initialize(){
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

	ModelManager::GetInstance()->LoadModel("terrain/terrain.obj");

	TextureManager::GetInstance()->LoadTexture("circle2.dds");
	TextureManager::GetInstance()->LoadTexture("gradationLine.dds");
	TextureManager::GetInstance()->LoadTexture("rostock_laage_airport_4k.dds");

	//衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();

	//skybox
	std::unique_ptr<Object3d> object3d = std::make_unique<Object3d>();
	object3d->Initialize();
	object3d->SetModel("terrain/terrain.obj");
	object3ds_.push_back(std::move(object3d));
	
	isAccelerationField = false;
	accelerationField_ = std::make_unique<AccelerationField>();

	particleSystem_ = std::make_unique<ParticleSystem>();
	particleSystem_->Initialize();
	std::unique_ptr<EmitterSphere> emitterSphere = std::make_unique<EmitterSphere>();
	emitterSphere->Initialize("emitterSphere",10000);
	emitterSphere->SetTexture("gradationLine.dds");
	emitterSphere->SetRing(16, 0.5f, 0.0f);
	particleSystem_->SetParticleEmitter(std::move(emitterSphere));

	std::unique_ptr<EmitterSphere> emitterHit = std::make_unique<EmitterSphere>();
	emitterHit->Initialize("emitterHit",100);
	emitterHit->SetTranslate({ 1.0f,1.0f,0.0f });
	emitterHit->SetTexture("circle2.dds");
	particleSystem_->SetParticleEmitter(std::move(emitterHit));

	std::unique_ptr<EmitterSphere> hitEffect = std::make_unique<EmitterSphere>();
	hitEffect->Initialize("hitEffect", 100);
	hitEffect->SetTranslate({ 1.0f,1.0f,0.0f });
	hitEffect->SetTexture("circle2.dds");
	particleSystem_->SetParticleEmitter(std::move(hitEffect));

	std::unique_ptr<EmitterSphere> airEffect = std::make_unique<EmitterSphere>();
	airEffect->Initialize("airEffect", 100);
	airEffect->SetTranslate({ 0.0f,0.0f,0.0f });
	airEffect->SetTexture("circle2.dds");
	particleSystem_->SetParticleEmitter(std::move(airEffect));
}

void DParticleScene::Update(){
	BaseScene::Update();

	CameraManager::GetInstance()->GetCamera()->Update();
	PostEffectManager::GetInstance()->Update();
#ifdef _DEBUG
	// デバッグ用にワールドトランスフォームの更新
	collisionManager_->UpdateWorldTransform();
#endif //_DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
		//AudioManager::GetInstance()->PlayWave("maou_se_system48.wav");
		//AudioManager::GetInstance()->PlayMP3("audiostock_1420737.mp3");
		//ParticleManager::GetInstance()->Emit("uvChecker", { 0,0,0 }, 10);
		particleSystem_->Emit("emitterHit");
	}
	
	for (std::unique_ptr<Object3d>& object3d : object3ds_) {
		object3d->Update();
	}

	/*if (isAccelerationField) {
		for (std::pair<const std::string, std::unique_ptr<ParticleManager::ParticleGroup>>& pair : ParticleManager::GetInstance()->GetParticleGroups()) {
			ParticleManager::ParticleGroup& group = *pair.second;
			int index = 0;
			for (std::list<ParticleManager::Particle>::iterator it = group.particles.begin(); it != group.particles.end();) {
				ParticleManager::Particle& particle = *it;

				if (Collision::IsCollision(accelerationField_->GetAABB(), particle.transform.translate)) {
					//particle.velocity += accelerationField_->GetAcceleration() * TimeManager::GetInstance()->deltaTime_;

				}

				++it;
				++index;
			}
		}
	}*/
	
	particleSystem_->Update();

	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Update();
	}
}

void DParticleScene::Draw(){
	//Object3dの描画
	for (std::unique_ptr<Object3d>& object3d : object3ds_) {
		object3d->Draw();
	}
	//object3ds_[0]->Draw();
	//当たり判定の表示
	collisionManager_->Draw();
	
	//ラインの描画
	//Line3dManager::GetInstance()->DrawLine(object3ds_[0]->GetCenterPosition(), object3ds_[1]->GetCenterPosition(),{1.0f,0.0f,0.0f,1.0f});
	//Line3dManager::GetInstance()->DrawLine(object3ds_[1]->GetCenterPosition(), object3ds_[2]->GetCenterPosition(),{1.0f,0.0f,0.0f,1.0f});
	//Line3dManager::GetInstance()->DrawSphere({ object3ds_[0]->GetCenterPosition(),1.0f }, { 1.0f,0.0f,0.0f,1.0f });
	/*Line3dManager::GetInstance()->DrawSphere({ {},1.0f}, {1.0f,0.0f,0.0f,1.0f},10);
	Line3dManager::GetInstance()->DrawGrid({50.0f,3.0f});*/
	Line3dManager::GetInstance()->Draw();

	//Particleの描画
	particleSystem_->Draw();

	//Spriteの描画
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}

void DParticleScene::CheckAllCollisions(){
	//衝突マネージャのリストクリアする
	collisionManager_->Reset();
	//全てのコライダーを衝突マネージャのリストに登録する

	/*for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		collisionManager_->AddCollider(enemy.get());
	}*/
	//リスト内の総当たり判定
	collisionManager_->CheckAllCollisions();
}

} // namespace Engine
