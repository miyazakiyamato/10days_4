#pragma once
#include <random>

namespace Engine {

// 前方宣言
struct Line;
struct Ray;
struct Segment;
struct Quaternion;

/// <summary>
/// 3次元ベクトル
/// ワールド座標、速度、方向などで使用する
/// </summary>
struct Vector3 {
		// --- メンバ変数 ---
	float x;
	float y;
	float z;

		// --- メンバ関数 ---
	/// <summary>
	/// 長さ（ノルム）を返す
	/// </summary>
	float Length() const;

	/// <summary>
	/// 正規化（長さ1）したベクトルを返す
	/// </summary>
	Vector3 Normalize() const;

	/// <summary>
	/// 絶対値ベクトルを返す
	/// </summary>
	Vector3 Abs() const;

	/// <summary>
	/// 0.0～1.0の範囲にクランプする
	/// </summary>
	Vector3 Clamp01() const;

	/// <summary>
	/// -1.0～1.0の範囲にクランプする
	/// </summary>
	Vector3 Clamp_11() const;

	/// <summary>
	/// クォータニオンへ変換（オイラー角として扱う場合など）
	/// </summary>
	Quaternion ToQuaternion() const;

		// --- 静的関数 ---
	static Vector3 Add(const Vector3& v1, const Vector3& v2);
	static Vector3 Subtract(const Vector3& v1, const Vector3& v2);
	static Vector3 Multiply(float scalar, const Vector3& v);
	static Vector3 Divide(float scalar, const Vector3& v);

	static float Dot(const Vector3& v1, const Vector3& v2); // 内積
	static float Length(const Vector3& v);
	static Vector3 Normalize(const Vector3& v);
	static Vector3 Abs(const Vector3& v);

	static Vector3 Min(const Vector3& v, const Vector3& v2);
	static Vector3 Max(const Vector3& v, const Vector3& v2);
	static Vector3 Clamp(const Vector3& v, const Vector3& min, const Vector3& max);

	static Vector3 Cross(const Vector3& v1, const Vector3& v2); // 外積
	static Vector3 Project(const Vector3& v1, const Vector3& v2); // 射影

	/// <summary>
	/// 垂直なベクトルを取得
	/// </summary>
	static Vector3 Perpendicular(const Vector3& vector);

	/// <summary>
	/// 反射ベクトルを計算
	/// </summary>
	static Vector3 Reflect(const Vector3& input, const Vector3& normal);

	// --- 最近接点計算 ---
	static Vector3 ClosestPoint(const Vector3& point, const Line& line);
	static Vector3 ClosestPoint(const Vector3& point, const Ray& ray);
	static Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

	// --- 補間 ---
	/// <summary>
	/// 線形補間 (Lerp)
	/// </summary>
	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

	/// <summary>
	/// 球面線形補間 (Slerp)
	/// </summary>
	static Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

	/// <summary>
	/// Catmull-Romスプライン補間 (4点指定)
	/// </summary>
	static Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

	/// <summary>
	/// Catmull-Romスプラインによる座標取得 (点群指定)
	/// </summary>
	static Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t);

	/// <summary>
	/// ランダムなベクトルを生成
	/// </summary>
	static Vector3 Random(std::mt19937& randomEngine, const Vector3& vMin, const Vector3& vMax);

		// --- 演算子のオーバーロード ---
	Vector3 operator-() const;
	Vector3 operator+(const Vector3& v2) const;
	Vector3 operator-(const Vector3& v2) const;
	Vector3 operator*(float v2) const;
	Vector3 operator/(float v2) const;
	Vector3& operator+=(const Vector3& v2);
	Vector3& operator-=(const Vector3& v2);
	Vector3& operator*=(float v2);
	Vector3& operator/=(float v2);
};

} // namespace Engine
