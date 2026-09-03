#pragma once
#include "DirectXCommon.h"
#include "BlendMode.h"
#include <map>

namespace Engine {

/// <summary>
/// 静的サンプラーのモード
/// </summary>
enum class StaticSamplersMode {
	kwrap,  //!< Wrapモード (繰り返し)
	kclamp, //!< Clampモード (端の色を引き伸ばす)
};

/// <summary>
/// パイプライン基底クラス
/// グラフィックスパイプラインやコンピュートパイプライン構築に必要な共通処理を定義する
/// </summary>
class BasePipeline {
public:
	// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
		// --- メンバ関数 ---
	/// <summary>
	/// 静的サンプラーの定義を取得する
	/// </summary>
	/// <param name="staticSamplersMode">サンプラーのモード</param>
	/// <returns>静的サンプラー記述のリスト</returns>
	virtual std::vector<D3D12_STATIC_SAMPLER_DESC> StaticSamplers(StaticSamplersMode staticSamplersMode);

	/// <summary>
	/// ディスクリプタレンジの設定を取得する
	/// </summary>
	/// <returns>ディスクリプタレンジのリスト</returns>
	virtual std::vector<D3D12_DESCRIPTOR_RANGE> DescriptorRanges();

	/// <summary>
	/// ルートパラメータの設定を取得する
	/// </summary>
	/// <param name="descriptorRanges">使用するディスクリプタレンジ</param>
	/// <returns>ルートパラメータのリスト</returns>
	virtual std::vector<D3D12_ROOT_PARAMETER> RootParameters(const std::vector<D3D12_DESCRIPTOR_RANGE>& descriptorRanges);

	/// <summary>
	/// 入力レイアウトの設定を取得する
	/// </summary>
	/// <returns>入力要素記述のリスト</returns>
	virtual std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc();

	/// <summary>
	/// コンピュートシェーダー用ディスクリプタレンジの設定を取得する
	/// </summary>
	/// <returns>ディスクリプタレンジのリスト</returns>
	virtual std::vector<D3D12_DESCRIPTOR_RANGE> ComputeDescriptorRanges();

	/// <summary>
	/// コンピュートシェーダー用ルートパラメータの設定を取得する
	/// </summary>
	/// <param name="descriptorRanges">使用するディスクリプタレンジ</param>
	/// <returns>ルートパラメータのリスト</returns>
	virtual std::vector<D3D12_ROOT_PARAMETER> ComputeRootParameters(const std::vector<D3D12_DESCRIPTOR_RANGE>& descriptorRanges);

		// --- ヘルパー関数 ---
	/// <summary>
	/// 静的サンプラー記述を作成する
	/// </summary>
	D3D12_STATIC_SAMPLER_DESC CreateStaticSampler(D3D12_FILTER filter, StaticSamplersMode staticSamplersMode, UINT ShaderRegister);

	/// <summary>
	/// ディスクリプタレンジを作成する
	/// </summary>
	D3D12_DESCRIPTOR_RANGE CreateDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type, UINT baseShaderRegister, UINT numDescriptors);

	/// <summary>
	/// 定数バッファビュー(CBV)用のルートパラメータを作成する
	/// </summary>
	D3D12_ROOT_PARAMETER CreateRootParameterCBV(D3D12_SHADER_VISIBILITY visibility, UINT shaderRegister);

	/// <summary>
	/// ディスクリプタテーブル用のルートパラメータを作成する
	/// </summary>
	D3D12_ROOT_PARAMETER CreateRootParameterTable(D3D12_SHADER_VISIBILITY visibility, const D3D12_DESCRIPTOR_RANGE& descriptorRegister, UINT descriptorCount);

	/// <summary>
	/// 入力要素記述を作成する
	/// </summary>
	D3D12_INPUT_ELEMENT_DESC CreateInputElementDesc(const char* semanticName, UINT semanticIndex, DXGI_FORMAT format, UINT inputSlot, UINT alignedByteOffset);
};

} // namespace Engine
