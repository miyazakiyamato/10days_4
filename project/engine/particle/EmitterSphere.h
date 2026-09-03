#pragma once
#include "BaseParticleEmitter.h"

namespace Engine {

/// <summary>
/// 球形エミッタークラス
/// 指定された半径の球体範囲内でパーティクルを生成・管理する
/// </summary>
class EmitterSphere : public BaseParticleEmitter {
private:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
		// --- 構造体定義 ---
	/// <summary>
	/// エミッターのパラメータデータ (定数バッファ用)
	/// HLSLのアライメント(16バイト境界)に合わせる必要がある
	/// </summary>
	struct EmitterData {
		Vector3 translate;       // 位置
		float radius;            // 射出半径
		uint32_t count;          // 1回あたりの射出数
		float frequency;         // 射出間隔 (秒)
		float frequencyTime;     // 射出間隔計測タイマー
		uint32_t emit;           // 射出フラグ (1: 射出する)
		Vector4 startColor;      // 開始時の色
		Vector4 endColor;        // 終了時の色
		uint32_t isBillboard = 1;// ビルボード有効フラグ
		uint32_t isEmitUpdate = 0;// 毎フレーム射出するか (連続発生)
		float rlifeTimeMin;      // ランダム寿命の最小値
		float rlifeTimeMax;      // ランダム寿命の最大値
		Vector3 startScale;      // 開始時のスケール
		float pad0;              // パディング
		Vector3 endScale;        // 終了時のスケール
		float pad1;              // パディング
		Vector3 rVelocityMin;    // ランダム速度の最小値
		float pad2;              // パディング
		Vector3 rVelocityMax;    // ランダム速度の最大値
		float pad3;              // パディング
		Vector2 startUvTranslate;// UVスクロール開始位置
		Vector2 endUvTranslate;  // UVスクロール終了位置
	};

	/// <summary>
	/// フレームごとの時間情報 (定数バッファ用)
	/// </summary>
	struct PerFrame {
		float time;      // 全体時間
		float deltaTime; // 1フレームの経過時間
		float pad[2];    // パディング
	};

		// --- メンバ関数 ---
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="emitterName">エミッター名</param>
	/// <param name="kMaxParticles">最大パーティクル数</param>
	void Initialize(const std::string& emitterName, uint32_t kMaxParticles) override;

	/// <summary>
	/// 更新処理
	/// パラメータの更新やコンピュートシェーダーのディスパッチを行う
	/// </summary>
	void Update() override;

	/// <summary>
	/// 射出処理
	/// パーティクル生成のコマンドを発行する
	/// </summary>
	void Emit() override;

	/// <summary>
	/// 調整項目の初期化
	/// </summary>
	void InitializeGlobalVariables() override;

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables() override;

private:
		// --- メンバ変数 ---
	// エミッターのデータを格納するリソース (Constant Buffer / Structured Buffer)
	ComPtr<ID3D12Resource> emitterDataResource_;
	EmitterData* emitterData_ = nullptr; // マッピングされたデータへのポインタ

	std::string computeShaderPipelineName_ = ""; // 使用するCSパイプライン名

public:
		// --- ゲッター ---
	const Vector3& GetTranslate() const { return emitterData_->translate; }
	
	float GetRadius() const { return emitterData_->radius; }
	const uint32_t& GetCount() const { return emitterData_->count; }
	float GetFrequency() const { return emitterData_->frequency; }
	bool GetIsBillboard() const { return (bool)emitterData_->isBillboard; }
	bool GetIsEmitUpdate() const { return (bool)emitterData_->isEmitUpdate; }

		// --- セッター ---
	void SetTranslate(const Vector3& translate) { emitterData_->translate = translate; }
	void SetRadius(float radius) { emitterData_->radius = radius; }
	void SetCount(uint32_t count) { emitterData_->count = count; }
	void SetFrequency(float frequency) { emitterData_->frequency = frequency; }
	void SetIsBillboard(bool isBillboard) { emitterData_->isBillboard = isBillboard; }
	void SetIsEmitUpdate(bool isEmitUpdate);
	void SetRLifeTimeMin(float rlifeTimeMin) { emitterData_->rlifeTimeMin = rlifeTimeMin; }
	void SetRLifeTimeMax(float rlifeTimeMax) { emitterData_->rlifeTimeMax = rlifeTimeMax; }
	void SetRVelocityMin(const Vector3& rVelocityMin) { emitterData_->rVelocityMin = rVelocityMin; }
	void SetRVelocityMax(const Vector3& rVelocityMax) { emitterData_->rVelocityMax = rVelocityMax; }
};

} // namespace Engine
