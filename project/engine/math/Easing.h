#pragma once

namespace Engine {

/// <summary>
/// イージング関数クラス
/// アニメーションの補間計算（始点から終点への遷移）を行う静的メソッド群
/// </summary>
class Easing {
public:
	// --- Sine (正弦波) ---
	/// <summary>
	/// Sine形状のイージング (入り)
	/// ゆっくり動き出し、徐々に加速する
	/// </summary>
	static float EaseInSine(float time, float startPos, float endPos);
	static float EaseOutSine(float time, float startPos, float endPos);
	static float EaseInOutSine(float time, float startPos, float endPos);

	// --- Quad (2乗) ---
	/// <summary>
	/// 2乗のイージング (入り)
	/// </summary>
	static float EaseInQuad(float time, float startPos, float endPos);
	static float EaseOutQuad(float time, float startPos, float endPos);
	static float EaseInOutQuad(float time, float startPos, float endPos);

	// --- Cubic (3乗) ---
	/// <summary>
	/// 3乗のイージング (入り)
	/// </summary>
	static float EaseInCubic(float time, float startPos, float endPos);
	static float EaseOutCubic(float time, float startPos, float endPos);
	static float EaseInOutCubic(float time, float startPos, float endPos);

	// --- Quart (4乗) ---
	/// <summary>
	/// 4乗のイージング (入り)
	/// </summary>
	static float EaseInQuart(float time, float startPos, float endPos);
	static float EaseOutQuart(float time, float startPos, float endPos);
	static float EaseInOutQuart(float time, float startPos, float endPos);

	// --- Quint (5乗) ---
	/// <summary>
	/// 5乗のイージング (入り)
	/// </summary>
	static float EaseInQuint(float time, float startPos, float endPos);
	static float EaseOutQuint(float time, float startPos, float endPos);
	static float EaseInOutQuint(float time, float startPos, float endPos);

	// --- Expo (指数) ---
	/// <summary>
	/// 指数関数のイージング (入り)
	/// 非常に急激な加速
	/// </summary>
	static float EaseInExpo(float time, float startPos, float endPos);
	static float EaseOutExpo(float time, float startPos, float endPos);
	static float EaseInOutExpo(float time, float startPos, float endPos);

	// --- Circ (円形) ---
	/// <summary>
	/// 円弧形状のイージング (入り)
	/// </summary>
	static float EaseInCirc(float time, float startPos, float endPos);
	static float EaseOutCirc(float time, float startPos, float endPos);
	static float EaseInOutCirc(float time, float startPos, float endPos);

	// --- Back (バック) ---
	/// <summary>
	/// 少し行き過ぎてから戻るイージング (入り)
	/// </summary>
	static float EaseInBack(float time, float startPos, float endPos);
	static float EaseOutBack(float time, float startPos, float endPos);
	static float EaseInOutBack(float time, float startPos, float endPos);

	// --- Elastic (弾性) ---
	/// <summary>
	/// ゴムのような弾性のあるイージング (入り)
	/// </summary>
	static float EaseInElastic(float time, float startPos, float endPos);
	static float EaseOutElastic(float time, float startPos, float endPos);
	static float EaseInOutElastic(float time, float startPos, float endPos);

	// --- Bounce (バウンド) ---
	/// <summary>
	/// ボールが弾むようなイージング (入り)
	/// </summary>
	static float EaseInBounce(float time, float startPos, float endPos);
	static float EaseOutBounce(float time, float startPos, float endPos);
	static float EaseInOutBounce(float time, float startPos, float endPos);
};

} // namespace Engine
