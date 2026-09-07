#include "TitleScene.h"
#include "MathFunctions.h"

TitleScene::TitleScene() {}
float EaseOutQuad(float t) { return -t * (t - 2); }

TitleScene::~TitleScene() {
	delete fade_;
}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	se_ = audio_->LoadWave("maou_se_onepoint25.wav");

	// [ぴょ]の初期化
	modelTitle_ = std::unique_ptr<Model>(Model::CreateFromOBJ("Title_1", true));
	title1WorldTransform_.Initialize();
	title1WorldTransform_.translation_ = {-8.0f, 0.0f, 1.0f};
	title1WorldTransform_.scale_ = {10.0f, 0.0f, 1.0f}; // Yスケールは0.0fからスタート
	viewProjection_.Initialize();

	// [引]の初期化
	model2Title_ = std::unique_ptr<Model>(Model::CreateFromOBJ("Title_2", true));
	title2WorldTransform_.Initialize();
	title2WorldTransform_.translation_ = {10.0f, 60.0f, 1.0f};
	title2WorldTransform_.scale_ = {10.0f, 10.0f, 1.0f};
	title2ViewProjection_.Initialize();

	// [引]の初期化
	model3Title_ = std::unique_ptr<Model>(Model::CreateFromOBJ("TitleSpace", true));
	title3WorldTransform_.Initialize();
	title3WorldTransform_.translation_ = {0.0f, -10.0f, 1.0f};
	title3WorldTransform_.scale_ = {0.0f, 0.0f, 1.0f};
	title3ViewProjection_.Initialize();
	//

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// スカイドーム
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
	animationDuration_ = 2.0f;                         // アニメーションの長さ（秒）
	isAnimating_ = true;

	// Title1のスケールアニメーション用の初期化
	title1ScaleYTime_ = 0.0f;
	title1ScaleYDuration_ = 1.5f; // 徐々に1.5秒でスケールを10.0fにする
	isScalingTitle1_ = false;

	// Playerのジャンプアニメーション用の初期化
	playerJumpTime_ = 0.0f;
	playerJumpDuration_ = 0.5f; // ジャンプに0.5秒かける
	playerStartY_ = 0.0f;
	playerJumpHeight_ = 10.0f;
	isPlayerJumping_ = false;

	// 新しい移動用の初期化
	playerNewEndX_ = 20.0f;           // ジャンプ後に移動する新しいX位置
	isPlayerMovingAfterJump_ = false; // ジャンプ後の移動はまだ開始しない

	// Y軸移動用の初期化
	isPlayerFalling_ = false; // PlayerはまだY軸で落下しない
	playerFallTime_ = 0.0f;   // Y軸移動経過時間

	PlayerInitialize();
	// 各ワールド行列を更新
	title1WorldTransform_.UpdateMatrix();
	title2WorldTransform_.UpdateMatrix();
	title3WorldTransform_.UpdateMatrix();
	playerWorldTransform_.UpdateMatrix();
	title3WorldTransform_.scale_ = {7.0f, 7.0f, 1.0f};
	
}


void TitleScene::Update() {
	// スカイドームの更新
	skydome_->Update();
	(this->*spFuncTableUpdate[static_cast<size_t>(phase_)])();
}

void TitleScene::UpdateTitle2Drop() {
	animationTime_ += 1.0f / 60.0f; // フレームレートが60fpsの場合

	if (animationTime_ < animationDuration_) {
		float normalizedTime = animationTime_ / animationDuration_;
		float bounceValue = easeInOutElastic(normalizedTime);
		title2WorldTransform_.translation_.y = startPosY_ + (endPosY_ - startPosY_) * bounceValue;
	} else {
		title2WorldTransform_.translation_.y = endPosY_; // アニメーション終了
		isAnimating_ = false;                            // アニメーション終了フラグをリセット
		isPlayerAnimating_ = true;                       // 次はPlayerの移動を開始
	}
}

void TitleScene::UpdatePlayerMove() {
	playerAnimationTime_ += 1.0f / 50.0f;

	if (playerAnimationTime_ < playerAnimationDuration_) {
		float normalizedTime = playerAnimationTime_ / playerAnimationDuration_;
		playerWorldTransform_.translation_.x = playerStartX_ + (playerEndX_ - playerStartX_) * normalizedTime;
	} else {
		playerWorldTransform_.translation_.x = playerEndX_; // 移動完了
		isPlayerAnimating_ = false;

		isScalingTitle1_ = true; // Title1のスケール変更開始
		isPlayerJumping_ = true; // Playerのジャンプ開始
	}
}

void TitleScene::UpdatePlayerMoveAfterJump() {
	playerAnimationTime_ += 1.0f / 50.0f;

	if (playerAnimationTime_ < playerAnimationDuration_) {
		float normalizedTime = playerAnimationTime_ / playerAnimationDuration_;
		playerWorldTransform_.translation_.x = playerEndX_ + (playerNewEndX_ - playerEndX_) * normalizedTime;
	} else {
		playerWorldTransform_.translation_.x = playerNewEndX_; // 移動完了
		isPlayerMovingAfterJump_ = false;                      // 移動が終了したらフラグをリセット
		isPlayerFalling_ = true;                               // Y軸移動を開始
		playerFallTime_ = 0.0f;                                // Y軸移動経過時間リセット
	}
}

