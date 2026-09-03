#pragma once
#include "Sprite.h"

namespace Engine {

/// <summary>
/// UI基底クラス
/// </summary>
class BaseUI {
public:
	virtual ~BaseUI() = default;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// ImGui更新
	/// </summary>
	virtual void ImGuiUpdate() {}

	/// <summary>
	/// デッドフラグの取得
	/// </summary>
	bool GetIsDead() const { return isDead_; }

	/// <summary>
	/// デッドフラグの設定
	/// </summary>
	void SetIsDead(bool isDead) { isDead_ = isDead; }

protected:
	// デッドフラグ
	bool isDead_ = false;
};

} // namespace Engine
