#pragma once
#include "Vector3.h"
#include <vector>

namespace Engine {

/// <summary>
/// スプライン曲線(レール)
/// </summary>
class SplineRail {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="points">制御点</param>
	void Initialize(const std::vector<Vector3>& points);

	/// <summary>
	/// パラメータt(0.0~1.0)に対応する座標を取得
	/// </summary>
	/// <param name="t">0.0f ~ 1.0f</param>
	/// <returns>座標</returns>
	Vector3 GetPosition(float t) const;

	/// <summary>
	/// 全長を取得
	/// </summary>
	/// <returns>長さ</returns>
	float GetLength() const { return length_; }

private:
	// 制御点
	std::vector<Vector3> controlPoints_;
	// 描画・計算用の中間点
	std::vector<Vector3> pointsDrawing_;
	// 全長
	float length_ = 0.0f;
	// セグメント数(精度)
	size_t segmentCount_ = 100;
};

} // namespace Engine