void TitleScene::UpdateTitle1Scale() {
	title1ScaleYTime_ += 1.0f / 20.0f;

	if (title1ScaleYTime_ < title1ScaleYDuration_) {
		float normalizedScaleTime = title1ScaleYTime_ / title1ScaleYDuration_;
		title1WorldTransform_.scale_.y = 0.0f + (10.0f - 0.0f) * normalizedScaleTime;
	} else {
		title1WorldTransform_.scale_.y = 10.0f; // スケール変更終了
		isScalingTitle1_ = false;
	}
}

void TitleScene::UpdatePlayerJump() {
	playerJumpTime_ += 1.0f / 60.0f;

	if (playerJumpTime_ < playerJumpDuration_) {
		float normalizedJumpTime = playerJumpTime_ / playerJumpDuration_;
		float jumpValue = EaseOutQuad(normalizedJumpTime); // イージングでジャンプの効果
		playerWorldTransform_.translation_.y = playerStartY_ + playerJumpHeight_ * jumpValue;
	} else {
		playerWorldTransform_.translation_.y = playerJumpHeight_; // ジャンプ終了
		isPlayerJumping_ = false;

		// ジャンプ終了後に次のX軸移動を開始
		isPlayerMovingAfterJump_ = true;
		playerAnimationTime_ = 0.0f; // 移動時間をリセット
	}
}

// PlayerのY軸移動処理
void TitleScene::UpdatePlayerFall() {
	playerFallTime_ += 1.0f / 20.0f;

	if (playerFallTime_ < playerFallDuration_) {
		float normalizedFallTime = playerFallTime_ / playerFallDuration_;
		playerWorldTransform_.translation_.y = playerJumpHeight_ + (0.0f - playerJumpHeight_) * normalizedFallTime;
	} else {
		playerWorldTransform_.translation_.y = 0.0f; // Y軸移動完了
		isPlayerFalling_ = false;                    // Y軸移動終了フラグをリセット
	}
}

void TitleScene::FadeInUpdate() {
	fade_->Update();
	if (fade_->GetStatus() == Fade::Status::None) {
		phase_ = Phase::kMain;
	}
}
void TitleScene::MainUpdate() {
	// Title2の落下アニメーション
	if (isAnimating_) {
		UpdateTitle2Drop();
	}

	// Playerの移動
	if (isPlayerAnimating_) {
		UpdatePlayerMove();
	}

	// Title1のスケール変更
	if (isScalingTitle1_) {
		UpdateTitle1Scale();
	}

	// Playerのジャンプ処理
	if (isPlayerJumping_) {
		UpdatePlayerJump();
	}

	// ジャンプ後のPlayerのX軸移動処理
	if (isPlayerMovingAfterJump_) {
		UpdatePlayerMoveAfterJump();
	}

	// PlayerのY軸移動処理
	if (isPlayerFalling_) {
		UpdatePlayerFall();
		title3WorldTransform_.UpdateMatrix();
	}

	// 各ワールド行列を更新
	title1WorldTransform_.UpdateMatrix();
	title2WorldTransform_.UpdateMatrix();
	
	playerWorldTransform_.UpdateMatrix();
	if (input_->GetInstance()->TriggerKey(DIK_SPACE)) {
		audio_->PlayWave(se_, false);
		phase_ = Phase::kFadeOut;
		fade_->Start(Fade::Status::FadeOut, 1.0f);
	}
}
void TitleScene::FadeOutUpdate() {
	fade_->Update();
	finished_ = fade_->IsFinished();
}
void TitleScene::FadeInDraw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	fade_->Draw(commandList);
}
void TitleScene::MainDraw() {}
void TitleScene::FadeOutDraw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	fade_->Draw(commandList);
}
void (TitleScene::*TitleScene::spFuncTableUpdate[])() = {
    &TitleScene::FadeInUpdate,
    &TitleScene::MainUpdate,
    &TitleScene::FadeOutUpdate,
};
void (TitleScene::*TitleScene::spFuncTableDraw[])() = {
    &TitleScene::FadeInDraw,
    &TitleScene::MainDraw,
    &TitleScene::FadeOutDraw,
};
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
	model2Title_->Draw(title2WorldTransform_, title2ViewProjection_);
	model3Title_->Draw(title3WorldTransform_, title3ViewProjection_);
	playerModel_->Draw(playerWorldTransform_, playerViewProjection_);
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
#pragma endregion
	(this->*spFuncTableDraw[static_cast<size_t>(phase_)])();
}

void TitleScene::PlayerInitialize() {
	playerModel_ = std::unique_ptr<Model>(Model::CreateFromOBJ("Player", true));
	playerWorldTransform_.Initialize();
	playerWorldTransform_.scale_ = {2.0f, 2.0f, 1.0f};
	playerWorldTransform_.translation_ = {-35.0f, 0.0f, 0.0f};
	playerViewProjection_.Initialize();

	playerStartX_ = playerWorldTransform_.translation_.x; // 初期位置 (-35.0f)
	playerEndX_ = -10.0f;                                 // 目的地
	playerAnimationTime_ = 0.0f;                          // 経過時間
	playerAnimationDuration_ = 2.0f;                      // アニメーション時間（2秒間で移動）
	isPlayerAnimating_ = true;                            // Playerのアニメーションが有効かどうか
}