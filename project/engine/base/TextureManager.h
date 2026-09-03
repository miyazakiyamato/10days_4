#pragma once
#include <string>
#include <wrl.h>
#include <d3d12.h>
#include <unordered_map>
#include <memory>
#include "DirectXTex.h"

namespace Engine {

class DirectXCommon;
class SrvUavManager;

/// <summary>
/// テクスチャマネージャクラス
/// テクスチャの読み込み、保持、デスクリプタの管理を行う
/// </summary>
class TextureManager {
public:
		// --- 構造体定義 ---
	/// <summary>
	/// テクスチャ一枚分のデータ
	/// </summary>
	struct TextureData {
		DirectX::TexMetadata metadata;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;
		uint32_t srvIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

		// --- メンバ関数 ---
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	static TextureManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, SrvUavManager* srvUavManager);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// テクスチャファイルの読み込み
	/// </summary>
	/// <param name="filePath">読み込むテクスチャのパス</param>
	void LoadTexture(const std::string& filePath);

	/// <summary>
	/// テクスチャパスからGPUハンドルを取得
	/// </summary>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(const std::string& filePath);

	/// <summary>
	/// メタデータの取得
	/// </summary>
	const DirectX::TexMetadata& GetMetaData(const std::string& filePath);

	/// <summary>
	/// SRVインデックスの取得
	/// </summary>
	uint32_t GetSrvIndex(const std::string& filePath);

	/// <summary>
	/// 読み込み済みテクスチャのキー一覧を取得
	/// </summary>
	std::vector<std::string> GetKeys();

private:
	// 外部からは絶対にインスタンス化できない「鍵」となる構造体を定義
	struct PrivateToken {};
public:
	// シングルトン(make_uniqueする用)
	explicit TextureManager(PrivateToken) {}
	~TextureManager() = default;
	// コピー・ムーブの禁止
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

private:
		// --- シングルトン ---
	static std::unique_ptr<TextureManager> instance;

		// --- メンバ変数 ---
	DirectXCommon* dxCommon_ = nullptr;
	SrvUavManager* srvUavManager_ = nullptr;

	// テクスチャディレクトリパス
	std::string directoryPath_ = "resources/texture/";

	// テクスチャデータコンテナ
	std::unordered_map<std::string, TextureData> textureData_;
};

} // namespace Engine
