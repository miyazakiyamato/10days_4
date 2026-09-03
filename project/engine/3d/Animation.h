#pragma once
#include <map>
#include "Quaternion.h"

namespace Engine {

/// <summary>
/// アニメーションデータクラス
/// キーフレームによる移動・回転・スケールの推移を管理する
/// </summary>
class Animation {
public:
		// --- 構造体 ---
	/// <summary>
	/// キーフレーム構造体
	/// 特定の時間における値を保持する
	/// </summary>
	template <typename tValue>
	struct Keyframe {
		float time;   //!< キーフレームの時間
		tValue value; //!< キーフレームの値
	};

	using KeyframeVector3 = Keyframe<Vector3>;
	using KeyframeQuaternion = Keyframe<Quaternion>;

	/// <summary>
	/// アニメーションカーブ
	/// キーフレームの集合により、時間の経過に伴う値の変化を表す
	/// </summary>
	template <typename tValue>
	struct AnimationCurve {
		std::vector<Keyframe<tValue>> keyframes;
	};

	/// <summary>
	/// ノードごとのアニメーションデータ
	/// </summary>
	struct NodeAnimation {
		AnimationCurve<Vector3> translate;
		AnimationCurve<Quaternion> rotate;
		AnimationCurve<Vector3> scale;
	};

		// --- メンバ関数 ---
	/// <summary>
	/// アニメーションファイルを読み込む (.gltf など)
	/// </summary>
	/// <param name="filename">ファイル名</param>
	void LoadFile(const std::string& filename);

	/// <summary>
	/// 指定時刻の値を算出する（線形補間）
	/// </summary>
	/// <param name="curve">対象のアニメーションカーブ</param>
	/// <param name="time">現在の時間</param>
	/// <returns>補間されたVector3値</returns>
	Vector3 CalculateValue(const AnimationCurve<Vector3>& curve, float time);

	/// <summary>
	/// 指定時刻の値を算出する（球面線形補間 Slerp）
	/// </summary>
	/// <param name="curve">対象のアニメーションカーブ</param>
	/// <param name="time">現在の時間</param>
	/// <returns>補間されたQuaternion値</returns>
	Quaternion CalculateValue(const AnimationCurve<Quaternion>& curve, float time);

	/// <summary>
	/// 指定時刻におけるローカル行列を作成する
	/// </summary>
	/// <param name="rootNodeName">対象のノード名</param>
	/// <param name="time">アニメーション時間</param>
	/// <returns>ローカル変換行列</returns>
	Matrix4x4 MakeLocalMatrix(std::string rootNodeName, float time);

	/// <summary>
	/// アニメーションブレンド時のローカル行列を作成する（補間あり）
	/// </summary>
	Matrix4x4 MakeLocalMatrix(std::string rootNodeName, const NodeAnimation& nextNodeAnimation, float time);

private:
		// --- メンバ変数 ---
	std::string directoryPath = "./resources/model/"; //!< ディレクトリパス

	float duration; //!< アニメーション全体の長さ（秒）
	std::map<std::string, NodeAnimation> nodeAnimations; //!< ノード名とアニメーションデータのマップ

public:
		// --- ゲッター ---
	float GetDuration() { return duration; }
	const NodeAnimation& GetNodeAnimations(std::string rootNodeName) { return nodeAnimations[rootNodeName]; }
	const std::map<std::string, NodeAnimation>& GetNodeAnimationsMap() { return nodeAnimations; }
};

} // namespace Engine
