#pragma once
#include <string>
#include <map>
#include <memory>
#include "DirectXCommon.h"

namespace Engine {

/// <summary>
/// タイムマネージャクラス
/// ゲーム内の時間経過、デルタタイム、タイマー機能を管理する
/// </summary>
class TimeManager {
private:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
		// --- 構造体定義 ---
	struct Timer {
		float kLimitTime = 0.5f; // 制限時間
		float timeCount = 0.0f;  // 現在のカウント
		bool isStart = false;    // 計測中フラグ
	};

	struct PerFrame {
		float time;      // ゲーム開始からの経過時間
		float deltaTime; // 1フレームの経過時間
	};

		// --- メンバ関数 ---
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	static TimeManager* GetInstance();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 初期化処理
	/// 時間管理用リソースを作成する
	/// </summary>
	void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// 更新処理
	/// 毎フレーム呼び出し、時間を進める
	/// </summary>
	void Update();

	/// <summary>
	/// 時間速度のリセット
	/// </summary>
	void TimeSpeedReset();

	/// <summary>
	/// デルタタイムの速度変更（スローモーション等）
	/// </summary>
	/// <param name="timeSpeed">時間の進行速度 (1.0fが通常)</param>
	/// <param name="kLimitTime">効果を持続させる時間</param>
	void SetDeltaTimeSpeedStart(float timeSpeed, float kLimitTime);

	// デルタタイム
	static inline float deltaTime_ = 1.0f / 60.0f;
	static inline const float kFlamTime_ = 1.0f / 60.0f;

private:
	// 外部からは絶対にインスタンス化できない「鍵」となる構造体を定義
	struct PrivateToken {};
public:
	// シングルトン(make_uniqueする用)
	explicit TimeManager(PrivateToken) {}
	~TimeManager() = default;
	// コピー・ムーブの禁止
	TimeManager(const TimeManager&) = delete;
	TimeManager& operator=(const TimeManager&) = delete;

private:
	// --- シングルトン ---
	static std::unique_ptr<TimeManager> instance;

	// --- メンバ変数 ---
	DirectXCommon* dxCommon_ = nullptr; // DirectXの共通インスタンス

	float timeSpeed_ = kFlamTime_;
	float kLimitTime_ = 0.5f;
	float timeCount_ = 0.0f;
	std::map<std::string, Timer> timers_;

	// timeのリソース
	ComPtr<ID3D12Resource> perFrameResource_;
	PerFrame* perFrameData_ = nullptr; // フレームごとの時間情報

public:
		// --- ゲッター ---
	Timer GetTimer(std::string name) { return timers_[name]; }

	/// <summary>
	/// 現在の時間を取得
	/// </summary>
	float GetTimeCurrent() const { return perFrameData_->time; }

	ID3D12Resource* GetPerFrameResource() const { return perFrameResource_.Get(); }

		// --- セッター ---
	/// <summary>
	/// タイマーの設定
	/// </summary>
	void SetTimer(std::string name, float kLimitTime);
};

} // namespace Engine
