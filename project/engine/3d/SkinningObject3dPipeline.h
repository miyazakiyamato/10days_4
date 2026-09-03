#pragma once
#include "BasePipeline.h"

namespace Engine {

/// <summary>
/// スキニング用3Dオブジェクトパイプライン
/// スキニングされた頂点データ（頂点シェーダーでの処理など）を用いて描画するための設定
/// </summary>
class SkinningObject3dPipeline : public BasePipeline {
private:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
		// --- メンバ関数 ---
	std::vector<D3D12_STATIC_SAMPLER_DESC> StaticSamplers(StaticSamplersMode statocSamplersMode);

	/// <summary>
	/// ルートシグネチャ取得
	/// 通常のモデルとは異なり、スキニング結果を受け取るための設定が含まれる可能性がある
	/// </summary>
	std::vector<D3D12_DESCRIPTOR_RANGE> DescriptorRanges() override;
	std::vector<D3D12_ROOT_PARAMETER> RootParameters(const std::vector<D3D12_DESCRIPTOR_RANGE>& descriptorRanges) override;

	/// <summary>
	/// 入力要素記述子の取得
	/// </summary>
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc() override;
};

} // namespace Engine
