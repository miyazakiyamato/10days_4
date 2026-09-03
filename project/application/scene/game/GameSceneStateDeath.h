#pragma once
#include "BaseSceneState.h"
#include "PlayerDeathUI.h"
#include <memory>

namespace Engine {

class GameScene;

class GameSceneStateDeath : public BaseSceneState<GameScene> {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameSceneStateDeath() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(GameScene* gameScene) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:
	PlayerDeathUI* playerDeathUI_ = nullptr;

	//フェードの持続時間
	float duration_ = 1.0f;
	//経過時間カウンター
	float counter_ = 0.0f;
	bool isTransitioning_ = false;
};

} // namespace Engine
