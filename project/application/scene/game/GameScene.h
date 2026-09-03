#pragma once
#include <vector>
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "ParticleSystem.h"
#include "CollisionManager.h"
#include "AccelerationField.h"
#include "StageManager.h"
#include "Player.h"
#include "BulletManager.h"
#include "BaseEnemy.h"
#include "RailCamera.h"
#include "BaseUI.h"
#include "UIManager.h"
#include "BaseSceneState.h"

namespace Engine {

class GameSceneStatePause;
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene {
public:
		// --- メンバ関数 ---
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	
	/// <summary>
	/// ステート変更
	/// </summary>
	void ChangeState(std::unique_ptr<BaseSceneState<GameScene>> newState);

	/// <summary>
	/// ゲーム本編の3D描画
	/// </summary>
	void DrawGame3D();
private:
		// --- メンバ変数 ---
	//衝突マネージャ
	std::unique_ptr<CollisionManager> collisionManager_;

	//ステージマネージャ
	std::unique_ptr<StageManager> stageManager_;
	//プレイヤー
	Player* player_ = nullptr;
	
	//UIマネージャ
	std::unique_ptr<UIManager> uiManager_;

	//パーティクルシステム
	std::unique_ptr<ParticleSystem> particleSystem_ = nullptr;
	//バレットマネージャー
	std::unique_ptr<BulletManager> bulletManager_ = nullptr;

	//ステート
	std::unique_ptr<BaseSceneState<GameScene>> state_;
public:
	ParticleSystem* GetParticleSystem() { return particleSystem_.get(); }
	StageManager* GetStageManager() { return stageManager_.get(); }
	BulletManager* GetBulletManager() { return bulletManager_.get(); }
	CollisionManager* GetCollisionManager() { return collisionManager_.get(); }
	UIManager* GetUIManager() { return uiManager_.get(); }
};


} // namespace Engine
