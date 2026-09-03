#pragma once
#include <span>
#include "Model.h"

namespace Engine {

class SrvUavManager;
class Skeleton;

/// <summary>
/// スキンクラスタクラス
/// スケルトンの姿勢に基づいてメッシュの頂点を変形（スキニング）させる機能を管理する
/// GPUスキニングのためのリソース管理を行う
/// </summary>
class SkinCluster {
private:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
		// --- 構造体 ---
	static const uint32_t kNumMaxInfluence = 4; // 1頂点あたりの最大影響ボーン数

	/// <summary>
	/// 頂点ごとの影響度データ（CPU側）
	/// </summary>
	struct VertexInfluence {
		std::array<float, kNumMaxInfluence> weights;      // ウェイト
		std::array<int32_t, kNumMaxInfluence> jointIndices; // ジョイント番号
	};

	/// <summary>
	/// GPUに送るMatrixPalette（ボーン行列）の1要素
	/// </summary>
	struct WellForGPU {
		Matrix4x4 skeletonSpaceMatrix;                 // 位置変換用
		Matrix4x4 skeletonSpaceInverseTransposeMatrix; // 法線変換用
	};

	struct VertexForGPU {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct SkinningInformationForGPU {
		uint32_t numVertices;
	};

	/// <summary>
	/// スキンクラスタのデータセット（メッシュごと）
	/// </summary>
	struct SkinClusterData {
		// Influence (ウェイト情報)
		Microsoft::WRL::ComPtr<ID3D12Resource> influenceResource;
		std::span<VertexInfluence> mappedInfluence;
		uint32_t influenceSrvIndex = 0;

		// 元頂点 (Input: Bind Pose)
		Microsoft::WRL::ComPtr<ID3D12Resource> inputVerticesResource;
		uint32_t inputVertexSrvIndex = 0;

		// スキニング後頂点 (Output: Vertex Shaderへ渡すデータ)
		Microsoft::WRL::ComPtr<ID3D12Resource> skinnedVerticesResource;
		D3D12_VERTEX_BUFFER_VIEW skinnedVerticesView;
		uint32_t outputVertexUavIndex = 0;

		// 定数バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> skinningInformationResource;
		SkinningInformationForGPU* mappedSkinningInformation = nullptr;
	};

		// --- メンバ関数 ---
	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// スキンクラスタを生成する
	/// モデルデータとスケルトンを紐づけ、リソースを確保する
	/// </summary>
	void CreateSkinCluster(Skeleton* skeleton, std::vector<Model::Mesh> modelData);

	/// <summary>
	/// 更新処理
	/// 現在のスケルトンの姿勢からマトリックスパレットを計算・転送する
	/// </summary>
	void Update(Skeleton* skeleton);

	/// <summary>
	/// スキニング計算（Compute Shaderのディスパッチなど）を実行する
	/// </summary>
	void Draw(size_t meshIndex);

private://メンバ変数
	SrvUavManager* srvUavManager_ = nullptr; // SRV/UAVマネージャ

	std::vector<SkinClusterData> skinClusterDates_;

	std::vector<Matrix4x4> inverseBindPoseMatrices_; // 逆バインドポーズ行列

	// マトリックスパレット（全ジョイントの現在姿勢行列）
	ComPtr<ID3D12Resource> paletteResource_;
	std::span<WellForGPU> mappedPalette_;
	uint32_t paletteSrvIndex_ = 0;

public:
		// --- ゲッター ---
	/// <summary>
	/// スキニング後の頂点バッファビューを取得する
	/// </summary>
	const D3D12_VERTEX_BUFFER_VIEW* GetSkinnedVerticesView(size_t meshIndex) const {return &skinClusterDates_[meshIndex].skinnedVerticesView;}
};

} // namespace Engine
