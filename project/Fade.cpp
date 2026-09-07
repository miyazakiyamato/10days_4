#define NOMINMAX
#include "Fade.h"
#include "TextureManager.h"
#include "cmath"
#include <algorithm>

Fade::~Fade() { delete sprite_; }

void Fade::Initialize() {
	textureHandle_ = TextureManager::Load("./Resources/white1x1.png");
	sprite_ = Sprite::Create(textureHandle_, {});
	sprite_->SetSize({1280,720});
	sprite_->SetColor({0,0,0,1.0f});
}

void Fade::Update() {
	switch (status_) {
	case Fade::Status::None:
		break;
	case Fade::Status::FadeIn:
		// 1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;
		// フェード継続時間に達したら打ち止め
		if (std::min(1.0f, counter_ / duration_) == 1.0f) {
			counter_ = duration_;
			Stop();
		}
		// 0.0fから1.0fの間で経過時間がフェード継続時間に近づくほどアルファ値を小さくする
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(1.0f - counter_ / duration_, 0.0f, 1.0f)));
		break;
	case Fade::Status::FadeOut:
		//1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;
		//フェード継続時間に達したら打ち止め
		if (std::min(1.0f,counter_ / duration_) == 1.0f) {
			counter_ = duration_;
		}
		//0.0fから1.0fの間で経過時間がフェード継続時間に近づくほどアルファ値を大きくする
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)));
		break;
	default:
		break;
	}
}

void Fade::Draw(ID3D12GraphicsCommandList* commandList) {
	if (status_ == Status::None) {
		return;
	}
	#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void Fade::Start(Status status, float duration) {
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

void Fade::Stop() { status_ = Status::None; }

bool Fade::IsFinished() const {
	switch (status_) {
	case Fade::Status::None:
		break;
	case Fade::Status::FadeIn:
		break;
	case Fade::Status::FadeOut:
		return counter_ >= duration_ ? true : false;
		break;
	default:
		break;
	}
	return true;
}
