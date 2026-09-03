#pragma once
#include "BaseScene.h"
#include "BaseSceneState.h"
#include <vector>
#include <memory>
#include "Sprite.h"

namespace Engine {

/// <summary>
/// ポーズシーン
/// </summary>
class PauseScene : public BaseScene {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	// --- ステートパターン ---
	/// <summary>
	/// ステート変更
	/// </summary>
	void ChangeState(std::unique_ptr<BaseSceneState<PauseScene>> newState);

public: // Accessors for States
	Sprite* GetBgSprite() const { return bgSprite_.get(); }
	const std::vector<std::unique_ptr<Sprite>>& GetButtonSprites() const { return buttonSprites_; }
	int GetSelectionIndex() const { return currentSelectionIndex_; }
	void SetSelectionIndex(int index) { currentSelectionIndex_ = index; }

private:
	// ステート
	// 現在のステート
	std::unique_ptr<BaseSceneState<PauseScene>> state_;
	// 次のステート（更新処理終了後に切り替える）
	std::unique_ptr<BaseSceneState<PauseScene>> nextState_;

	// UIスプライト
	std::unique_ptr<Sprite> bgSprite_;
	std::unique_ptr<Sprite> pauseTitleSprite_;
	std::vector<std::unique_ptr<Sprite>> buttonSprites_;

public:
	Sprite* GetPauseTitleSprite() const { return pauseTitleSprite_.get(); }
private:

	// 選択中のインデックス
	int currentSelectionIndex_ = 0;
};

} // namespace Engine
