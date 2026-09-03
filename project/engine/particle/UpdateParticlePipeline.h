#pragma once
#include "BasePipeline.h"

namespace Engine {

/// <summary>
/// パーティクル更新用パイプラインクラス
/// コンピュートシェーダーを用いてパーティクルの移動・寿命計算を行う
/// </summary>
class UpdateParticlePipeline : public BasePipeline {
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
