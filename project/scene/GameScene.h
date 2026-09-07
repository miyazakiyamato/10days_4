#pragma once
#include <vector>
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Player.h"
#include "Skydome.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "Enemy.h"
#include "DeathParticles.h"
#include "Fade.h"
#include "goalBlock.h"
#include "CollisionManager.h"
#include "ClearParticle.h"
#include "ButtonBlock.h"
#include "MovingBlock.h"
#include "BrokenBlock.h"
#include "ObjectColor.h"

// IDと色の対応を定義する関数を追加
namespace {
Vector4 GetColorById(int id) {
	switch (id) {
	case 0:
		return {1.0f, 0.0f, 0.0f, 1.0f}; // 赤
	case 1:
		return {0.0f, 0.0f, 1.0f, 1.0f}; // 青
	case 2:
		return {0.0f, 1.0f, 0.0f, 1.0f}; // 緑
	case 3:
		return {1.0f, 1.0f, 0.0f, 1.0f}; // 黄
	case 4:
		return {1.0f, 0.0f, 1.0f, 1.0f}; // 紫
	case 5:
		return {0.0f, 1.0f, 1.0f, 1.0f}; // 水色
	default:
		return {1.0f, 1.0f, 1.0f, 1.0f}; // 白
	}
}
} // namespace

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
	/// <summary>
	/// ゲームシーン用
	/// </summary>
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	
	void GenerateBlocks();
	//すべての当たり判定
	void CheckAllCollisions();

	void ChangePhase();

private: // メンバ変数
	enum class Phase {
		kFadeIn,//フェードイン
		kPlay,  // ゲームプレイ
		kClear, //クリア演出
		kDeath, // デス演出
		kFadeOut,//フェードアウト
	};
	// ゲームの現在フェーズ
	Phase phase_ = Phase::kFadeIn;
	// 終了フラグ
	bool finished_ = false;
	//
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	//
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	//
	CameraController* cameraController = nullptr;

	// スカイドーム
	Model* modelSkydome_ = nullptr;
	Skydome* skydome_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t se_ = 0;
	uint32_t se1_ = 0;
	uint32_t se2_ = 0;

	//マップチップフィールド
	MapChipField* mapChipField_;

	// ブロック
	uint32_t textureHandleBlock_ = 0;
	Model* modelBlock_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	std::vector<ButtonBlock*> buttons_;
	std::vector<MovingBlock*> movingBlocks_;
	std::vector<BrokenBlock*> brokenBlocks_;
	//デバッグ
	bool isDebagCameraActive = false;
	DebugCamera* debugCamera_ = nullptr;
	//自キャラ
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	Player* player_ = nullptr;
	//敵
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	std::list<Enemy*> enemies_;
	//デスパーティクル
	DeathParticles* deathParticles_ = nullptr;

	ClearParticle* clearParticle_ = nullptr;
	//フェード
	Fade* fade_ = nullptr;

	bool stop = false;

	std::list<GoalBlock*> goalBlocks_;

	std::vector<std::unique_ptr<Sprite>> uiSprite_;

	int stageNum_ = 0;

	std::map<int, ObjectColor*> objectColors_;
public:
	/// //デスフラグのゲッター
	bool IsFinished() const { return finished_; }
	bool GetIsClear() const { return player_->GetIsClear(); }
	int GetStageNum() { return stageNum_;}
	void SetStageNum(int stageNum) { stageNum_ = stageNum; }
};
