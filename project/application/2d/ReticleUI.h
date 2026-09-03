#pragma once
#include "BaseUI.h"
#include <memory>
#include "Vector2.h"

namespace Engine {

class ReticleUI : public BaseUI {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ImGui更新
	/// </summary>
	void ImGuiUpdate() override;

	/// <summary>
	/// 座標の取得
	/// </summary>
	Vector2 GetPosition() const;

	/// <summary>
	/// 座標の設定
	/// </summary>
	void SetPosition(const Vector2& position);

private:
	std::unique_ptr<Sprite> sprite_;
};

} // namespace Engine
