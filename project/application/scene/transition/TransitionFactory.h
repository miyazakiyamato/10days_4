#pragma once
#include "AbstractTransitionFactory.h"
#include <map>
#include <functional>

namespace Engine {

/// <summary>
/// トランジションシーン工場
/// </summary>
class TransitionFactory : public AbstractTransitionFactory {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TransitionFactory();

	/// <summary>
	/// トランジションシーン生成
	/// </summary>
	/// <param name="sceneName">シーン遷移エフェクト名</param>
	/// <returns>生成したシーン遷移エフェクト</returns>
	std::unique_ptr<BaseTransition> CreateTransition(const std::string& transitionName) override;

private:
	// 遷移エフェクト生成関数を保持するマップ
	std::map<std::string, std::function<std::unique_ptr<BaseTransition>()>> transitionGenerators_;
};

} // namespace Engine
