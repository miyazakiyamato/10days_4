#include "StageSelect.h"
#include <cassert>
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"
#include "MyMtMatrix.h"

StageSelect::StageSelect() {}

StageSelect::~StageSelect() {
	delete modelPlayer_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete skydome_;

	delete fade_;
}

void StageSelect::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	se_ = audio_->LoadWave("maou_se_onepoint25.wav");

	worldTransform_.Initialize();
	uint32_t numBlockVirtical = stageNum_ % 5;
	uint32_t numBlockHorizontal = stageNum_ / 5;
	worldTransform_.translation_ = Vector3(float(numBlockVirtical) * 4.0f, float(numBlockHorizontal) * -6.0f + 2.0f, 0.0f);
	worldTransform_.UpdateMatrix();
	viewProjection_.Initialize();
	viewProjection_.translation_ = {8.0, 0, -25.0f};
	viewProjection_.UpdateMatrix();
	// 自キャラ生成
	modelPlayer_ = Model::CreateFromOBJ("Player");
	textureHandlePlayer_ = TextureManager::Load("./Resources/Player/player.png");

	modelBlock_.reset(Model::CreateFromOBJ("cube"));
	textureHandleBlocks_[0] = TextureManager::Load("./Resources/StageSelect/StageSelect1.png");
	textureHandleBlocks_[1] = TextureManager::Load("./Resources/StageSelect/StageSelect2.png");
	textureHandleBlocks_[2] = TextureManager::Load("./Resources/StageSelect/StageSelect3.png");
	textureHandleBlocks_[3] = TextureManager::Load("./Resources/StageSelect/StageSelect4.png");
	textureHandleBlocks_[4] = TextureManager::Load("./Resources/StageSelect/StageSelect5.png");
	textureHandleBlocks_[5] = TextureManager::Load("./Resources/StageSelect/StageSelect6.png");
	textureHandleBlocks_[6] = TextureManager::Load("./Resources/StageSelect/StageSelect7.png");
	textureHandleBlocks_[7] = TextureManager::Load("./Resources/StageSelect/StageSelect8.png");
	textureHandleBlocks_[8] = TextureManager::Load("./Resources/StageSelect/StageSelect9.png");
	textureHandleBlocks_[9] = TextureManager::Load("./Resources/StageSelect/StageSelect10.png");

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// スカイドーム
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	GenerateBlocks();
	//
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

}

void StageSelect::Update() {
	if (input_->TriggerKey(DIK_T)) {
		isReturnSelect_ = true;
		phase_ = Phase::kFadeOut;
		fade_->Start(Fade::Status::FadeOut, 1.0f);
	}

	skydome_->Update();
		(this->*spFuncTableUpdate[static_cast<size_t>(phase_)])();

	#ifdef _DEBUG
	ImGui::Begin("Debug");
	ImGui::DragInt("stageNum",&stageNum_);
	ImGui::DragFloat3("viewPorjection", &viewProjection_.translation_.x);
	ImGui::DragFloat3("worldTransform_.translation", &worldTransform_.translation_.x);
	ImGui::End();
	#endif // _DEBUG
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();
}

void StageSelect::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	if (!finished_) {

#pragma region 背景スプライト描画
		// 背景スプライト描画前処理
		Sprite::PreDraw(commandList);

		/// <summary>
		/// ここに背景スプライトの描画処理を追加できる
		/// </summary>
		
		// スプライト描画後処理
		Sprite::PostDraw();
		// 深度バッファクリア
		dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
		// 3Dオブジェクト描画前処理
		Model::PreDraw(commandList);

		/// <summary>
		/// ここに3Dオブジェクトの描画処理を追加できる
		/// </summary>
		skydome_->Draw();
		// 自キャラ描画
		modelPlayer_->Draw(worldTransform_, viewProjection_, textureHandlePlayer_);

		int index = 0;
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				
				modelBlock_->Draw(*worldTransformBlock, viewProjection_, textureHandleBlocks_[index]);
				index += 5;
			}
			index++;
				index -= 10;
		}

		// 3Dオブジェクト描画後処理
		Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
		// 前景スプライト描画前処理
		Sprite::PreDraw(commandList);

		/// <summary>
		/// ここに前景スプライトの描画処理を追加できる
		/// </summary>

		// スプライト描画後処理
		Sprite::PostDraw();

#pragma endregion
	}
	(this->*spFuncTableDraw[static_cast<size_t>(phase_)])();
}
void StageSelect::FadeInUpdate() {
	fade_->Update();
	if (fade_->GetStatus() == Fade::Status::None) {
		phase_ = Phase::kMain;
	}
}
void StageSelect::MainUpdate() {
	if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
		stageNum_ += 1;
		audio_->PlayWave(se_, false);
	}
	if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
		stageNum_ -= 1;
		audio_->PlayWave(se_, false);
	}
	if (input_->TriggerKey(DIK_UP) || input_->TriggerKey(DIK_W)) {
		stageNum_ -= 5;
		audio_->PlayWave(se_, false);
	}
	if (input_->TriggerKey(DIK_DOWN) || input_->TriggerKey(DIK_S)) {
		stageNum_ += 5;
		audio_->PlayWave(se_, false);
	}

	stageNum_ = stageNum_ < 0 ? 0 : stageNum_;
	stageNum_ = stageNum_ < LimitStageNum_ ? stageNum_ : LimitStageNum_ - 1;
	uint32_t numBlockVirtical = stageNum_ % 5;
	uint32_t numBlockHorizontal = stageNum_ / 5;
	worldTransform_.translation_ = Vector3(float(numBlockVirtical) * 4.0f, float(numBlockHorizontal) * -6.0f + 2.0f, 0.0f);
	worldTransform_.UpdateMatrix();

	if (input_->GetInstance()->TriggerKey(DIK_SPACE)) {
		audio_->PlayWave(se_, false);
		phase_ = Phase::kFadeOut;
		fade_->Start(Fade::Status::FadeOut, 1.0f);
	}
}
void StageSelect::FadeOutUpdate() {
	fade_->Update();
	finished_ = fade_->IsFinished();
}
void StageSelect::FadeInDraw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	fade_->Draw(commandList);
}
void StageSelect::MainDraw() {}
void StageSelect::FadeOutDraw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	fade_->Draw(commandList);
}
void (StageSelect::*StageSelect::spFuncTableUpdate[])() = {
    &StageSelect::FadeInUpdate,
    &StageSelect::MainUpdate,
    &StageSelect::FadeOutUpdate,
};
void (StageSelect::*StageSelect::spFuncTableDraw[])() = {
    &StageSelect::FadeInDraw,
    &StageSelect::MainDraw,
    &StageSelect::FadeOutDraw,
};

void StageSelect::GenerateBlocks() { // 要素数
	uint32_t numBlockVirtical = 5;
	uint32_t numBlockHorizontal = LimitStageNum_ / 5;
	// 要素を変更する
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// キューブの生成
	for (uint32_t y = 0; y < numBlockVirtical; ++y) {
		for (uint32_t x = 0; x < numBlockHorizontal; ++x) {
			WorldTransform* worldTransform = new WorldTransform();
			worldTransform->Initialize();
			worldTransformBlocks_[y][x] = worldTransform;
			worldTransformBlocks_[y][x]->translation_ = Vector3(float(y) * 4.0f, float(x) * -6.0f,0.0f);
			worldTransformBlocks_[y][x]->UpdateMatrix();
		}
	}
}