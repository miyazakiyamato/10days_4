#include "GameSceneStateDeath.h"
#include "GameScene.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "StageManager.h"
#include "BulletManager.h"
#include "CollisionManager.h"
#include "ParticleSystem.h"
#include "Line3D.h"
#include "Easing.h"
#include "EmitterSphere.h"

namespace Engine {

GameSceneStateDeath::~GameSceneStateDeath() {
	if (playerDeathUI_) {
		playerDeathUI_->SetIsDead(true);
	}
}

void GameSceneStateDeath::Initialize(GameScene* gameScene) {
	BaseSceneState<GameScene>::Initialize(gameScene);

	// Player Death UI
	std::unique_ptr<PlayerDeathUI> newUI = std::make_unique<PlayerDeathUI>();
	newUI->Initialize();
	playerDeathUI_ = newUI.get();
	scene_->GetUIManager()->AddUI(std::move(newUI));
}

void GameSceneStateDeath::Update() {
	auto stageManager = scene_->GetStageManager();
	auto bulletManager = scene_->GetBulletManager();
	auto particleSystem = scene_->GetParticleSystem();
	auto player = scene_->GetStageManager()->GetPlayer();

	//死んだ時の処理
	counter_ += TimeManager::GetInstance()->kFlamTime_;

	if (counter_ < duration_) {
		float scale = Easing::EaseOutBounce(counter_ / duration_, 1.0f, 0.0f);
		player->GetObject3d()->SetScale({ scale,scale ,scale });
		
		if (particleSystem->FindEmitter("hitEffect")) {
			static_cast<EmitterSphere*>(particleSystem->FindEmitter("hitEffect"))->SetTranslate(player->GetObject3d()->GetCenterPosition());
			if (int(scale * 100.0f) % 5 < 1 && scale > 0.1f) {
				particleSystem->Emit("hitEffect");
			}
		}
	}
	else {
		// 次のシーンへ
		if (!isTransitioning_) {
			isTransitioning_ = true;
			SceneManager::GetInstance()->ChangeScene("GAMEOVER");
			SceneManager::GetInstance()->ChangeTransition("FADE");
		}
	}
	//ステージ
	stageManager->Update();
	//弾管理者
	bulletManager->Update();
	//パーティクルシステム
	particleSystem->Update();
}

void GameSceneStateDeath::Draw() {
	// 背景描画
	auto stageManager = scene_->GetStageManager();
	auto bulletManager = scene_->GetBulletManager();
	auto particleSystem = scene_->GetParticleSystem();

	stageManager->Draw();
	bulletManager->Draw();
	particleSystem->Draw();
}

} // namespace Engine
