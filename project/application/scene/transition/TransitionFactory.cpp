#include "TransitionFactory.h"
#include "FadeTransition.h"
#include <cassert>

namespace Engine {

TransitionFactory::TransitionFactory(){
	// 遷移エフェクト生成関数の登録
	transitionGenerators_["FADE"] = []() { return std::make_unique<FadeTransition>(); };
}

std::unique_ptr<BaseTransition> TransitionFactory::CreateTransition(const std::string& transitionName){
	// マップから遷移エフェクト生成関数を検索
	auto it = transitionGenerators_.find(transitionName);
	if (it != transitionGenerators_.end()) {
		return it->second();
	}

	// 未登録の遷移名
	assert(0 && "不明な遷移名");
	return nullptr;
}

} // namespace Engine
