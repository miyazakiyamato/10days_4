#pragma once
#include <memory>
#include <list>
#include "Collider.h"

namespace Engine {

/// <summary>
/// 衝突判定マネージャクラス
/// ゲーム内の全コライダーを管理し、衝突判定を実行する
/// </summary>
class CollisionManager {
public:
		// --- メンバ関数 ---
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ワールドトランスフォームの更新
	/// 管理下の全コライダーの更新を行う
	/// </summary>
	void UpdateWorldTransform();

	/// <summary>
	/// 描画
	/// デバッグ表示が有効な場合、コライダーを描画する
	/// </summary>
	void Draw();

	/// <summary>
	/// リセット
	/// 登録されたコライダーリストをクリアする
	/// </summary>
	void Reset();

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// 衝突していた場合、それぞれのOnCollisionを呼び出す
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// 全ての当たり判定チェック
	/// 登録された全ペアに対して総当たり判定を行う
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// コライダーの登録
	/// </summary>
	/// <param name="collider">登録するコライダーへのポインタ</param>
	void AddCollider(Collider* collider);

private:
	/// <summary>
	/// 調整項目の適用 (GlobalVariablesなどから設定を反映)
	/// </summary>
	void ApplyGlobalVariables();

		// --- メンバ変数 ---
	// 登録されたコライダーのリスト
	std::list<Collider*> colliders_;

	// コライダーモデルを表示するかどうかのフラグ
	bool isColliderModel_ = false;
};

} // namespace Engine
