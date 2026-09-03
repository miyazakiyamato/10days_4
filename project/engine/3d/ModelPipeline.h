#pragma once
#include "BasePipeline.h"

namespace Engine {

/// <summary>
/// 通常モデル描画用グラフィックスパイプライン
/// 静的なモデルの描画設定を管理する
/// </summary>
class ModelPipeline : public BasePipeline {
private:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
		//	--- メンバ関数 ---
	/// <summary>
	/// 静的サンプラーの設定
	/// </summary>
	std::vector<D3D12_STATIC_SAMPLER_DESC> StaticSamplers(StaticSamplersMode statocSamplersMode);

	/// <summary>
	/// ルートシグネチャの定義（ディスクリプタレンジ）
	/// </summary>
	std::vector<D3D12_DESCRIPTOR_RANGE> DescriptorRanges() override;

	/// <summary>
	/// ルートパラメータの定義
	/// </summary>
	std::vector<D3D12_ROOT_PARAMETER> RootParameters(const std::vector<D3D12_DESCRIPTOR_RANGE>& descriptorRanges) override;

	/// <summary>
	/// 入力要素記述子（Input Layout）
	/// POSITION, TEXCOORD, NORMALなどを定義
	/// </summary>
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc() override;

	// コンピュートシェーダー関連（※BasePipelineの継承要件）
	std::vector<D3D12_DESCRIPTOR_RANGE> ComputeDescriptorRanges() override;
	std::vector<D3D12_ROOT_PARAMETER> ComputeRootParameters(const std::vector<D3D12_DESCRIPTOR_RANGE>& descriptorRanges) override;
};

} // namespace Engine
