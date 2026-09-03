#pragma once
#include <Vector3.h>
#include "Collision.h"

namespace Engine {

/// <summary>
/// 加速フィールド
/// </summary>
class AccelerationField{
public:

private:
	Vector3 acceleration_ = {15.0f,0.0f,0.0f};
	AABB area_{ .min{-1.0f,-1.0f,-1.0f},.max{1.0f,1.0f,1.0f} };
public:
	Vector3 GetAcceleration() { return acceleration_; }
	AABB GetAABB() { return area_; }
};


} // namespace Engine
