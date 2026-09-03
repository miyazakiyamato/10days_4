#pragma once
#include "BasePipeline.h"

namespace Engine {

/// <summary>
/// スカイボックス描画用パイプライン
/// キューブマップテクスチャを使用して背景（天球）を描画する
/// </summary>
class SkyboxPipeline : public BasePipeline {
private:
	// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	// --- メンバ関数 ---
	std::vector<D3D12_STATIC_SAMPLER_DESC> StaticSamplers(StaticSamplersMode statocSamplersMode) override;

	/// <summary>
	/// ルートシグネチャ取得
	/// キューブマップ用SRVの設定など
	/// </summary>
	std::vector<D3D12_DESCRIPTOR_RANGE> DescriptorRanges() override;
	std::vector<D3D12_ROOT_PARAMETER> RootParameters(const std::vector<D3D12_DESCRIPTOR_RANGE>& descriptorRanges) override;

	/// <summary>
	/// 入力要素記述子
	/// ポジションのみなど、軽量な構成になることが多い
	/// </summary>
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc() override;
};

} // namespace Engine
