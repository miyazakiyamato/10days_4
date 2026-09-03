#pragma once
#include "BaseSceneState.h"

#include "Player.h"
#include "UIManager.h"
#include <memory>

namespace Engine {

class ClearScene;

class ClearSceneStateMain : public BaseSceneState<ClearScene> {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ClearScene* scene) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:
	std::unique_ptr<Player> player_;
	std::unique_ptr<UIManager> uiManager_;
};

} // namespace Engine
