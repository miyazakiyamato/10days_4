#pragma once
#include "BasePipeline.h"

namespace Engine {

/// <summary>
/// オフスクリーンパイプラインクラス
/// オフスクリーン描画用の設定を行う
/// </summary>
class OffScreenPipeline : public BasePipeline {
public:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
		// --- メンバ関数 ---
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
