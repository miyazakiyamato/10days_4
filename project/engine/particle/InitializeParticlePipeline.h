#pragma once
#include "BasePipeline.h"

namespace Engine {

/// <summary>
/// パーティクル初期化用パイプラインクラス
/// コンピュートシェーダーを用いてパーティクルリストの初期化を行う
/// </summary>
class InitializeParticlePipeline : public BasePipeline {
public:
	/// <summary>
	/// コンピュートシェーダー用のディスクリプタレンジ設定
	/// </summary>
	std::vector<D3D12_DESCRIPTOR_RANGE> ComputeDescriptorRanges() override;

	/// <summary>
	/// コンピュートシェーダー用のルートパラメータ設定
	/// </summary>
	std::vector<D3D12_ROOT_PARAMETER> ComputeRootParameters(const std::vector<D3D12_DESCRIPTOR_RANGE>& descriptorRanges) override;
};

} // namespace Engine
