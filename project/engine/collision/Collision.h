#pragma once
#include <iostream>
#include "Vector3.h"
#include "Matrix4x4.h"

namespace Engine {

/// <summary>
/// 直線 (無限長)
/// </summary>
struct Line {
	Vector3 origin; // 通過点
	Vector3 diff;   // 方向ベクトル
};

/// <summary>
/// 半直線 (始点から一方向に無限)
/// </summary>
struct Ray {
	Vector3 origin; // 始点
	Vector3 diff;   // 方向ベクトル
};

/// <summary>
/// 線分 (始点と終点がある)
/// </summary>
struct Segment {
	Vector3 origin; // 始点
	Vector3 diff;   // 終点へのベクトル (origin + diff = 終点)
};

/// <summary>
/// 三角形
/// </summary>
struct Triangle {
	Vector3 Vertices[3]; // 3つの頂点
};

/// <summary>
/// 球体
/// </summary>
struct Sphere {
	Vector3 center; // 中心点
	float radius;   // 半径
};

/// <summary>
/// 平面
/// </summary>
struct Plane {
	Vector3 normal; // 法線ベクトル
	float distance; // 原点からの距離
};

/// <summary>
/// 軸平行バウンディングボックス (Axis Aligned Bounding Box)
/// </summary>
struct AABB {
	Vector3 min; // 最小点
	Vector3 max; // 最大点
};

/// <summary>
/// 有向バウンディングボックス (Oriented Bounding Box)
/// </summary>
struct OBB {
	Vector3 center;         // 中心点
	Vector3 orientations[3];// 3軸の方向ベクトル（正規化済み）
	Vector3 size;           // 各軸方向の半分の長さ（半径）

	/// <summary>
	/// 回転角から各軸の方向ベクトルを設定する
	/// </summary>
	/// <param name="rotate">回転角（ラジアン）</param>
	void MakeOBBRotate(const Vector3& rotate);
};

/// <summary>
/// 衝突判定クラス
/// 各種形状同士の交差判定関数を提供する静的クラス
/// </summary>
class Collision {
public:
	/// <summary>
	/// 球と球の衝突判定
	/// </summary>
	static bool IsCollision(const Sphere& s1, const Sphere& s2);

	/// <summary>
	/// 球と平面の衝突判定
	/// </summary>
	static bool IsCollision(const Sphere& sphere, const Plane& plane);

	/// <summary>
	/// 直線と平面の衝突判定
	/// </summary>
	static bool IsCollision(const Line& line, const Plane& plane);

	/// <summary>
	/// 半直線と平面の衝突判定
	/// </summary>
	static bool IsCollision(const Ray& ray, const Plane& plane);

	/// <summary>
	/// 線分と平面の衝突判定
	/// </summary>
	static bool IsCollision(const Segment& segment, const Plane& plane);

	/// <summary>
	/// 三角形と線分の衝突判定
	/// </summary>
	static bool IsCollision(const Triangle& triangle, const Segment& segment);

	/// <summary>
	/// AABBと点の包含判定
	/// </summary>
	static bool IsCollision(const AABB& aabb, const Vector3 point);

	/// <summary>
	/// AABB同士の衝突判定
	/// </summary>
	static bool IsCollision(const AABB& aabb1, const AABB& aabb2);

	/// <summary>
	/// AABBと球の衝突判定
	/// </summary>
	static bool IsCollision(const AABB& aabb, const Sphere& sphere);

	/// <summary>
	/// AABBと線分の衝突判定
	/// </summary>
	static bool IsCollision(const AABB& aabb, const Segment& segment);

	/// <summary>
	/// AABBと半直線の衝突判定
	/// </summary>
	static bool IsCollision(const AABB& aabb, const Ray& ray);

	/// <summary>
	/// AABBと直線の衝突判定
	/// </summary>
	static bool IsCollision(const AABB& aabb, const Line& line);

	/// <summary>
	/// OBBと球の衝突判定
	/// </summary>
	static bool IsCollision(const OBB& obb, const Sphere& sphere);

	/// <summary>
	/// 線分とOBBの衝突判定
	/// </summary>
	static bool IsCollision(const Segment& segment, const OBB& obb);

	/// <summary>
	/// 直線とOBBの衝突判定
	/// </summary>
	static bool IsCollision(const Line& line, const OBB& obb);
};

} // namespace Engine
