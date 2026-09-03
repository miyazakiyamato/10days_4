#pragma once
#include "BaseUI.h"
#include <vector>
#include <memory>
#include "Vector2.h"

namespace Engine {

class PlayerWinUI : public BaseUI {
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

	void SetSpriteSize(const Vector2& size);
	void SetColor(const Vector4& color);

private:
	std::vector<std::unique_ptr<Sprite>> sprites_;
};

} // namespace Engine
