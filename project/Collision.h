#pragma once
#include "MyMtMatrix.h"
class Collision {
public:
	static bool IsCollision(const Sphere& s1, const Sphere& s2);
	static bool IsCollision(const Sphere& sphere, const Plane& plane);
	static bool IsCollision(const Line& line, const Plane& plane);
	static bool IsCollision(const Ray& ray, const Plane& plane);
	static bool IsCollision(const Segment& segment, const Plane& plane);
	static bool IsCollision(const Triangle& triangle, const Segment& segment);
	static bool IsCollision(const AABB& aabb1, const AABB& aabb2);
	static bool IsCollision(const AABB& aabb, const Sphere& sphere);
	static bool IsCollision(const AABB& aabb, const Segment& segment);
	static bool IsCollision(const AABB& aabb, const Ray& ray);
	static bool IsCollision(const AABB& aabb, const Line& line);
	static bool IsCollision(const OBB& obb, const Sphere& sphere);
	static bool IsCollision(const Segment& segment, const OBB& obb);
	static bool IsCollision(const Line& line, const OBB& obb);
};
