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

	// シーンの終了フラグ
	bool IsFinished() const { return finished_; };

private:
	// [道る記憶]の変数
	std::unique_ptr<Model> modelTitle_;
	WorldTransform title1WorldTransform_;
	ViewProjection viewProjection_;

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

	// シーン切り替え待機
	float waitTime_ = 0.0f;
	const float kWaitTime = 1.0f;
	bool isWait_ = false;
};