#pragma once
#include "MyMtMatrix.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Model.h>
#include <ObjectColor.h>
#include <vector>

class Player;
class BrokenBlock;

class MovingBlock {
public:
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position);
	void Update(const std::vector<BrokenBlock*>& brokenBlocks);
	void Draw();
	void OnCollision(const Player* player);

	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	Vector3 GetWorldPosition();
	AABB GetAABB();

	void SetId(int id) { id_ = id; }
	int GetId() { return id_; }
	void SetIsActive(bool isActive) { isActive_ = isActive; }

	// 終わりの位置（目的地：0_0の位置）を設定する関数
	void SetEndPosition(const Vector3& endPos) {
		endPos_ = endPos;
		hasEndPos_ = true;
	}

	Vector3 GetVelocity() { return velocity_; }
	void SetObjectColor(ObjectColor* objectColor) { objectColor_ = objectColor; }

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	ObjectColor* objectColor_ = nullptr;

	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};

	static inline const float kWidth = 2.0f;
	static inline const float kHeight = 2.0f;
	static inline const float kSpeed = 0.05f;

	int id_ = -1;
	bool isActive_ = false;

	// 1対の空間管理用
	Vector3 startPos_ = {0.0f, 0.0f, 0.0f}; // 開始位置（5_0の位置：初期化時に記録）
	Vector3 endPos_ = {0.0f, 0.0f, 0.0f};   // 終わりの位置（0_0の位置）
	bool hasEndPos_ = false;               // 終わりの位置が設定されているか
	bool isReturning_ = false;             // false = 終わり(0_0)へ向かっている, true = 始まり(5_0)へ戻っている
};