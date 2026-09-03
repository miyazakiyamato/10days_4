#pragma once
#include "BasePipeline.h"

namespace Engine {

/// <summary>
/// スプライト描画用パイプラインクラス
/// BasePipelineを継承し、スプライト描画に必要なルートシグネチャや入力レイアウトを定義する
/// </summary>
class SpritePipeline : public BasePipeline {
private:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
		// --- メンバ関数 ---
	/// <summary>
	/// 静的サンプラーの設定を取得する
	/// テクスチャのフィルタリング（Point, Linearなど）やアドレッシングモードを定義
	/// </summary>
	/// <param name="staticSamplersMode">サンプラー設定モード</param>
	/// <returns>静的サンプラー記述子のリスト</returns>
	std::vector<D3D12_STATIC_SAMPLER_DESC> StaticSamplers(StaticSamplersMode staticSamplersMode);

	/// <summary>
	/// ルートシグネチャのディスクリプタレンジ設定を取得する
	/// テクスチャレジスタ(t0)などの範囲を定義
	/// </summary>
	/// <returns>ディスクリプタレンジのリスト</returns>
	std::vector<D3D12_DESCRIPTOR_RANGE> DescriptorRanges() override;

	/// <summary>
	/// ルートパラメータの設定を取得する
	/// 定数バッファ(b0, b1)やディスクリプタテーブルの設定を定義
	/// </summary>
	/// <param name="descriptorRanges">使用するディスクリプタレンジ</param>
	/// <returns>ルートパラメータのリスト</returns>
	std::vector<D3D12_ROOT_PARAMETER> RootParameters(const std::vector<D3D12_DESCRIPTOR_RANGE>& descriptorRanges) override;

	/// <summary>
	/// 入力要素記述子（Input Layout）を取得する
	/// 頂点シェーダーへ渡すデータフォーマット（POSITION, TEXCOORDなど）を定義
	/// </summary>
	/// <returns>入力要素記述子のリスト</returns>
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc() override;
};

} // namespace Engine
