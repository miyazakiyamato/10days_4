#include "GameSceneStateStart.h"
#include "GameScene.h"
#include "TimeManager.h"
#include "GameStartUI.h"
#include "CameraManager.h"
#include "GameSceneStateBattle.h" 

namespace Engine {

GameSceneStateStart::~GameSceneStateStart() {
	if (gameStartUI_) {
		gameStartUI_->SetIsDead(true);
	}
}

void GameSceneStateStart::Initialize(GameScene* gameScene) {
	BaseSceneState<GameScene>::Initialize(gameScene);

	// Game Start UI
	std::unique_ptr<GameStartUI> newUI = std::make_unique<GameStartUI>();
	newUI->Initialize(duration_);
	gameStartUI_ = newUI.get();
	scene_->GetUIManager()->AddUI(std::move(newUI));

	TimeManager::GetInstance()->SetDeltaTimeSpeedStart(0.0f, duration_ * 3.0f);
}

void GameSceneStateStart::Update() {
	auto stageManager = scene_->GetStageManager();
	auto bulletManager = scene_->GetBulletManager();
	auto particleSystem = scene_->GetParticleSystem();
	auto collisionManager = scene_->GetCollisionManager();
	auto player = stageManager->GetPlayer();

	//ステージ
	stageManager->Update();

	bulletManager->Update();

	//当たり判定
	//衝突マネージャのリストクリアする
	collisionManager->Reset();
	//全てのコライダーを衝突マネージャのリストに登録する
	stageManager->SetStageCollisions(collisionManager);
	bulletManager->AddCollider(collisionManager);
	//リスト内の総当たり判定
	collisionManager->CheckAllCollisions();

	collisionManager->UpdateWorldTransform();

	particleSystem->Update();
	
	if (gameStartUI_->ShouldClose()) {
		// Battleへ遷移
		scene_->ChangeState(std::make_unique<GameSceneStateBattle>());
	}
}

void GameSceneStateStart::Draw() {
	auto stageManager = scene_->GetStageManager();
	auto bulletManager = scene_->GetBulletManager();
	auto particleSystem = scene_->GetParticleSystem();
	auto collisionManager = scene_->GetCollisionManager();

	//Object3dの描画
	//ステージ
	stageManager->Draw();
	bulletManager->Draw();

	//当たり判定の表示
	collisionManager->Draw();

	//ラインの描画
	//Line3dManager::GetInstance()->Draw();

	//Particleの描画
	particleSystem->Draw();
}

} // namespace Engine
