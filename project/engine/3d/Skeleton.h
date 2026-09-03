#pragma once
#include <optional>
#include <map>
#include "Model.h"

namespace Engine {

class Animation;

/// <summary>
/// スケルトンクラス
/// モデルのボーン（Joint）階層構造を管理し、アニメーションを適用する
/// </summary>
class Skeleton {
public:
		// --- 構造体 ---
	/// <summary>
	/// ジョイント（ボーン）構造体
	/// </summary>
	struct Joint {
		Model::QuaternionTransform transform; // ローカル変形情報
		Matrix4x4 localMatrix;                // ローカル行列
		Matrix4x4 skeletonSpaceMatrix;        // スケルトン空間での行列（ルートからの累積）
		std::string name;
		std::vector<int32_t> children;        // 子JointのIndexリスト
		int32_t index;                        // 自身のIndex
		std::optional<int32_t> parent;        // 親JointのIndex
	};

		// --- メンバ関数 ---
	/// <summary>
	/// モデルのノード情報からスケルトンを生成する
	/// </summary>
	void CreateSkeleton(const Model::Node& rootNode);

	/// <summary>
	/// ノードからジョイントを再帰的に生成する内部関数
	/// </summary>
	int32_t CreateJoint(const Model::Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);

	/// <summary>
	/// アニメーションを適用する
	/// 指定時刻の姿勢を各ジョイントに反映する
	/// </summary>
	void ApplyAnimation(Animation* animation, float time);

	/// <summary>
	/// 2つのアニメーションをブレンドして適用する
	/// </summary>
	void ApplyAnimation(Animation* nowAnimation, Animation* nextAnimation, float time);

	/// <summary>
	/// 行列の更新処理
	/// 親子関係を加味してスケルトン空間行列を計算する
	/// </summary>
	void Update();

	/// <summary>
	/// デバッグ用：スケルトン（骨組み）を描画する
	/// </summary>
	void Draw(Matrix4x4 worldMatrix);

private:
		// --- メンバ変数 ---
	int32_t root; // RootJointのIndex
	std::map<std::string, int32_t> jointMap; // Joint名とIndexの辞書
	std::vector<Joint> joints; // 全ジョイントのリスト

public:
		// --- ゲッター ---
	std::vector<Joint>& GetJoints() { return joints; }
	std::map<std::string, int32_t>& GetJointMap() { return jointMap; }
};

} // namespace Engine
