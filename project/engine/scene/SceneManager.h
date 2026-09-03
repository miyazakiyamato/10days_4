#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include "BaseTransition.h"
#include "AbstractTransitionFactory.h"
#include "PipelineManager.h"
#include <unordered_map>
#include <memory>

namespace Engine {

/// <summary>
/// シーンマネージャクラス
/// 複数のシーンを管理し、切り替えや並行実行を制御するシングルトン
/// </summary>
class SceneManager {
public:
		// --- メンバ関数 ---
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	static SceneManager* GetInstance();

	/// <summary>
	/// 終了処理
	/// 管理している全シーンの破棄を行う
	/// </summary>
	void Finalize();

	/// <summary>
	/// 毎フレーム更新
	/// シーン変更後、シーンのUpdateを呼ぶ
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// シーンのDrawを呼ぶ
	/// </summary>
	void Draw();

	/// <summary>
	/// シーン変更
	/// 次のシーン予約する
	/// </summary>
	void ChangeScene(std::string sceneName);

	/// <summary>
	/// ポーズシーンへ変更（現在のシーンをスタックに積む）
	/// </summary>
	void ChangeSceneToPause(std::unique_ptr<BaseScene> pauseScene);

	/// <summary>
	/// シーン復帰（スタックからシーンを戻す）
	/// </summary>
	void ReturnScene();

	/// <summary>
	/// シーン遷移エフェクト変更
	/// 次のシーン遷移エフェクトを変更
	/// </summary>
	/// <param name="transitionName">遷移名</param>
	/// <param name="isGamePaused">遷移中にシーン更新を止めるか（true: 止める、false: 止めない）</param>
	void ChangeTransition(std::string transitionName, bool isGamePaused = false);
private:
	/// <summary>
	/// シーン遷移開始
	/// シーン遷移開始とシーンの変更を行う
	/// </summary>
	void SceneTransition();

private:
	// 外部からは絶対にインスタンス化できない「鍵」となる構造体を定義
	struct PrivateToken {};
public:
	// シングルトン(make_uniqueする用)
	explicit SceneManager(PrivateToken) {}
	~SceneManager() = default;
	// コピー・ムーブの禁止
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

private:
		// --- シングルトン ---
	static std::unique_ptr<SceneManager> instance;

		// --- メンバ変数 ---
	//今のシーン
	std::unique_ptr<BaseScene> scene_ = nullptr;
	//次のシーン
	std::unique_ptr<BaseScene> nextScene_ = nullptr;
	// シーンスタック (ポーズ用)
	std::vector<std::unique_ptr<BaseScene>> sceneStack_;

	// シーン切り替えモード
	bool isPushMode_ = false;
	bool isPopMode_ = false;

	// シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;

	// シーン遷移エフェクト
	std::unique_ptr<BaseTransition> transition_ = nullptr;
	// シーン遷移エフェクトファクトリー
	AbstractTransitionFactory* transitionFactory_ = nullptr;

	// トランジション中にシーン更新を止めるか
	bool isScenePausedOnTransition_ = false;

public:
		// --- ゲッター ---
	/// シーン遷移エフェクトの取得
	BaseTransition* GetTransition() const { return transition_.get(); }
		// --- セッター ---
	/// <summary>
	/// シーンファクトリーの設定
	/// 初期化時に必ず設定する必要がある
	/// </summary>
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }
	/// <summary>
	/// シーン遷移エフェクトファクトリーの設定
	///	初期化時に必ず設定する必要がある
	/// </summary>
	void SetTransitionFactory(AbstractTransitionFactory* transitionFactory) { transitionFactory_ = transitionFactory; }
};

} // namespace Engine
