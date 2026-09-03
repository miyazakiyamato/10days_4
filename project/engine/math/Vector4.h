#pragma once
#include <random>

namespace Engine {

/// <summary>
/// 4次元ベクトル
/// 斉次座標や色情報(RGBA)として使用する
/// </summary>
struct Vector4 {
		// --- メンバ変数 ---
	float x;
	float y;
	float z;
	float w;

		// --- 静的関数 ---

	/// <summary>
	/// 誤差を許容する等価比較
	/// </summary>
	static bool IsEqualWithEpsilon(const Vector4& a, const Vector4& b, float epsilon = 1e-5f);

	/// <summary>
	/// ランダムなベクトルを生成
	/// </summary>
	static Vector4 Random(std::mt19937& randomEngine, const Vector4& vMin, const Vector4& vMax);

	// 演算子のオーバーロード
};

} // namespace Engine
