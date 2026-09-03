#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>
#include <dxcapi.h>
#include <string>
#include <thread>
#include <chrono>
#include "WinApp.h"
#include "externals/DirectXTex/DirectXTex.h"
#include "Vector4.h"

namespace Engine {

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

namespace {
	// RTVの数
	const uint32_t kRTVHandleSwapChainNum = 2;
	const uint32_t kRTVHandleRenderTextureNum = 1;
	const uint32_t kRTVHandleNum = 3;
}

class SrvUavManager;

/// <summary>
/// DirectX12基盤クラス
/// デバイス生成、コマンドリスト管理、スワップチェーン制御など、DX12の基本機能を提供する
/// </summary>
class DirectXCommon {
public:
		// --- メンバ関数 ---
	/// <summary>
	/// 初期化処理
	/// デバイス、コマンド、スワップチェーン、各ヒープの生成を行う
	/// </summary>
	/// <param name="winApp">Windowsアプリケーションクラスのポインタ</param>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// 描画前処理(レンダーテクスチャ)
	/// リソースバリアの設定やRTVのクリアを行う（オフスクリーン用）
	/// </summary>
	void RenderTexturePreDraw();

	/// <summary>
	/// 描画前処理(スワップチェイン)
	/// リソースバリアの設定やRTVのクリアを行う（画面表示用）
	/// </summary>
	void SwapChainPreDraw();

	/// <summary>
	/// オフスクリーン深度描画設定
	/// </summary>
	void OffScreenDepthDraw();

	/// <summary>
	/// オフスクリーン描画設定
	/// </summary>
	void OffScreenDraw();

	/// <summary>
	/// 描画後処理
	/// スワップチェーンのフリップやフェンスによる同期を行う
	/// </summary>
	void PostDraw();

	/// <summary>
	/// DX12デバイスの生成
	/// </summary>
	void CreateDevice();

	/// <summary>
	/// コマンド関連リソースの生成
	/// コマンドアロケータ、コマンドリスト、コマンドキュー
	/// </summary>
	void CreateCommand();

	/// <summary>
	/// スワップチェーンの生成
	/// </summary>
	void CreateSwapChain();

	/// <summary>
	/// 深度ステンシルバッファの生成
	/// </summary>
	void CreateDepthStencil();

	/// <summary>
	/// ディスクリプタヒープの生成
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& heapType, const UINT& numDescriptors, bool shaderVisible);

	/// <summary>
	/// 各種ディスクリプタヒープの一括生成
	/// </summary>
	void CreateDescriptorHeaps();

	/// <summary>
	/// RTV用ディスクリプタヒープの初期化と作成
	/// </summary>
	void CreateRTVDescriptorHeaps();

	/// <summary>
	/// オフスクリーン用SRVの作成
	/// </summary>
	void CreateOffScreenSRV(SrvUavManager* srvUavManager);

	/// <summary>
	/// 指定の番号のCPUデスクリプタハンドルを取得
	/// </summary>
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, const uint32_t& descriptorSize, const uint32_t& index);

	/// <summary>
	/// 指定の番号のGPUデスクリプタハンドルを取得
	/// </summary>
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, const uint32_t& descriptorSize, const uint32_t& index);

	/// <summary>
	/// 深度ステンシルビュー(DSV)の初期化
	/// </summary>
	void CreateDepthStencilView();

	/// <summary>
	/// フェンスの初期化
	/// </summary>
	void CreateFence();

	/// <summary>
	/// ビューポート矩形の初期化
	/// </summary>
	void CreateViewportRect();

	/// <summary>
	/// シザー矩形の初期化
	/// </summary>
	void CreateScissorRect();

	/// <summary>
	/// DXCコンパイラの生成
	/// </summary>
	void CreateDXCCompiler();

	/// <summary>
	/// シェーダーのコンパイル
	/// </summary>
	/// <param name="filePath">シェーダーファイルのパス</param>
	/// <param name="profile">コンパイルプロファイル (例: vs_6_0)</param>
	/// <returns>コンパイル済みBlob</returns>
	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile);

	/// <summary>
	/// レンダーテクスチャリソースの生成
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTextureResource(DXGI_FORMAT format, const Vector4& clearColor);

	/// <summary>
	/// バッファリソースの生成
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(const size_t& sizeInbytes);

	/// <summary>
	/// RW(Read/Write)バッファリソースの生成
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateRWBufferResource(const size_t& sizeInbytes);

	/// <summary>
	/// テクスチャリソースの生成
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	/// <summary>
	/// テクスチャデータの転送
	/// </summary>
	[[nodiscard]]
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	/// <summary>
	/// テクスチャファイルの読み込み
	/// </summary>
	DirectX::ScratchImage LoadTexture(const std::string& filePath);

private:
		// --- 内部関数 ---
	/// <summary>
	/// FPS固定用初期化
	/// </summary>
	void InitializeFixFPS();

	/// <summary>
	/// FPS固定用更新
	/// フレームレートを一定に保つための待機処理
	/// </summary>
	void UpdateFixFPS();

	// 記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

	// --- メンバ変数 ---
// WindowsAPI
	WinApp* winApp_ = nullptr;
	SrvUavManager* srvUavManager_ = nullptr;

	// DirectX12デバイス関連
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;

	// コマンド関連
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;

	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain = nullptr;
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> swapChainResources;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// 深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource = nullptr;

	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
	uint32_t descriptorSizeRTV;
	uint32_t descriptorSizeDSV;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap = nullptr;

	// RTVハンドル
	std::array<D3D12_CPU_DESCRIPTOR_HANDLE, kRTVHandleNum> rtvHandles;

	// RenderTextureResource
	Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResource = nullptr;
	D3D12_CLEAR_VALUE clearValue_;
	uint32_t offScreenSRVIndex = 0;
	uint32_t offScreenDepthSRVIndex = 0;

	// フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
	UINT16 fenceVal = 0;

	// ビューポート・シザー
	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};

	// DXCコンパイラ
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils = nullptr;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler = nullptr;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler = nullptr;

	Vector4 color_{ 0.1f,0.25f,0.5f,1.0f }; // 背景色 (初期値は水色)

public:
		// --- ゲッター ---
	ID3D12Device* GetDevice() { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); }
	size_t GetBackBufferCount() const { return swapChainDesc.BufferCount; }
	IDXGISwapChain4* GetSwapChain() { return swapChain.Get(); }
	ID3D12DescriptorHeap* GetDsvDescriptorHeap() { return dsvDescriptorHeap.Get(); }
	ID3D12CommandQueue* GetCommandQueue() { return commandQueue.Get(); }
	uint32_t GetOffScreenSRVIndex() const { return offScreenSRVIndex; }
};

} // namespace Engine
