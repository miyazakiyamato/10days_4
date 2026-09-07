#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMtMatrix.h"
#include <array>
#include <numbers>

class DeathParticles {
public:
	~DeathParticles();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	ViewProjection* viewProjection_ = nullptr;
	//
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;

	std::array<WorldTransform, kNumParticles> worldTransforms_;

	//存続時間
	static inline const float kDuration = 2.0f;
	//移動の速さ
	static inline const float kSpeed = 0.1f;
	//分割した1個分の角度
	static inline const float kAngleUnit = 2.0f * std::numbers::pi_v<float> / kNumParticles;
	//終了フラグ
	bool isFinished_ = false;
	//経過時間カウント
	float counter_ = 0.0f;

	//色変更オブジェクト
	ObjectColor objectColor_;
	//色の数値
	Vector4 color_;

public:
	bool IsFinished() { return isFinished_; }
};
