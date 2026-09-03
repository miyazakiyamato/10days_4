#include "GameSceneStateWin.h"
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

GameSceneStateWin::~GameSceneStateWin() {
	if (playerWinUI_) {
		playerWinUI_->SetIsDead(true);
	}
}

void GameSceneStateWin::Initialize(GameScene* gameScene) {
	BaseSceneState<GameScene>::Initialize(gameScene);

	// Player Win UI
	std::unique_ptr<PlayerWinUI> newUI = std::make_unique<PlayerWinUI>();
	newUI->Initialize();
	playerWinUI_ = newUI.get();
	scene_->GetUIManager()->AddUI(std::move(newUI));

	auto particleSystem = scene_->GetParticleSystem();
	auto player = scene_->GetStageManager()->GetPlayer();
	float scale = Easing::EaseOutBounce(counter_ / duration_, 1.0f, 0.0f);

	// エフェクト処理
	if (particleSystem->FindEmitter("emitterHit")) {
		static_cast<EmitterSphere*>(particleSystem->FindEmitter("emitterHit"))->SetTranslate(player->GetObject3d()->GetCenterPosition());
		if (scale >= 1.0f) {
			particleSystem->Emit("emitterHit");
		}
	}
	// カメラ追従停止
	if (player) {
		player->StopCameraFollow();
	}
}

void GameSceneStateWin::Update() {
	auto stageManager = scene_->GetStageManager();
	auto bulletManager = scene_->GetBulletManager();
	auto particleSystem = scene_->GetParticleSystem();
	auto player = scene_->GetStageManager()->GetPlayer();
	//勝った時の処理
	counter_ += TimeManager::GetInstance()->kFlamTime_;

	if (counter_ < duration_) {

	}
	else {
		// 次のシーンへ
		if (!isTransitioning_) {
			isTransitioning_ = true;
			SceneManager::GetInstance()->ChangeScene("CLEAR");
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

void GameSceneStateWin::Draw() {
	// 背景描画
	auto stageManager = scene_->GetStageManager();
	auto bulletManager = scene_->GetBulletManager();
	auto particleSystem = scene_->GetParticleSystem();

	stageManager->Draw();
	bulletManager->Draw();
	particleSystem->Draw();
}

} // namespace Engine
