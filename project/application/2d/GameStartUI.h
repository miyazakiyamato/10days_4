#pragma once
#include "BaseUI.h"
#include <vector>
#include <memory>
#include "Vector2.h"

namespace Engine {

class GameStartUI : public BaseUI {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(float duration);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	bool IsFinished() const { return isFinished_; }
	bool ShouldClose() const { return shouldClose_; }
	float GetCounter() const { return counter_; }

private:
	std::vector<std::unique_ptr<Sprite>> sprites_;
	float duration_ = 0.0f;
	float counter_ = 0.0f;
	Vector2 spriteSize_ = { 0.0f, 0.0f }; // Initialize in cpp
	bool isFinished_ = false;
	bool shouldClose_ = false;
};

} // namespace Engine
