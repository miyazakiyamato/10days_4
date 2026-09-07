#pragma once
#include "MathFunctions.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Input.h>
#include <array>
#include <numbers>

class ClearParticle {
public:
	~ClearParticle();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="model"></param>
	/// <param name="viewprojection"></param>
	/// <param name="position"></param>
	void Initialize(ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	// パーティクルの個数とそのワールドトランスフォーム
	static inline const uint32_t kNumParticles = 30;
	std::array<WorldTransform, kNumParticles> worldTransforms_;

	Model* modelClearParticle_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
	ObjectColor objectColor_;
	Vector4 color_;

	static inline const float kDuration = 6.0f;
	static inline const float kSpeed = 0.2f;
	std::array<Vector3, kNumParticles> initialVelocities_; // パーティクルごとの初期速度を保持
	// static inline const float kAngleUnit = std::numbers::pi_v<float> / float(kNumParticles);

	bool isFinished_ = false;
	float counter_ = -2.0f;

	public:
	bool IsFinished() { return isFinished_; }
};