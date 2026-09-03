#include "SplineRail.h"
#include <cmath>

namespace Engine {

void SplineRail::Initialize(const std::vector<Vector3>& points) {
	controlPoints_ = points;
	
	// 長さ計算
	if (controlPoints_.size() >= 2) {
		pointsDrawing_.clear();
		for (size_t i = 0; i < segmentCount_ + 1; i++) {
			float t = 1.0f / segmentCount_ * i;
			Vector3 pos = Vector3::CatmullRomPosition(controlPoints_, t);
			pointsDrawing_.push_back(pos);
		}
		length_ = 0.0f;
		for (size_t i = 0; i < pointsDrawing_.size() - 1; i++) {
			length_ += (pointsDrawing_.at(i) - pointsDrawing_.at(i + 1)).Length();
		}
	}
}

Vector3 SplineRail::GetPosition(float t) const {
	// 範囲外対策
	if (t < 0.0f) t = 0.0f;
	if (t > 1.0f) t = 1.0f;

	return Vector3::CatmullRomPosition(controlPoints_, t);
}

} // namespace Engine
