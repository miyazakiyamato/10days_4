#include "PauseScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "TextureManager.h"
#include "PauseStateStart.h"

namespace Engine {

void PauseScene::Initialize() {
	// ベースシーンの初期化 (必要であれば)
	BaseScene::Initialize();

	// テクスチャロード
	TextureManager::GetInstance()->LoadTexture("white.dds");
	TextureManager::GetInstance()->LoadTexture("keepPlaying.dds");
	TextureManager::GetInstance()->LoadTexture("backToTitle.dds");
	TextureManager::GetInstance()->LoadTexture("pause.dds");

	// 背景スプライト作成 (黒、透過)
	bgSprite_ = std::make_unique<Sprite>();
	bgSprite_->Initialize("white.dds");
	bgSprite_->SetSize({ 0.0f, 0.0f }); // 初期サイズ0
	bgSprite_->SetColor({ 0.0f, 0.0f, 0.0f, 0.5f });
	bgSprite_->SetPosition({ 0.0f, 0.0f });
	bgSprite_->Update();

	// タイトルスプライト ("pause.dds")
	pauseTitleSprite_ = std::make_unique<Sprite>();
	pauseTitleSprite_->Initialize("pause.dds");
	pauseTitleSprite_->SetSize({ 0.0f, 0.0f }); // 初期サイズ0
	pauseTitleSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	pauseTitleSprite_->SetPosition({ 640.0f, 200.0f }); // 真ん中の少し上
	pauseTitleSprite_->Update();

	// ボタンボックス作成 (2つ)
	std::vector<std::string> buttonTextures = { "keepPlaying.dds", "backToTitle.dds" };
	for (int i = 0; i < 2; ++i) {
		std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
		sprite->Initialize(buttonTextures[i]);
		sprite->SetSize({ 0.0f, 0.0f }); // 初期サイズ0
		sprite->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		sprite->SetAnchorPoint({ 0.5f, 0.5f });
		// 中央 (360) を基準に配置
		float y = 360.0f + (i - 0.5f) * 80.0f; 
		sprite->SetPosition({ 640.0f, y });
		sprite->Update();
		buttonSprites_.push_back(std::move(sprite));
	}

	// 初期ステート設定
	// Initialize内なので即座に適用して問題ないが、ChangeStateがnextState_に入れる仕様になったため、
	// 手動で適用するか、Updateの最初のフレームで適用させる。
	// ここでは手動で適用しておく。
	state_ = std::make_unique<PauseStateStart>();
	state_->Initialize(this);
}

void PauseScene::ChangeState(std::unique_ptr<BaseSceneState<PauseScene>> newState) {
	nextState_ = std::move(newState);
}

void PauseScene::Update() {
	// ステート切り替えリクエストがあれば実行
	if (nextState_) {
		state_ = std::move(nextState_);
		state_->Initialize(this);
	}

	if (state_) {
		state_->Update();
	}
}

void PauseScene::Draw() {
	// 背景のゲーム描画は SceneManager が担当する

	// 背景描画
	if (bgSprite_) {
		bgSprite_->Draw();
	}
	// タイトル描画
	if (pauseTitleSprite_) {
		pauseTitleSprite_->Draw();
	}
	// ボタン描画
	for (auto& sprite : buttonSprites_) {
		sprite->Draw();
	}
}

} // namespace Engine
