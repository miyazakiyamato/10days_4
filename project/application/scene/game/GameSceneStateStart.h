#pragma once
#include "BaseSceneState.h"
#include "GameStartUI.h"
#include <memory>

namespace Engine {

class GameScene;

class GameSceneStateStart : public BaseSceneState<GameScene> {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameSceneStateStart() override;

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
	GameStartUI* gameStartUI_ = nullptr;
	//フェードの持続時間
	float duration_ = 1.0f;
};

} // namespace Engine
