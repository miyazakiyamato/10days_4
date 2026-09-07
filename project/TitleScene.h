#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MyMtMatrix.h"
#include "Fade.h"
#include <Skydome.h>

///<summary>
/// タイトルシーン
///</summary>
class TitleScene {
public: // メンバ関数
	enum class Phase {
		kFadeIn,//フェードイン
		kMain,//メイン部
		kFadeOut,//フェードアウト
	};
	/// <summary>
	/// コンストクラタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

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
	void PlayerInitialize();

	// シーンの終了フラグ
	bool IsFinished() const { return finished_; };

private:
	// [ぴょ]の変数
	std::unique_ptr<Model> modelTitle_;
	WorldTransform title1WorldTransform_;
	ViewProjection viewProjection_;

	// [引]の変数
	std::unique_ptr<Model> model2Title_;
	WorldTransform title2WorldTransform_;
	ViewProjection title2ViewProjection_;

	// SpaceKey の変数
	std::unique_ptr<Model> model3Title_;
	WorldTransform title3WorldTransform_;
	ViewProjection title3ViewProjection_;

	// スカイドーム
	Model* modelSkydome_ = nullptr;
	Skydome* skydome_ = nullptr;

	// 基盤部分
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t se_ = 0;
	// シーンの終了フラグ
	bool finished_ = false;

	// アニメーション用の変数
	float animationTime_ = 0.0f;     // 経過時間
	float startPosY_ = 0.0f;         // 初期位置Y
	float endPosY_ = 0.0f;           // 終了位置Y
	float animationDuration_ = 2.0f; // アニメーションの長さ（秒）
	bool isAnimating_ = false;       // アニメーションが有効かどうか

	// Playerの変数
	WorldTransform playerWorldTransform_;
	ViewProjection playerViewProjection_;
	std::unique_ptr<Model> playerModel_;
	uint32_t playerTextureHandle_ = 0;

	// PlayerのX軸移動アニメーション用変数
	float playerStartX_ = 0.0f;            // Playerの初期X位置
	float playerEndX_ = -10.0f;            // Playerの目的X位置
	float playerAnimationTime_ = 0.0f;     // アニメーション経過時間
	float playerAnimationDuration_ = 2.0f; // アニメーションにかかる時間（秒）
	bool isPlayerAnimating_ = false;       // Playerアニメーションの状態

	// title1のスケールアニメーション用変数
	float title1ScaleYTime_ = 0.0f;     // title1のYスケール変更の経過時間
	float title1ScaleYDuration_ = 1.5f; // スケール変更にかける時間
	bool isScalingTitle1_ = false;      // スケールアニメーションが開始されたかどうか

	// Playerのジャンプアニメーション用変数
	float playerJumpTime_ = 0.0f;     // ジャンプアニメーションの経過時間
	float playerJumpDuration_ = 0.5f; // ジャンプにかける時間
	float playerStartY_ = 0.0f;       // Playerのジャンプ前のY位置
	float playerJumpHeight_ = 10.0f;  // ジャンプの高さ
	bool isPlayerJumping_ = false;    // Playerがジャンプ中かどうか

	// ジャンプ後のPlayer移動用変数
	float playerNewEndX_ = 20.0f;          // Playerの新しい目的X位置（ジャンプ後）
	bool isPlayerMovingAfterJump_ = false; // ジャンプ後の移動フラグ

	// Y軸移動用変数
	bool isPlayerFalling_ = false;    // PlayerがY軸で落ちているか
	float playerFallDuration_ = 1.0f; // Y軸の移動にかかる時間
	float playerFallTime_ = 0.0f;     // Y軸の移動経過時間// フェード
	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;
	void UpdateTitle2Drop();
	void UpdatePlayerMove();
	void UpdatePlayerMoveAfterJump(); // ジャンプ後のPlayerの移動
	void UpdateTitle1Scale();         // Title1のスケール変更
	void UpdatePlayerJump();          // Playerのジャンプ
	void UpdatePlayerFall();          // PlayerのY軸移動

	void FadeInUpdate();
	void MainUpdate();
	void FadeOutUpdate();
	void FadeInDraw();
	void MainDraw();
	void FadeOutDraw();

	static void (TitleScene::*spFuncTableUpdate[])();
	static void (TitleScene::*spFuncTableDraw[])();
};