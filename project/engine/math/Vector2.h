#pragma once

namespace Engine {

/// <summary>
/// 2次元ベクトル
/// UV座標や画面座標などで使用する
/// </summary>
struct Vector2 {
		//  --- メンバ変数 ---
	float x;
	float y;

		// --- メンバ関数 ---
	/// <summary>
	/// 長さ（ノルム）を返す
	/// </summary>
	float Length() const;

	/// <summary>
	/// 正規化（長さ1）したベクトルを返す
	/// </summary>
	Vector2 Normalize() const;

	/// <summary>
	/// 絶対値ベクトルを返す
	/// </summary>
	Vector2 Abs() const;

	/// <summary>
	/// 0.0～1.0の範囲にクランプする
	/// </summary>
	Vector2 Clamp01() const;

	/// <summary>
	/// -1.0～1.0の範囲にクランプする
	/// </summary>
	Vector2 Clamp_11() const;

		// --- 静的関数 ---
	static float Dot(const Vector2& v1, const Vector2& v2);   // 内積
	static float Cross(const Vector2& v1, const Vector2& v2); // 外積 (2D擬似外積)
	static float Length(float x, float y);
	static float Length(Vector2 v);
	static Vector2 Normalize(float x, float y);
	static Vector2 Normalize(Vector2 v);
	static Vector2 Abs(const Vector2& v);
	static Vector2 Min(const Vector2& v, const Vector2& v2);
	static Vector2 Max(const Vector2& v, const Vector2& v2);
	static Vector2 Clamp(const Vector2& v, const Vector2& min, const Vector2& max);

		// --- 演算子のオーバーロード ---
	Vector2 operator-() const;
	Vector2 operator+(const Vector2& v2) const;
	Vector2 operator-(const Vector2& v2) const;
	Vector2 operator*(float v2) const;
	Vector2 operator/(float v2) const;
	Vector2& operator+=(const Vector2& v2);
	Vector2& operator-=(const Vector2& v2);
	Vector2& operator*=(float v2);
	Vector2& operator/=(float v2);
};

} // namespace Engine
