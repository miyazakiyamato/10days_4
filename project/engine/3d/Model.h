#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <optional>
#include <map>
#include "DirectXCommon.h"
#include "BlendMode.h"
#include "Quaternion.h"

namespace Engine {

/// <summary>
/// 3Dモデルリソースクラス
/// メッシュデータ、階層構造(Node)、マテリアル情報などを保持する
/// </summary>
class Model {
private:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
		// --- 構造体 ---
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	struct EulerTransform {
		Vector3 scale;
		Vector3 rotate; // Eulerでの回転
		Vector3 translate;
	};
	struct QuaternionTransform {
		Vector3 scale;
		Quaternion rotate;
		Vector3 translate;
	};
	struct Node {
		QuaternionTransform transform;
		Matrix4x4 localMatrix;
		std::string name;
		std::vector<Node> children;
	};
	struct MeshMaterialData {
		// EulerTransform uvTransform;
		Vector4 color;
		std::string textureFilePath;
		Vector3 uvScale = { 1.0f,1.0f,1.0f };
	};
	struct VertexWeightData {
		float weight;
		uint32_t vertexIndex;
	};
	struct JointWeightData {
		Matrix4x4 inverseBindPoseMatrix; // 逆バインドポーズ行列（初期姿勢の逆行列）
		std::vector<VertexWeightData> vertexWeights;
	};

	/// <summary>
	/// メッシュデータ構造体
	/// 1つのモデルは複数のメッシュで構成される場合がある
	/// </summary>
	struct Mesh {
		std::vector<VertexData> vertices;
		std::vector<uint32_t> indices;
		MeshMaterialData material;
		std::map<std::string, JointWeightData> skinClusterData; // スキニング用データ

		// バッファリソース
		ComPtr<ID3D12Resource> vertexResource;
		ComPtr<ID3D12Resource> indexResource;

		// バッファリソース内のデータを指すポインタ
		VertexData* vertexData = nullptr;
		uint32_t* indexData = nullptr;

		// バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;
	};

		// --- メンバ関数 ---
	/// <summary>
	/// 初期化（ファイル読み込みとリソース生成）
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// 描画コマンドの発行
	/// </summary>
	/// <param name="meshIndex">描画するメッシュのインデックス</param>
	void Draw(size_t meshIndex);

	/// <summary>
	/// スキニング済み頂点バッファなど、外部のVBVを使用して描画する場合
	/// </summary>
	void Draw(size_t meshIndex, const D3D12_VERTEX_BUFFER_VIEW* vertexBufferView);

	/// <summary>
	/// Assimpを使用してファイルを読み込む
	/// </summary>
	void LoadFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// ノード階層を再帰的に読み込む
	/// </summary>
	Node ReadNode(aiNode* node);

	/// <summary>
	/// マテリアルカラーを読み込む
	/// </summary>
	void LoadColor(Mesh& mesh, aiMaterial* aiMeshMaterial);

private:
		// --- ローカル関数 ---
	/// <summary>
	/// 標準プリミティブ（球や立方体など）の生成処理が必要か判定する
	/// </summary>
	bool IsPrimitive(const std::string& filename);

	/// <summary>
	/// スカイボックス用のモデルデータを生成する
	/// </summary>
	void ModelDataSkybox();

		// --- メンバ変数 ---
	DirectXCommon* dxCommon_ = nullptr;

	// Obj/GLTFファイルの解析データ
	std::vector<Mesh> meshData_;
	size_t meshCount_ = 0;
	Node rootNode_;
	std::map<std::string, Vector3> userMtls_;
	void LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

public:
		// --- ゲッター ---
	const std::vector<Mesh>& GetMeshData() { return meshData_; }
	const Node& GetNode() { return rootNode_; }
};

} // namespace Engine
