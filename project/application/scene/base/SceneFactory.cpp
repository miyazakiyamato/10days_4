#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "GameOverScene.h"
#include "DParticleScene.h"

namespace Engine {

SceneFactory::SceneFactory(){
	// シーン生成関数の登録
	sceneGenerators_["TITLE"] = []() { return std::make_unique<TitleScene>(); };
	sceneGenerators_["GAME"] = []() { return std::make_unique<GameScene>(); };
	sceneGenerators_["CLEAR"] = []() { return std::make_unique<ClearScene>(); };
	sceneGenerators_["GAMEOVER"] = []() { return std::make_unique<GameOverScene>(); };
	sceneGenerators_["D_PARTICLE"] = []() { return std::make_unique<DParticleScene>(); };
}

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName){
	// マップからシーン生成関数を検索
	auto it = sceneGenerators_.find(sceneName);
	if (it != sceneGenerators_.end()) {
		return it->second();
	}

	// 未登録のシーン名
	assert(0 && "不明なシーン名");
	return nullptr;
}

} // namespace Engine
