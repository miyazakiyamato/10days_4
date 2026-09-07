#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"

/// <summary>
/// フェード
/// </summary>
class Fade {
public: // メンバ関数
	// フェードの状態
	enum class Status {
		None,    // フェードなし
		FadeIn,  // フェードイン
		FadeOut, // フェードアウト
	};
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Fade(){};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Fade();

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
	void Draw(ID3D12GraphicsCommandList* commandList);

	//フェード開始
	void Start(Status status,float duration); 
	void Stop();
	bool IsFinished() const;

private:
	Status status_ = Status::None;

	//フェードの持続時間
	float duration_ = 0.0f;
	//経過時間カウンター
	float counter_ = 0.0f;

	uint32_t textureHandle_ = 0;
	Sprite* sprite_ = nullptr;

public:
	Status GetStatus() { return status_; }
};
