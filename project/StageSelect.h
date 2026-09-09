#pragma once
#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Fade.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "array"
#include <Skydome.h>

///< summary>
/// タイトルシーン
///</summary>
class StageSelect {
public: // メンバ関数
	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};
	/// <summary>
	/// コンストクラタ
	/// </summary>
	StageSelect();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StageSelect();

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

private:
	void FadeInUpdate();
	void MainUpdate();
	void FadeOutUpdate();
	void FadeInDraw();
	void MainDraw();
	void FadeOutDraw();

	void GenerateBlocks();

	static void (StageSelect::*spFuncTableUpdate[])();
	static void (StageSelect::*spFuncTableDraw[])();

private:
	// 終了フラグ
	bool finished_ = false;
	//
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	//
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	//
	// 自キャラ
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;

	//
	std::array<uint32_t,10> textureHandleBlocks_;
	std::unique_ptr<Model> modelBlock_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	std::unique_ptr<Sprite> uiSprite_;

	// スカイドーム
	Model* modelSkydome_ = nullptr;
	Skydome* skydome_ = nullptr;
	
	// SE
	Audio* audio_ = nullptr;
	uint32_t se_ = 0;
	uint32_t seSelectNg_ = 0;
	uint32_t seStageSelect_ = 0;
	
	// フェード
	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;

	static inline int LimitStageNum_ = 10;
	int stageNum_ = 0;

	bool isReturnSelect_ = false;

public:
	// デスフラグのゲッター
	bool IsFinished() const { return finished_; }
	int GetStageNum() {return stageNum_ < LimitStageNum_ ? stageNum_ : LimitStageNum_ - 1;}
	void SetStageNum(int stageNum) { stageNum_ = stageNum < LimitStageNum_ ? stageNum : LimitStageNum_ - 1; }
	bool GetIsReturnSelect() { return isReturnSelect_; }
};
