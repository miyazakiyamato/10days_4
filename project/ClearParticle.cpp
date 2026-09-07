#include "ClearParticle.h"
#include <algorithm>
#include <cstdlib> // rand() 関数のため
#include <ctime>   // ランダムシード設定のため
#include "MyMtVector3.h"

ClearParticle::~ClearParticle() { delete modelClearParticle_; }

void ClearParticle::Initialize(ViewProjection* viewProjection, const Vector3& position) {
	// ランダムシードを設定
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// ワールド変換の初期化および初期速度の設定
	for (uint32_t i = 0; i < kNumParticles; ++i) {
		worldTransforms_[i].Initialize();
		worldTransforms_[i].translation_ = position;

		// パーティクルごとの初期速度を一度だけ計算して保持する
		float randomSpeedX = -5.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 10.0f));
		float randomSpeedY = -5.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 10.0f));
		initialVelocities_[i] = {kSpeed * randomSpeedX, kSpeed * randomSpeedY, 0.0f};
	}

	modelClearParticle_ = Model::CreateFromOBJ("player", true);
	viewProjection_ = viewProjection;
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void ClearParticle::Update() {
	if (!isFinished_) {
		Vector3 gravity = {0, -0.05f, 0}; // 強い重力設定

		for (uint32_t i = 0; i < kNumParticles; ++i) {
			// 初期速度に基づいて移動させ、重力を加算
			Vector3 velocity_ = MyMtVector3::Add(initialVelocities_[i] , gravity);

			// パーティクルを移動させる
			worldTransforms_[i].translation_ = MyMtVector3::Add(worldTransforms_[i].translation_ ,velocity_);
		}

		// 一定時間でパーティクルを消す
		counter_ += 1.0f / 25.0f;
		if (counter_ >= kDuration) {
			counter_ = kDuration;
			isFinished_ = true;
		}

		// フェードアウト処理
		color_.w = std::clamp(1.0f - counter_, 0.0f, 1.0f);
		objectColor_.SetColor(color_);
		objectColor_.TransferMatrix();
	}

	// ワールド変換の更新
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
}

void ClearParticle::Draw() {
	if (!isFinished_) {
		for (auto& worldTransform : worldTransforms_) {
			modelClearParticle_->Draw(worldTransform, *viewProjection_, &objectColor_);
		}
	}
}
