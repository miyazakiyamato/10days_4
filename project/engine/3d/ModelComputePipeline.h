#pragma once
#include "BasePipeline.h"

namespace Engine {

/// <summary>
/// モデル用コンピュートシェーダーパイプライン
/// GPUスキニングなどの計算処理に使用するパイプライン設定を管理
/// </summary>
class ModelComputePipeline : public BasePipeline {
private:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
		// --- メンバ関数 ---
	/// <summary>
	/// コンピュートシェーダー用ディスクリプタレンジ設定の取得
	/// </summary>
	std::vector<D3D12_DESCRIPTOR_RANGE> ComputeDescriptorRanges() override;

	/// <summary>
	/// コンピュートシェーダー用ルートパラメータ設定の取得
	/// </summary>
	std::vector<D3D12_ROOT_PARAMETER> ComputeRootParameters(const std::vector<D3D12_DESCRIPTOR_RANGE>& descriptorRanges) override;
};

} // namespace Engine
