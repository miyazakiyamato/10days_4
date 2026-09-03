#pragma once
#include "DirectXCommon.h"
#include <queue>

namespace Engine {

/// <summary>
/// SRV/UAVマネージャクラス
/// シェーダーリソースビュー(SRV)およびアンオーダーアクセスビュー(UAV)のデスクリプタヒープを管理する
/// </summary>
class SrvUavManager {
public:
		// --- メンバ関数 ---
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// デスクリプタの確保（インデックス割り当て）
	/// </summary>
	/// <returns>確保されたSRVインデックス</returns>
	uint32_t Allocate();

	/// <summary>
	/// リソースの解放
	/// 指定インデックスを再利用可能にする
	/// </summary>
	void ResourceClear(uint32_t index);

	// --- ハンドル取得 ---
	/// <summary>
	/// SRVの指定番号のCPUデスクリプタハンドルを取得
	/// </summary>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// SRVの指定番号のGPUデスクリプタハンドルを取得
	/// </summary>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	// --- ビュー生成 ---
	/// <summary>
	/// テクスチャ用SRVの生成
	/// </summary>
	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, DirectX::TexMetadata metadata);

	/// <summary>
	/// 深度テクスチャ用SRVの生成
	/// </summary>
	void CreateSRVforDepthTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format);

	/// <summary>
	/// StructuredBuffer用SRVの生成
	/// </summary>
	void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	/// <summary>
	/// StructuredBuffer用UAVの生成
	/// </summary>
	void CreateUAVforStructuredBuffer(uint32_t uavIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	/// <summary>
	/// 描画前処理
	/// デスクリプタヒープをコマンドリストにセットする
	/// </summary>
	void PreDraw();

	/// <summary>
	/// グラフィックスルートディスクリプタテーブルの設定
	/// </summary>
	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);

	/// <summary>
	/// コンピュートルートディスクリプタテーブルの設定
	/// </summary>
	void SetComputeRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);

	/// <summary>
	/// 空き状況のチェック
	/// </summary>
	bool AvailabilityCheck();

	/// <summary>
	/// ImGui用デスクリプタヒープの取得
	/// </summary>
	ID3D12DescriptorHeap* GetDescriptorHeapForImGui();

private:
		// --- メンバ変数 ---
	DirectXCommon* dxCommon_ = nullptr;

	// 最大SRV数
	static const uint32_t kMaxSRVCount;
	// SRV用デスクリプタサイズ
	uint32_t descriptorSize;
	// SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	// 次に使用するSRVインデックス
	uint32_t useIndex = 1;
	// 削除(解放)済みインデックスキュー
	std::queue<uint32_t> deleteQueue;
};

} // namespace Engine
