#pragma once
#include "BasePipeline.h"

namespace Engine {

/// <summary>
/// パーティクル描画用グラフィックスパイプライン
/// 頂点シェーダー・ピクセルシェーダーを用いた描画設定を定義する
/// </summary>
class ParticlePipeline : public BasePipeline {
public:
	/// <summary>
	/// 静的サンプラーの設定
	/// </summary>
	std::vector<D3D12_STATIC_SAMPLER_DESC> StaticSamplers(StaticSamplersMode statocSamplersMode) override;

	/// <summary>
	/// ルートシグネチャのディスクリプタレンジ設定
	/// </summary>
	std::vector<D3D12_DESCRIPTOR_RANGE> DescriptorRanges() override;

	/// <summary>
	/// ルートパラメータの設定
	/// </summary>
	std::vector<D3D12_ROOT_PARAMETER> RootParameters(const std::vector<D3D12_DESCRIPTOR_RANGE>& descriptorRanges) override;

	/// <summary>
	/// 入力要素記述の設定
	/// </summary>
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc() override;
};

} // namespace Engine
