#pragma once
#include <memory>
#include <map>
#include <d3d12.h>
#include <wrl.h>
#include "Matrix4x4.h"

namespace Engine {

class DirectXCommon;
class SrvUavManager;

/// <summary>
/// ライトマネージャクラス（シングルトン）
/// 平行光源、点光源、スポットライトのデータを管理・GPUへ転送する
/// </summary>
class LightManager {
private:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
		// --- 構造体定義 ---
	/// <summary>
	/// 平行光源（Directional Light）構造体
	/// 太陽のように無限遠からの光をシミュレート
	/// </summary>
	struct DirectionalLight {
		Vector4 color;       //!< ライトの色
		Vector3 direction;   //!< ライトの向き
		float intensity;     //!< 輝度
		int isBlinnPhong;    //!< BlinnPhong反射モデルを使用するか(1:true)
		int pointLightCount = 0; // アクティブな点光源の数
		int spotLightCount = 0;  // アクティブなスポットライトの数
		float padding;
	};

	/// <summary>
	/// 点光源（Point Light）構造体
	/// 全方位に光を放射する光源（電球など）
	/// </summary>
	struct PointLight {
		Vector4 color;    //!< ライトの色
		Vector3 position; //!< ライトの場所
		float intensity;  //!< 輝度
		float radius;     //!< ライトの届く最大距離
		float decay;      //!< 減衰率
		float padding[2];
	};

	/// <summary>
	/// スポットライト（Spot Light）構造体
	/// 特定の方向に円錐状に光を放射する光源（懐中電灯など）
	/// </summary>
	struct SpotLight {
		Vector4 color;       //!< ライトの色
		Vector3 position;    //!< ライトの場所
		float intensity;     //!< 輝度
		Vector3 direction;   //!< ライトの向き
		float distance;      //!< ライトの届く最大距離
		float decay;         //!< 減衰率
		float cosAngle;      //!< スポットライトの照射角の余弦
		float cosFalloffStart; //!< Falloff（減衰）の開始角度
		float padding;
	};

		// --- メンバ関数 ---
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	static LightManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// 定数バッファやStructuredBufferの作成を行う
	/// </summary>
	/// <param name="dxCommon">DirectX共通基盤</param>
	/// <param name="srvUavManager">SRV/UAVマネージャ</param>
	void Initialize(DirectXCommon* dxCommon, SrvUavManager* srvUavManager);

	/// <summary>
	/// 描画時処理（コマンド積込）
	/// ルートパラメータへの設定などはここで行う想定
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGuiでの編集用更新
	/// 光源パラメータをGUIで調整する
	/// </summary>
	void ImGuiUpdate();

private:
	// 外部からは絶対にインスタンス化できない「鍵」となる構造体を定義
	struct PrivateToken {};
public:
	// シングルトン(make_uniqueする用)
	explicit LightManager(PrivateToken) {}
	~LightManager() = default;
	// コピー・ムーブの禁止
	LightManager(const LightManager&) = delete;
	LightManager& operator=(const LightManager&) = delete;

private:
		// --- シングルトン ---
	static std::unique_ptr<LightManager> instance;

		// --- メンバ変数 ---
	// ポインタ
	DirectXCommon* dxCommon_ = nullptr;
	SrvUavManager* srvUavManager_ = nullptr;

	// ライトデータ
	// DirectionalLight (ConstantBuffer想定)
	ComPtr<ID3D12Resource> directionalLightResource_;
	DirectionalLight* directionalLightData_ = nullptr;

	// PointLight (StructuredBuffer想定)
	ComPtr<ID3D12Resource> pointLightResource_;
	PointLight* pointLightData_ = nullptr;
	uint32_t srvIndexForPointLight = 0;
	uint32_t kMaxPointLight = 10;

	// SpotLight (StructuredBuffer想定)
	ComPtr<ID3D12Resource> spotLightResource_;
	SpotLight* spotLightData_;
	uint32_t srvIndexForSpotLight = 0;
	uint32_t kMaxSpotLight = 10;

public:
		// --- ゲッター ---
	// 平行光源取得
	DirectionalLight* GetDirectionalLight() const { return directionalLightData_; }
	// 点光源取得
	PointLight* GetPointLight() const { return pointLightData_; }
	uint32_t GetPointLightCount() const { return directionalLightData_->pointLightCount; }
	// スポットライト取得
	SpotLight* GetSpotLight() const { return spotLightData_; }
	uint32_t GetSpotLightCount() const { return directionalLightData_->spotLightCount; }

		// --- セッター ---
	/// <summary>
	/// 平行光源データを設定する
	/// </summary>
	void SetDirectionalLight(const DirectionalLight& directionalLight);

	/// <summary>
	/// 指定インデックスの点光源データを設定する
	/// </summary>
	void SetPointLight(uint32_t index, const PointLight& pointLight);

	/// <summary>
	/// 指定インデックスのスポットライトデータを設定する
	/// </summary>
	void SetSpotLight(uint32_t index, const SpotLight& spotLight);
};

} // namespace Engine
