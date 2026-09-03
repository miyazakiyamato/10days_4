#pragma once
#include "BasePipeline.h"

namespace Engine {

/// <summary>
/// パーティクル発生用パイプラインクラス
/// コンピュートシェーダーを用いて新規パーティクルを生成(Emit)する
/// </summary>
class EmitParticlePipeline : public BasePipeline {
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
