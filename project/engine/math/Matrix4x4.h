#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace Engine {

/// <summary>
/// トランスフォーム構造体
/// オブジェクトのSRT(Scale, Rotate, Translate)を保持する
/// </summary>
struct Transform {
	Vector3 scale{ 1.0f, 1.0f, 1.0f };      // 拡大縮小
	Vector3 rotate{ 0.0f, 0.0f, 0.0f };     // 回転 (オイラー角)
	Vector3 translate{ 0.0f, 0.0f, 0.0f };  // 平行移動
};

struct Quaternion;

/// <summary>
/// 4x4行列構造体
/// 3D座標変換に使用する
/// </summary>
struct Matrix4x4 {
		// メンバ変数
	float m[4][4];

		// --- メンバ関数 ---
	/// <summary>
	/// 逆行列を計算して返す
	/// </summary>
	Matrix4x4 Inverse() const;

	/// <summary>
	/// 転置行列を計算して返す
	/// </summary>
	Matrix4x4 Transpose() const;

	/// <summary>
	/// ベクトルとの積 (座標変換) を計算する
	/// </summary>
	Vector3 Transform(const Vector3& Vector) const;

	/// <summary>
	// Matrix4x4 からクォータニオンを生成する静的関数
	/// </summary>
	Quaternion ToQuaternion() const;
	// --- 静的関数 ---

	/// <summary>
	/// 逆行列の作成
	/// </summary>
	static Matrix4x4 Inverse(const Matrix4x4& m);

	/// <summary>
	/// 転置行列の作成
	/// </summary>
	static Matrix4x4 Transpose(const Matrix4x4& m);

	/// <summary>
	/// 単位行列の作成
	/// </summary>
	static Matrix4x4 MakeIdentity4x4();

	// --- 変換行列作成 ---

	/// <summary>
	/// 平行移動行列の作成
	/// </summary>
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	/// <summary>
	/// 拡大縮小行列の作成
	/// </summary>
	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	/// <summary>
	/// 座標変換 (静的関数版)
	/// </summary>
	static Vector3 Transform(const Vector3& Vector, const Matrix4x4& matrix);

	// --- 回転行列作成 ---
	static Matrix4x4 MakeRotateXMatrix(float radius);
	static Matrix4x4 MakeRotateYMatrix(float radius);
	static Matrix4x4 MakeRotateZMatrix(float radius);

	/// <summary>
	/// アフィン変換行列の作成 (SRT順)
	/// </summary>
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	// --- プロジェクション行列作成 ---

	/// <summary>
	/// 透視投影行列の作成
	/// </summary>
	static Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	/// <summary>
	/// 平行投影行列の作成
	/// </summary>
	static Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

	/// <summary>
	/// ビューポート変換行列の作成
	/// </summary>
	static Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

		// --- 演算子のオーバーロード ---
	Matrix4x4 operator*(const Matrix4x4& m2) const;
	Matrix4x4& operator*=(const Matrix4x4& m2);
};

} // namespace Engine
