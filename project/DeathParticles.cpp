#include "DeathParticles.h"
#include <cassert>
#include <algorithm>

DeathParticles::~DeathParticles() {}

void DeathParticles::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	//ワールド変数の初期化
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.scale_ = {0.5f, 0.5f, 0.5f};
		worldTransform.translation_ = position;
		worldTransform.UpdateMatrix();
	}
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void DeathParticles::Update() {
	//終了なら何もしない
	if (isFinished_) {
		return;
	}
	for (uint32_t i = 0; i < kNumParticles; ++i) {
		//基本となる速度ベクトル
		Vector3 velocity = {kSpeed, 0, 0};
		//回転角を計算する
		float angle = kAngleUnit * i;
		//z軸まわり回転行列
		Matrix4x4 matrixRotation = MyMtMatrix::MakeRotateZMatrix(angle);
		//基本ベクトルを回転させて速度ベクトルを得る
		velocity = MyMtMatrix::Transform(velocity, matrixRotation);
		//移動処理
		worldTransforms_[i].translation_ = MyMtVector3::Add(worldTransforms_[i].translation_, velocity);
	}
	//ワールド変換の更新
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
	//カウンターを1フレーム分の秒進める
	counter_ += 1.0f / 60.0f;
	//存続時間の上限に達したら
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		//終了扱いにする
		isFinished_ = true;
	}
	color_.w = 1.0f - counter_ / kDuration;
	color_.w = std::clamp(color_.w, 0.0f, 1.0f);
	//色変更オブジェクトに色の変数を設定する
	objectColor_.SetColor(color_);
	//色変更オブジェクトVRAMに転送
	objectColor_.TransferMatrix();
}

void DeathParticles::Draw() {
	// 終了なら何もしない
	if (isFinished_) {
		return;
	}
	//モデルの描画
	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *viewProjection_, textureHandle_,&objectColor_);
	}
}
