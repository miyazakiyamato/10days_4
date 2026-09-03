#pragma once
#include <map>
#include <memory>
#include "BlendMode.h"
#include "random"
#include "DirectXCommon.h"
#include "Matrix4x4.h"

namespace Engine {

class DirectXCommon;
class SrvUavManager;

/// <summary>
/// パーティクルマネージャクラス
/// GPUパーティクルのリソース管理、生成、描画を一括で行うシングルトン
/// </summary>
class ParticleManager {
private:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
		// --- 構造体定義 ---
	/// <summary>
	/// 頂点データ構造体 (描画用)
	/// </summary>
	struct VertexData {
		Vector4 position; // 座標
		Vector2 texcoord; // UV座標
	};

	/// <summary>
	/// マテリアルデータ
	/// </summary>
	struct MaterialData {
		std::string textureFilePath = "circle.dds"; // テクスチャパス
		uint32_t srvIndex; // SRVインデックス
	};

	/// <summary>
	/// パーティクルデータ構造体 (シミュレーション用)
	/// GPU上のStructuredBufferで扱われる個々のパーティクル情報
	/// </summary>
	struct Particle {
		uint32_t isBillboard;    // ビルボード有効フラグ
		Vector3 translate;       // 現在の位置
		Vector3 scale;           // 現在のスケール
		float lifeTime;          // 寿命 (秒)
		Vector3 velocity;        // 速度
		float currentTime;       // 経過時間
		Vector4 color;           // 現在の色
		Vector2 uvTranslate;     // UVスクロール位置
		Vector2 uvScale;         // UVスケール
		Vector3 startScale;      // 開始時のスケール
		Vector3 endScale;        // 終了時のスケール
		Vector4 startColor;      // 開始色
		Vector4 endColor;        // 終了色
		Vector2 startUvTranslate;// 開始UV位置
		Vector2 endUvTranslate;  // 終了UV位置
	};

	/// <summary>
	/// ビュー・プロジェクション情報 (定数バッファ)
	/// </summary>
	struct PerView {
		Matrix4x4 viewProjection;  // ビュープロジェクション行列
		Matrix4x4 billboardMatrix; // ビルボード行列
	};

	/// <summary>
	/// パーティクル数制限情報
	/// </summary>
	struct Limit {
		uint32_t kMaxParticles = 1000; // 最大数
		float pad[3];
	};

	/// <summary>
	/// パーティクルグループ
	/// 1種類のエミッターに対応するリソース群をまとめた構造体
	/// </summary>
	struct ParticleGroup {
		MaterialData materialData;

		// パーティクル本体のデータ
		ComPtr<ID3D12Resource> particleResource;
		uint32_t particleSrvIndex; // SRV (描画用)
		uint32_t particleUavIndex; // UAV (CS計算用)

		// フリーリストインデックス (使用可能なインデックスのスタックポインタ等)
		ComPtr<ID3D12Resource> freeListIndexResource;
		uint32_t freeListIndexUAVIndex;

		// フリーリスト (使用可能なインデックス一覧)
		ComPtr<ID3D12Resource> freeListResource;
		uint32_t freeListUAVIndex;

		// 最大数情報
		ComPtr<ID3D12Resource> maxParticlesResource;
		Limit* limit = nullptr;

		// メッシュ頂点データ
		uint32_t kParticleVertexNum;
		uint32_t kParticleIndexNum;
		ComPtr<ID3D12Resource> vertexResource;
		ComPtr<ID3D12Resource> indexResource;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;
		VertexData* vertexData = nullptr;
		uint32_t* indexData = nullptr;

		// 描画ステート
		BlendMode blendMode_ = BlendMode::kAdd; // ブレンドモード
		std::string pipelineStateName = "";     // パイプライン名
		Vector2 textureLeftTop_ = { 0.0f, 0.0f };
		Vector2 textureSize_ = { 100.0f, 100.0f };
	};

		// --- メンバ関数 ---
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	static ParticleManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// 共通リソースの作成やパイプラインの準備を行う
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, SrvUavManager* srvUavManager);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 更新処理
	/// 全パーティクルグループのシミュレーションCSを実行する
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// 全パーティクルグループを描画コマンドリストに積む
	/// </summary>
	void Draw();

	/// <summary>
	/// パーティクルグループの生成
	/// GPUリソースを確保し、管理マップに追加する
	/// </summary>
	/// <param name="name">グループ識別名</param>
	/// <param name="kMaxParticles">このグループの最大パーティクル数</param>
	void CreateParticleGroup(const std::string name, uint32_t kMaxParticles);

	/// <summary>
	/// パーティクルグループの削除
	/// </summary>
	void ClearParticleGroup(const std::string name);

private:
		// --- ローカル関数 ---
	void CreateParticle(ParticleGroup* group);
	void CreatePlane(ParticleGroup* group);
	void CreateRing(ParticleGroup* group, const uint32_t& kDivide, float kOuterRadius, float kInnerRadius);
	void CreateCylinder(ParticleGroup* group, const uint32_t& kDivide, float kTopRadius, float kBottomRadius, float kHeight);

	// 調整項目の初期化
	void InitializeGlobalVariables();
	// 調整項目の適用
	void ApplyGlobalVariables();

private:
	// 外部からは絶対にインスタンス化できない「鍵」となる構造体を定義
	struct PrivateToken {};
public:
	// シングルトン(make_uniqueする用)
	explicit ParticleManager(PrivateToken) {}
	~ParticleManager() = default;
	// コピー・ムーブの禁止
	ParticleManager(const ParticleManager&) = delete;
	ParticleManager& operator=(const ParticleManager&) = delete;

private:
		// --- シングルトン ---
	static std::unique_ptr<ParticleManager> instance;

		// --- メンバ変数 ---
	DirectXCommon* dxCommon_ = nullptr;
	SrvUavManager* srvUavManager_ = nullptr;

	// WorldViewProjection用の共通リソース
	ComPtr<ID3D12Resource> perViewResource_; // PerView定数バッファ
	PerView* perViewData_ = nullptr;         // マッピング用ポインタ

	// パイプライン名
	std::string initCSPipelineName_ = "";    // 初期化CS
	std::string updateCSPipelineName_ = "";  // 更新CS

	// ランダムエンジン
	std::mt19937 randomEngine_;

	// ImGui用バッファ
	std::string groupNameText = "";
	char buffer[128] = "";
	std::string typeNameText = "";
	char buffer2[128] = "";

	// パーティクルグループの管理マップ
	std::map<std::string, std::unique_ptr<ParticleGroup>> particleGroups;

public:
		// --- ゲッター ---
	DirectXCommon* GetDirectXCommon() { return dxCommon_; }
	SrvUavManager* GetSrvUavManager() { return srvUavManager_; }
	std::map<std::string, std::unique_ptr<ParticleGroup>>& GetParticleGroups() { return particleGroups; }
	ParticleGroup* GetParticleGroup(std::string name);
	const BlendMode& GetBlendMode(std::string name) { return particleGroups[name]->blendMode_; }

		// --- セッター ---
	void SetBlendMode(std::string name, BlendMode blendMode);
	void SetTexture(std::string name, std::string textureName);

	// 形状設定用セッター
	void SetRing(std::string name, const uint32_t& kDivide, float kOuterRadius, float kInnerRadius);
	void SetCylinder(std::string name, const uint32_t& kDivide, float kTopRadius, float kBottomRadius, float kHeight);
};

} // namespace Engine
