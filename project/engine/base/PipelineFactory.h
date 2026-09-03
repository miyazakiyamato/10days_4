#pragma once
#include "BasePipeline.h"
#include <memory>

namespace Engine {

/// <summary>
/// パイプラインファクトリークラス
/// パイプライン名に基づいて適切なパイプラインインスタンスを生成する
/// </summary>
class PipelineFactory {
public:
	/// <summary>
	/// パイプラインの切り替え・生成
	/// </summary>
	/// <param name="pipelineName">生成するパイプラインの名前</param>
	/// <returns>生成されたBasePipeline派生クラスへのポインタ</returns>
	static std::unique_ptr<BasePipeline> ChangePipeline(const std::string& pipelineName);
};

} // namespace Engine
