#include "GameSceneStateBattle.h"
#include "GameScene.h"
#include "StageManager.h"
#include "BulletManager.h"
#include "ParticleSystem.h"
#include "CollisionManager.h"
#include "Line3D.h"
#include "BaseUI.h"
#include "SceneManager.h"
#include "GameSceneStateWin.h"
#include "GameSceneStateDeath.h"
#include "PlayerStateLeave.h"
#include "TextureManager.h"

#include "PauseScene.h"
#include "Input.h"

namespace Engine {

void GameSceneStateBattle::Initialize(GameScene* gameScene) {
	BaseSceneState<GameScene>::Initialize(gameScene);
}

void GameSceneStateBattle::Update() {
	auto stageManager = scene_->GetStageManager();
	auto bulletManager = scene_->GetBulletManager();
	auto particleSystem = scene_->GetParticleSystem();
	auto collisionManager = scene_->GetCollisionManager();
	auto player = stageManager->GetPlayer();
	auto sceneManager = SceneManager::GetInstance();


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

	//クリアチェック
	if (!player->GetIsAlive()) {
		// GAMEOVER遷移 -> StateDeath
		scene_->ChangeState(std::make_unique<GameSceneStateDeath>());
	}
	else if (stageManager->GetRailCamera()->GetIsFinished()) {
		// CLEAR遷移 -> StateWin
		player->ChangeState(std::make_unique<PlayerStateLeave>(player));
		scene_->ChangeState(std::make_unique<GameSceneStateWin>());
	}
}

void GameSceneStateBattle::Draw() {
	scene_->DrawGame3D();
}

} // namespace Engine
