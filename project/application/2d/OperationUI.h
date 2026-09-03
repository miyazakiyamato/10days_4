#pragma once
#include "BaseUI.h"
#include <vector>
#include <memory>
#include "Sprite.h"

namespace Engine {

class Player;
/// <summary>
/// 操作説明UIクラス
/// </summary>
class OperationUI : public BaseUI {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Initialize(Player* player);

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

private:
	Player* player_ = nullptr;
	std::vector<std::unique_ptr<Sprite>> sprites_;
};

} // namespace Engine
