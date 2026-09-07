#pragma once
#include "MyMtMatrix.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Model.h>
#include <ObjectColor.h>
#include <vector>

class Player;
class MapChipField; // 追加
class MovingBlock;

class BrokenBlock {
public:
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position);
	void Update(const std::vector<BrokenBlock*>& brokenBlocks, const std::vector<MovingBlock*>& movingBlocks);
	void Draw();
	void OnCollision(const Player* player);

	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	Vector3 GetWorldPosition();
	AABB GetAABB();

	void SetId(int id) { id_ = id; }
	int GetId() { return id_; }
	void SetIsActive(bool isActive) { isActive_ = isActive; }
	bool GetIsActive() { return isActive_; }
	Vector3 GetVelocity() { return velocity_; }

	// 追加：マップとの当たり判定用
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void SetObjectColor(ObjectColor* objectColor) { objectColor_ = objectColor; }

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	ObjectColor* objectColor_ = nullptr;
	MapChipField* mapChipField_ = nullptr; // 追加

	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};
	Vector3 targetPos_ = {0.0f, 0.0f, 0.0f}; // 追加：元の位置を記録する変数

	static inline const float kWidth = 2.0f;
	static inline const float kHeight = 2.0f;
	static inline const float kReturnSpeed = 0.2f;    // 戻るスピード
	static inline const float kGravity = 0.02f;       // 落下加速度
	static inline const float kLimitFallSpeed = 0.5f; // 最大落下速度

	int id_ = -1;
	bool isActive_ = false;
};