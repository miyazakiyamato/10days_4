#pragma once
#include <Model.h>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMtMatrix.h"

class Player;
class GoalBlock {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void OnCollision(const Player* player);

private:
	// ワールド返還データ
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	//
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 1.8f;
	static inline const float kHeight = 1.8f;

public:
	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	Vector3 GetWorldPosition();
	AABB GetAABB();
};
