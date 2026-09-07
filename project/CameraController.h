#pragma once
#include "ViewProjection.h"

class Player;
/// <summary>
/// カメラコントローラー
/// </summary>
class CameraController {
public:
	// 矩形
	struct Rect {
		float left = 0.0f;   // 左端
		float right = 1.0f;  // 右端
		float bottom = 0.0f; // 下端
		float top = 1.0f;    // 上端
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ViewProjection* viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void Reset();

private:
	//ビュープロジェクション
	ViewProjection* viewProjection_;

	//
	Player* target_ = nullptr;

	//
	Vector3 targetOffset_ = {17.0f,9,-25.0f};
	//カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};
	//カメラの目標座標
	Vector3 targetPosition_;
	//座標補完率
	static inline const float kInterpolationRate = 0.1f;
	//速度掛け算
	static inline const float kVelocityBias = 20.0f;
	//追従対象の各方向へのカメラ移動範囲
	static inline const Rect margin = {-15, 15, -10, 10};

public:
	void SetTarget(Player* target) { target_ = target; }
	void SetMovableArea(Rect area) { movableArea_ = area; }
};
