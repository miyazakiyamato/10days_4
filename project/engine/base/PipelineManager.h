#pragma once
#include "DirectXCommon.h"
#include "BlendMode.h"
#include "BasePipeline.h"
#include <map>
#include <memory>

namespace Engine {

	// --- 列挙型定義 ---
/// <summary>
/// カリングモード
/// </summary>
enum class CullMode {
	kNone,      //!< カリングなし
	kFront,     //!< フロントカリング
	kBack,      //!< バックカリング
};

/// <summary>
/// 塗りつぶしモード
/// </summary>
enum class FillMode {
	kSolid,     //!< ソリッド描画
	kWireframe, //!< ワイヤーフレーム描画
};

/// <summary>
/// 深度テストモード
/// </summary>
enum class DepthMode {
	kEnable,    //!< 深度テストを有効にする
	kDisable,   //!< 深度テストを無効にする
	kReadOnly,  //!< 深度テストを読み取り専用にする
};

/// <summary>
/// プリミティブトポロジタイプ
/// </summary>
enum class PrimitiveTopologyType {
	kTriangleList, //!< 三角形リスト
	kLineList,     //!< ラインリスト
	kPointList,    //!< ポイントリスト
};

/// <summary>
/// パイプラインステート設定構造体
/// パイプライン作成に必要なパラメータをまとめたもの
/// </summary>
struct PipelineState {
	std::string shaderName = ""; // シェーダーファイル名基底
	BlendMode blendMode = BlendMode::kNone; // ブレンドモード
	CullMode cullMode = CullMode::kBack; // カリングモード
	FillMode fillMode = FillMode::kSolid; // 塗りつぶしモード
	DepthMode depthMode = DepthMode::kEnable; // 深度テストを有効にするか
	StaticSamplersMode staticSamplersMode = StaticSamplersMode::kwrap; // サンプラーのモード
	PrimitiveTopologyType primitiveTopologyType = PrimitiveTopologyType::kTriangleList; // プリミティブトポロジのタイプ
	bool isOffScreen = false; // オフスクリーン描画かどうか
};

/// <summary>
/// パイプラインマネージャクラス
/// グラフィックスおよびコンピュートパイプラインの生成、管理、切り替えを行う
/// </summary>
class PipelineManager {
private:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
		// --- 構造体定義 ---
	struct GraphicsPipelineData {
		PipelineState state;
		ComPtr<ID3D12RootSignature> rootSignature = nullptr;
		ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	};
	struct ComputePipelineData {
		std::string shaderName = "";
		ComPtr<ID3D12RootSignature> rootSignature = nullptr;
		ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	};

		// --- メンバ関数 ---
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	static PipelineManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="dxCommon">DirectXCommonインスタンス</param>
	void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// グラフィックスパイプラインの描画設定適用
	/// </summary>
	/// <param name="stateName">適用するステート名</param>
	void DrawSetting(const std::string& stateName);

	/// <summary>
	/// コンピュートパイプラインの設定適用
	/// </summary>
	/// <param name="stateName">適用するステート名</param>
	void DrawSettingCS(const std::string& stateName);

	/// <summary>
	/// グラフィックスパイプラインの生成
	/// </summary>
	/// <param name="pipelineState">パイプライン設定</param>
	/// <returns>生成されたステート名</returns>
	std::string CreatePipelineState(const PipelineState& pipelineState);

	/// <summary>
	/// コンピュートパイプラインの生成
	/// </summary>
	/// <param name="shaderName">シェーダー名</param>
	/// <returns>生成されたステート名</returns>
	std::string CreateComputePipelineState(const std::string& shaderName);

private:
		// --- ローカル関数 ---
	/// <summary>
	/// グラフィックスルートシグネチャの作成
	/// </summary>
	void CreateGraphicsRootSignature(GraphicsPipelineData& pipeline);

	/// <summary>
	/// グラフィックスパイプラインステートの生成
	/// </summary>
	void CreateGraphicsPipeline(GraphicsPipelineData& pipeline);

	/// <summary>
	/// コンピュートルートシグネチャの作成
	/// </summary>
	void CreateComputeRootSignature(ComputePipelineData& pipeline);

	/// <summary>
	/// コンピュートパイプラインステートの生成
	/// </summary>
	void CreateComputePipeline(ComputePipelineData& pipeline);

private:
	// 外部からは絶対にインスタンス化できない「鍵」となる構造体を定義
	struct PrivateToken {};
public:
	// シングルトン(make_uniqueする用)
	explicit PipelineManager(PrivateToken) {}
	~PipelineManager() = default;
	// コピー・ムーブの禁止
	PipelineManager(const PipelineManager&) = delete;
	PipelineManager& operator=(const PipelineManager&) = delete;

private:
		// --- シングルトン ---
	static std::unique_ptr<PipelineManager> instance;

		// --- メンバ変数 ---
	DirectXCommon* dxCommon_ = nullptr;

	// パイプラインデータコンテナ
	std::map<std::string, std::unique_ptr<GraphicsPipelineData>> graphicsPipelineDates_;
	std::map<std::string, std::unique_ptr<ComputePipelineData>> computePipelineDates_;

	std::string nowStateName_;

	// シェーダーファイルパス設定
	std::wstring shaderFilePath_ = L"resources/shaders/";
	std::wstring vsFilePath_ = L".VS.hlsl";
	std::wstring psFilePath_ = L".PS.hlsl";
	std::wstring csFilePath_ = L".CS.hlsl";

public:
		// --- ゲッター ---
	DirectXCommon* GetDxCommon() { return dxCommon_; }
};

} // namespace Engine
