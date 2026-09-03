#pragma once
#include "BasePipeline.h"

namespace Engine {

/// <summary>
/// 3Dライン描画用パイプラインクラス
/// BasePipelineを継承し、ワイヤーフレーム描画に適したパイプライン設定を定義する
/// </summary>
class Line3DPipeline : public BasePipeline {
private:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
		// --- メンバ関数 ---
	/// <summary>
	/// 静的サンプラーの設定を取得する
	/// </summary>
	std::vector<D3D12_STATIC_SAMPLER_DESC> StaticSamplers(StaticSamplersMode statocSamplersMode);

	/// <summary>
	/// ルートシグネチャのディスクリプタレンジ設定を取得する
	/// </summary>
	std::vector<D3D12_DESCRIPTOR_RANGE> DescriptorRanges();

	/// <summary>
	/// ルートパラメータの設定を取得する
	/// </summary>
	std::vector<D3D12_ROOT_PARAMETER> RootParameters(const std::vector<D3D12_DESCRIPTOR_RANGE>& descriptorRanges);

	/// <summary>
	/// 入力要素記述子（Input Layout）を取得する
	/// PositionとColorを持つレイアウト定義
	/// </summary>
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc();
};

} // namespace Engine
