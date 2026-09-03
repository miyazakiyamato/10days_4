#pragma once
#include "AbstractSceneFactory.h"

#include <map>
#include <functional>

namespace Engine {

/// <summary>
/// シーン工場
/// </summary>
class SceneFactory : public AbstractSceneFactory{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneFactory();

	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	/// <returns>生成したシーン</returns>
	std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName)override;

private:
	// シーン生成関数を保持するマップ
	std::map<std::string, std::function<std::unique_ptr<BaseScene>()>> sceneGenerators_;
};


} // namespace Engine
