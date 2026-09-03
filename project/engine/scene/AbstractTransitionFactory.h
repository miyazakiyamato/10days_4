#pragma once
#include <string>
#include <memory>
#include "BaseTransition.h"

namespace Engine {

/// <summary>
/// トランジションシーン工場基底クラス
/// </summary>
class AbstractTransitionFactory {
public:
	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~AbstractTransitionFactory() = default;

	/// <summary>
	/// シーン遷移エフェクト生成
	/// </summary>
	/// <param name="sceneName">シーン遷移エフェクト名</param>
	/// <returns>生成したシーン遷移エフェクト</returns>
	virtual std::unique_ptr<BaseTransition> CreateTransition(const std::string& transitionName) = 0;
};

} // namespace Engine
