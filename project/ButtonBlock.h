#pragma once
#include "MyMtMatrix.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Model.h>
#include <ObjectColor.h>

class Player;

class ButtonBlock {
public:
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();
	void OnCollision(const Player* player);

	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	Vector3 GetWorldPosition();
	AABB GetAABB();

	void SetId(int id);
	
	int GetId() { return id_; }
	bool GetIsActive() { return isActive_; }

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	ObjectColor* objectColor_ = nullptr;

	static inline const float kWidth = 1.8f;
	static inline const float kHeight = 1.8f;

	int id_ = -1;
	bool isActive_ = false;
	bool isCollidingThisFrame_ = false;
	bool wasCollidingLastFrame_ = false;
};