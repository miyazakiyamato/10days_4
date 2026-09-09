#include "TitleScene.h"
#include "MathFunctions.h"

TitleScene::TitleScene() {}
float EaseOutQuad(float t) { return -t * (t - 2); }

TitleScene::~TitleScene() {}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	se_ = audio_->LoadWave("se_title_button_start.mp3");

	// [道る記憶]の初期化
	modelTitle_ = std::unique_ptr<Model>(Model::CreateFromOBJ("TitleLogo", true));
	title1WorldTransform_.Initialize();
	title1WorldTransform_.translation_ = {0.0f, 0.0f, 1.0f};
	title1WorldTransform_.scale_ = {10.0f, 10.0f, 10.0f};
	viewProjection_.Initialize();

	// [Space]の初期化
	model3Title_ = std::unique_ptr<Model>(Model::CreateFromOBJ("TitleSpace", true));
	title3WorldTransform_.Initialize();
	title3WorldTransform_.translation_ = {0.0f, -10.0f, 1.0f};
	title3WorldTransform_.scale_ = {5.0f, 5.0f, 1.0f};
	title3ViewProjection_.Initialize();
	//

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// スカイドーム
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	// 各ワールド行列を更新
	title1WorldTransform_.UpdateMatrix();
	title3WorldTransform_.UpdateMatrix();
	title3WorldTransform_.scale_ = {7.0f, 7.0f, 1.0f};
	
	// 待機時間の初期化
	waitTime_ = 0.0f;
	isWait_ = false;
}

void TitleScene::Update() {
	if (isWait_) {
		waitTime_ -= 1.0f / 60.0f; // 1フレーム分の時間を減算
		if (waitTime_ <= 0.0f) {
			finished_ = true; // シーン終了フラグを立てる
			return;
		}
	}

	// スカイドームの更新
	skydome_->Update();

	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->PlayWave(se_, false, 2.0f);
		isWait_ = true;
		waitTime_ = kWaitTime;
	}
}

void TitleScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	Model::PreDraw(commandList);
	skydome_->Draw();
	modelTitle_->Draw(title1WorldTransform_, viewProjection_);
	model3Title_->Draw(title3WorldTransform_, title3ViewProjection_);
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
#pragma endregion
}