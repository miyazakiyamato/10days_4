#pragma once
#include "Matrix4x4.h"

namespace Engine {

/// <summary>
/// クォータニオン（四元数）構造体
/// 3D空間での回転を表すために使用され、ジンバルロックの回避やスムーズな補間（Slerp）が可能。
/// </summary>
struct Quaternion {
public:
	/// <summary>
	/// 虚部(x, y, z) と 実部(w)
	/// </summary>
	float x, y, z, w;

	/// <summary>
	/// 任意軸回転の行列を作成する
	/// </summary>
	/// <param name="axis">回転軸（正規化されていることを推奨）</param>
	/// <param name="angle">回転角（ラジアン）</param>
	/// <returns>回転を表すMatrix4x4</returns>
	static Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);

	/// <summary>
	/// ある方向から別の方向への回転行列を作成する
	/// </summary>
	/// <param name="from">開始方向ベクトル（正規化推奨）</param>
	/// <param name="to">目標方向ベクトル（正規化推奨）</param>
	/// <returns>fromをtoに向ける回転を表すMatrix4x4</returns>
	static Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);

	/// <summary>
	/// 単位クォータニオン(Identity)を返す
	/// 回転なしを表す (x=0, y=0, z=0, w=1)
	/// </summary>
	/// <returns>単位クォータニオン</returns>
	static Quaternion Identity();

	/// <summary>
	/// 共役クォータニオンを返す
	/// 虚部の符号を反転させたもの (-x, -y, -z, w)
	/// </summary>
	/// <param name="quaternion">対象のクォータニオン</param>
	/// <returns>共役クォータニオン</returns>
	static Quaternion Conjugate(const Quaternion& quaternion);

	/// <summary>
	/// 逆クォータニオンを返す
	/// 単位クォータニオンであれば共役と同じ。回転を逆にする際に使用。
	/// </summary>
	/// <param name="quaternion">対象のクォータニオン</param>
	/// <returns>逆クォータニオン</returns>
	static Quaternion Inverse(const Quaternion& quaternion);

	/// <summary>
	/// 正規化したクォータニオンを返す
	/// 長さを1にする。回転の累積による誤差修正に必要。
	/// </summary>
	/// <param name="quaternion">対象のクォータニオン</param>
	/// <returns>正規化されたクォータニオン</returns>
	static Quaternion Normalize(const Quaternion& quaternion);

	/// <summary>
	/// クォータニオン同士の乗算（回転の合成）
	/// </summary>
	/// <param name="lhs">左辺</param>
	/// <param name="rhs">右辺</param>
	/// <returns>合成されたクォータニオン</returns>
	static Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);

	/// <summary>
	/// クォータニオンのノルム（長さ）を返す
	/// </summary>
	/// <param name="quaternion">対象のクォータニオン</param>
	/// <returns>ノルム</returns>
	static float Norm(const Quaternion& quaternion);

	/// <summary>
	/// 任意軸回転を表すクォータニオンを作成する
	/// </summary>
	/// <param name="axis">回転軸（正規化推奨）</param>
	/// <param name="angle">回転角（ラジアン）</param>
	/// <returns>回転クォータニオン</returns>
	static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

	/// <summary>
	/// ベクトルをクォータニオンで回転させる
	/// </summary>
	/// <param name="vector">回転させるベクトル</param>
	/// <param name="quaternion">回転クォータニオン</param>
	/// <returns>回転後のベクトル</returns>
	static Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

	/// <summary>
	/// クォータニオンから回転行列を作成する
	/// </summary>
	/// <param name="quaternion">回転クォータニオン</param>
	/// <returns>回転行列</returns>
	static Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

	/// <summary>
	/// アフィン変換行列を作成する（スケール -> 回転(Quat) -> 平行移動）
	/// </summary>
	/// <param name="scale">スケール</param>
	/// <param name="rotate">回転クォータニオン</param>
	/// <param name="translate">平行移動</param>
	/// <returns>アフィン行列</returns>
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);

	/// <summary>
	/// 球面線形補間 (Spherical Linear Interpolation)
	/// 2つの回転間を最短距離で滑らかに補間する
	/// </summary>
	/// <param name="q0">開始クォータニオン</param>
	/// <param name="q1">終了クォータニオン</param>
	/// <param name="t">補間係数 (0.0 ～ 1.0)</param>
	/// <returns>補間されたクォータニオン</returns>
	static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

	/// <summary>
	/// クォータニオンをオイラー角（Pitch, Yaw, Roll）に変換する
	/// </summary>
	/// <returns>オイラー角（Vector3: X=Pitch, Y=Yaw, Z=Roll）</returns>
	Vector3 ToEulerAngles() const;

	// --- 演算子オーバーロード ---

	/// <summary>
	/// 加算
	/// </summary>
	Quaternion operator+(const Quaternion& q) const;

	/// <summary>
	/// スカラー乗算
	/// </summary>
	Quaternion operator*(float f) const;

	/// <summary>
	/// スカラー除算
	/// </summary>
	Quaternion operator/(float f) const;

	/// <summary>
	/// 単項マイナス（各成分の符号反転）
	/// </summary>
	Quaternion operator-() const;

	/// <summary>
	/// クォータニオン同士の乗算
	/// </summary>
	Quaternion operator*(const Quaternion& q) const;
};

} // namespace Engine
