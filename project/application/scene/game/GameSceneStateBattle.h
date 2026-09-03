#include "BaseSceneState.h"
#include <memory>

namespace Engine {

class GameScene;

class GameSceneStateBattle : public BaseSceneState<GameScene> {
public:
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
};

} // namespace Engine
