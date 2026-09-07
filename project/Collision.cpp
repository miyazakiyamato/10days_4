#include "Collision.h"
#include <algorithm>

bool Collision::IsCollision(const Sphere& s1, const Sphere& s2) {
	float distance = MyMtVector3::Length(MyMtVector3::Subtract(s2.center, s1.center));
	return (distance <= (s1.radius + s2.radius) ? true : false);
}
bool Collision::IsCollision(const Sphere& sphere, const Plane& plane) {
	float k = MyMtVector3::Dot(plane.normal, sphere.center) - plane.distance;
	k = k < 0 ? -k : k;
	if (k <= sphere.radius) {
		return true;
	}
	return false;
}
bool Collision::IsCollision(const Line& line, const Plane& plane) {
	float dot = MyMtVector3::Dot(plane.normal, line.diff);
	if (dot == 0.0f) {
		return false;
	}
	float t = (plane.distance - MyMtVector3::Dot(line.origin, plane.normal)) / dot;
	if (t >= -1.0f && t <= 2.0f) {
		return true;
	}
	return false;
}
bool Collision::IsCollision(const Ray& ray, const Plane& plane) {
	float dot = MyMtVector3::Dot(plane.normal, ray.diff);
	if (dot == 0.0f) {
		return false;
	}
	float t = (plane.distance - MyMtVector3::Dot(ray.origin, plane.normal)) / dot;
	if (t >= 0.0f && t <= 2.0f) {
		return true;
	}
	return false;
}
bool Collision::IsCollision(const Segment& segment, const Plane& plane) {
	float dot = MyMtVector3::Dot(plane.normal, segment.diff);
	if (dot == 0.0f) {
		return false;
	}
	float t = (plane.distance - MyMtVector3::Dot(segment.origin, plane.normal)) / dot;
	if (t >= 0.0f && t <= 1.0f) {
		return true;
	}
	return false;
}
bool Collision::IsCollision(const Triangle& triangle, const Segment& segment) {
	// ベクトルv1,v2を求める
	Vector3 v01 = MyMtVector3::Subtract(triangle.Vertices[1], triangle.Vertices[0]);
	Vector3 v12 = MyMtVector3::Subtract(triangle.Vertices[2], triangle.Vertices[1]);
	Plane plane;
	// 法線nを算出
	plane.normal = MyMtVector3::Normalize(MyMtVector3::Cross(v01, v12));
	// 距離を求める
	plane.distance = MyMtVector3::Dot(triangle.Vertices[0], plane.normal);

	if (IsCollision(segment, plane)) {
		float dot = MyMtVector3::Dot(plane.normal, segment.diff);
		float t = (plane.distance - MyMtVector3::Dot(segment.origin, plane.normal)) / dot;
		Vector3 segmentP = MyMtVector3::Add(segment.origin, MyMtVector3::Multiply(t, segment.diff));

		Vector3 cross01 = MyMtVector3::Cross(MyMtVector3::Subtract(triangle.Vertices[1], triangle.Vertices[0]), MyMtVector3::Subtract(segmentP, triangle.Vertices[1]));
		Vector3 cross12 = MyMtVector3::Cross(MyMtVector3::Subtract(triangle.Vertices[2], triangle.Vertices[1]), MyMtVector3::Subtract(segmentP, triangle.Vertices[2]));
		Vector3 cross20 = MyMtVector3::Cross(MyMtVector3::Subtract(triangle.Vertices[0], triangle.Vertices[2]), MyMtVector3::Subtract(segmentP, triangle.Vertices[0]));

		if (MyMtVector3::Dot(cross01, plane.normal) >= 0.0f && MyMtVector3::Dot(cross12, plane.normal) >= 0.0f && MyMtVector3::Dot(cross20, plane.normal) >= 0.0f) {
			return true;
		}
	}
	return false;
}
bool Collision::IsCollision(const AABB& aabb1, const AABB& aabb2) {
	if (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x && aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y && aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z) {
		return true;
	}
	return false;
}
bool Collision::IsCollision(const AABB& aabb, const Sphere& sphere) {
	Vector3 closestPoint = MyMtVector3::Clamp(sphere.center, aabb.min, aabb.max);
	float distance = MyMtVector3::Length(MyMtVector3::Subtract(closestPoint, sphere.center));
	if (distance <= sphere.radius) {
		return true;
	}
	return false;
}
bool Collision::IsCollision(const AABB& aabb, const Segment& segment) {
	Vector3 tMin = {(aabb.min.x - segment.origin.x) / segment.diff.x, (aabb.min.y - segment.origin.y) / segment.diff.y, (aabb.min.z - segment.origin.z) / segment.diff.z};
	Vector3 tMax = {(aabb.max.x - segment.origin.x) / segment.diff.x, (aabb.max.y - segment.origin.y) / segment.diff.y, (aabb.max.z - segment.origin.z) / segment.diff.z};
	Vector3 tNear = MyMtVector3::Min(tMin, tMax);
	Vector3 tFar = MyMtVector3::Max(tMin, tMax);
	float tmin = fmaxf(fmaxf(tNear.x, tNear.y), tNear.z);
	float tmax = fminf(fminf(tFar.x, tFar.y), tFar.z);
	if (tmin <= tmax) {
		if ((tmin >= 0 && tmin <= 1) || (tmax >= 0 && tmax <= 1) || tmin < 0 && tmax > 1) {
			return true;
		}
	}
	return false;
}
bool Collision::IsCollision(const AABB& aabb, const Ray& ray) {
	Vector3 tMin = {(aabb.min.x - ray.origin.x) / ray.diff.x, (aabb.min.y - ray.origin.y) / ray.diff.y, (aabb.min.z - ray.origin.z) / ray.diff.z};
	Vector3 tMax = {(aabb.max.x - ray.origin.x) / ray.diff.x, (aabb.max.y - ray.origin.y) / ray.diff.y, (aabb.max.z - ray.origin.z) / ray.diff.z};
	Vector3 tNear = MyMtVector3::Min(tMin, tMax);
	Vector3 tFar = MyMtVector3::Max(tMin, tMax);
	float tmin = fmaxf(fmaxf(tNear.x, tNear.y), tNear.z);
	float tmax = fminf(fminf(tFar.x, tFar.y), tFar.z);
	if (tmin <= tmax) {
		if ((tmin >= 0) || (tmax >= 0)) {
			return true;
		}
	}
	return false;
}
bool Collision::IsCollision(const AABB& aabb, const Line& line) {
	Vector3 tMin = {(aabb.min.x - line.origin.x) / line.diff.x, (aabb.min.y - line.origin.y) / line.diff.y, (aabb.min.z - line.origin.z) / line.diff.z};
	Vector3 tMax = {(aabb.max.x - line.origin.x) / line.diff.x, (aabb.max.y - line.origin.y) / line.diff.y, (aabb.max.z - line.origin.z) / line.diff.z};
	Vector3 tNear = MyMtVector3::Min(tMin, tMax);
	Vector3 tFar = MyMtVector3::Max(tMin, tMax);
	float tmin = fmaxf(fmaxf(tNear.x, tNear.y), tNear.z);
	float tmax = fminf(fminf(tFar.x, tFar.y), tFar.z);
	if (tmin <= tmax) {
		return true;
	}
	return false;
}
bool Collision::IsCollision(const OBB& obb, const Sphere& sphere) {
	Matrix4x4 obbWorldMatrix{obb.orientations[0].x, obb.orientations[1].x, obb.orientations[2].x, 0, obb.orientations[0].y, obb.orientations[1].y, obb.orientations[2].y, 0,
	                         obb.orientations[0].z, obb.orientations[1].z, obb.orientations[2].z, 0, obb.center.x,          obb.center.y,          obb.center.z,          1};
	Vector3 centerInObbLocalSpace = MyMtMatrix::Transform(sphere.center, MyMtMatrix::Inverse(obbWorldMatrix));
	AABB aabbOBBLocal{.min = MyMtVector3::Multiply(-1.0, obb.size), .max = obb.size};
	Sphere sphereOBBLocal{centerInObbLocalSpace, sphere.radius};
	if (IsCollision(aabbOBBLocal, sphereOBBLocal)) {
		return true;
	}
	return false;
}
bool Collision::IsCollision(const Segment& segment, const OBB& obb) {
	Matrix4x4 obbWorldMatrix{obb.orientations[0].x, obb.orientations[1].x, obb.orientations[2].x, 0, obb.orientations[0].y, obb.orientations[1].y, obb.orientations[2].y, 0,
	                         obb.orientations[0].z, obb.orientations[1].z, obb.orientations[2].z, 0, obb.center.x,          obb.center.y,          obb.center.z,          1};
	Vector3 localOrigin = MyMtMatrix::Transform(segment.origin, MyMtMatrix::Inverse(obbWorldMatrix));
	Vector3 localEnd = MyMtMatrix::Transform(MyMtVector3::Add(segment.origin, segment.diff), MyMtMatrix::Inverse(obbWorldMatrix));

	AABB localAABB{.min = MyMtVector3::Multiply(-1.0, obb.size), .max = obb.size};

	Segment localSegment;

	localSegment.origin = localOrigin;
	localSegment.diff = MyMtVector3::Subtract(localEnd, localOrigin);

	if (IsCollision(localAABB, localSegment)) {
		return true;
	}
	return false;
}
bool Collision::IsCollision(const Line& line, const OBB& obb) {
	Matrix4x4 obbWorldMatrix{obb.orientations[0].x, obb.orientations[1].x, obb.orientations[2].x, 0, obb.orientations[0].y, obb.orientations[1].y, obb.orientations[2].y, 0,
	                         obb.orientations[0].z, obb.orientations[1].z, obb.orientations[2].z, 0, obb.center.x,          obb.center.y,          obb.center.z,          1};
	Vector3 localOrigin = MyMtMatrix::Transform(line.origin, MyMtMatrix::Inverse(obbWorldMatrix));
	Vector3 localEnd = MyMtMatrix::Transform(MyMtVector3::Add(line.origin, line.diff), MyMtMatrix::Inverse(obbWorldMatrix));

	AABB localAABB{.min = MyMtVector3::Multiply(-1.0, obb.size), .max = obb.size};

	Line localLine;

	localLine.origin = localOrigin;
	localLine.diff = MyMtVector3::Subtract(localEnd, localOrigin);

	return IsCollision(localAABB, localLine);
}
