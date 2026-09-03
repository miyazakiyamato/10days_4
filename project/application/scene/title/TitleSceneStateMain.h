#pragma once
#include "BaseSceneState.h"

#include "UIManager.h"
#include <memory>

namespace Engine {

class TitleScene;

class TitleSceneStateMain : public BaseSceneState<TitleScene> {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(TitleScene* scene) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:
	std::unique_ptr<UIManager> uiManager_;
};

} // namespace Engine
