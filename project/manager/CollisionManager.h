#pragma once
#include <memory>
#include <list>
#include "Collider.h"
#include "Model.h"
#include "ViewProjection.h"

class CollisionManager {
public:
	//初期化
	void Initialize();
	//ワールドトランスフォームの更新
	void UpdateWorldTransform();
	//描画
	void Draw(const ViewProjection& viewProjection);
	// リセット
	void Reset();

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	//全ての当たり判定チェック
	void CheckAllCollisions();

	void AddCollider(Collider* collider);

private:
	// 調整項目の適用
	void ApplyGlobalVariables();

	//コライダー
	std::list<Collider*> colliders_;
	//デバッグ用モデル
	Model* model_;

	bool isColliderModel_ = false;
};
