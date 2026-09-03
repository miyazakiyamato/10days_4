#pragma once
#include "BaseUI.h"
#include <vector>
#include <memory>

namespace Engine {

class GameOverUI : public BaseUI {
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

private:
	std::vector<std::unique_ptr<Sprite>> sprites_;
	float animationTime_ = 0.0f;
};

} // namespace Engine
