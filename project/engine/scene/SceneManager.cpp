#include "SceneManager.h"
#include <cassert>
#include "GlobalVariables.h"
#ifdef USE_IMGUI
#include <imgui.h>
#endif

namespace Engine {

std::unique_ptr<SceneManager> SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance(){
	//インスタンスがなければ生成
	if (instance == nullptr) {
		instance = std::make_unique<SceneManager>(PrivateToken{});
	}
	return instance.get();
}

void SceneManager::Finalize() {
	//インスタンスの削除
	instance.reset();
}

void SceneManager::Update() {
	//シーン遷移の開始とシーンの変更処理
	SceneTransition();

	//シーンの更新
	if (!transition_ || !isScenePausedOnTransition_) {
		scene_->Update();
	}
	//シーン遷移エフェクトの更新
	if (transition_) {
		transition_->Update();
	}
}

void SceneManager::Draw() {
	// スタックされているシーンを描画（背景として）
	for (auto& stack : sceneStack_) {
		stack->Draw();
	}
	//シーンの描画
	if (scene_) {
		scene_->Draw();
	}
	
	//シーン遷移エフェクトの描画
	if (transition_) {
		transition_->Draw();
	}
}

void SceneManager::ChangeScene(std::string sceneName) {
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	if (scene_ == nullptr) {
		//最初のシーンの場合は即座にセットする
		scene_ = std::move(sceneFactory_->CreateScene(sceneName));
		scene_->Initialize();
		scene_->SetSceneManager(this);
		return;
	}

	// 次のシーンを生成
	nextScene_ = sceneFactory_->CreateScene(sceneName);
	isPushMode_ = false;
	isPopMode_ = false;
}

void SceneManager::ChangeSceneToPause(std::unique_ptr<BaseScene> pauseScene) {
	// トランジション中は割り込み（ポーズ）不可
	if (transition_) { return; }

	assert(nextScene_ == nullptr);
	nextScene_ = std::move(pauseScene);
	isPushMode_ = true;
}

void SceneManager::ReturnScene() {
	isPopMode_ = true;
}

void SceneManager::ChangeTransition(std::string transitionName, bool isGamePaused){
	assert(transitionFactory_);
	if (transition_ != nullptr) {
		// すでにFadeIn中なら中断して次のFadeOutを開始する
		if (transition_->GetType() == BaseTransition::Type::kIn) {
			transition_->Finalize();
			transition_.reset();
		}
		else {
			// FadeOut中なら無視（重複呼び出し防止）
			return;
		}
	}

	transition_ = std::move(transitionFactory_->CreateTransition(transitionName));
	transition_->Initialize();
	transition_->SetType(BaseTransition::Type::kOut);
	isScenePausedOnTransition_ = isGamePaused;
}

void SceneManager::SceneTransition(){
	//シーン遷移エフェクトが終了しているなら
	if (transition_ && transition_->GetType() == BaseTransition::Type::kIn) {
		if (transition_->IsFinished()) {
			//シーン遷移エフェクトの終了
			transition_->Finalize();
			transition_.reset();
		}
		return;
	}

	// Popモードの処理
	if (isPopMode_) {
		if (!sceneStack_.empty()) {
			// 現在のシーンを破棄し、スタックから戻す
			scene_ = std::move(sceneStack_.back());
			sceneStack_.pop_back();
		}
		isPopMode_ = false;
		return;
	}

	//次のシーンの予約がないならreturn
	if (nextScene_ == nullptr) { return; }

	// transition_が存在しないなら
	if (!transition_ ||
		transition_->GetType() == BaseTransition::Type::kOut && transition_->IsFinished()) {

		if (isPushMode_) {
			// 現在のシーンをスタックに積む
			if (scene_) {
				sceneStack_.push_back(std::move(scene_));
			}
			isPushMode_ = false;
		}
		else {
			// 通常遷移
			sceneStack_.clear();
		}

		//シーンの切り替え
		scene_ = std::move(nextScene_);
		//次のシーンを初期化する
		scene_->Initialize();
		scene_->SetSceneManager(this);

		if (transition_) {
			//シーン遷移エフェクトの初期化
			transition_->Finalize();
			transition_->Initialize();
			transition_->SetType(BaseTransition::Type::kIn);
		}
	}
}



} // namespace Engine
