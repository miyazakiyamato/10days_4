#pragma once
#include <numbers>
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMtMatrix.h"

class MapChipField;
class Player;
/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	enum Corner {
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左下

		kNumCorner, // 要素数
	};
	// マップとの当たり判定
	struct CollisionMapInfo {
		bool isCeilingCollision = false;
		bool landing = false;
		bool hitWall = false;
		Vector3 amountMovement;
	};
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
	void MapCollision(CollisionMapInfo& info);
	void MapCollisionLeft(CollisionMapInfo& info);
	void MapCollisionRight(CollisionMapInfo& info);
	void MapCollisionBottom(CollisionMapInfo& info);
	void MapCollisionTop(CollisionMapInfo& info);
	void MoveToReflectCollision(CollisionMapInfo& info);
	void onHitWall(const CollisionMapInfo& info);
	Vector3 CornerPosition(const Vector3& center, Corner corner);
	// ワールド返還データ
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	//
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//歩行の速さ
	static inline const float kWalkSpeed = 0.1f;
	//速度
	Vector3 velocity_ = {};
	//最初の角度
	static inline const float kWalkMotionAngleStart = std::numbers::pi_v<float> / -4.0f;
	//最後の角度
	static inline const float kWalkMotionAngleEnd = std::numbers::pi_v<float> / 2.0f;
	//アニメーションの周期となる時間
	static inline const float kWalkMotionTime = 1.0f;
	//経過時間
	float walkTimer_ = 0.0f;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 1.8f;
	static inline const float kHeight = 1.8f;

	static inline const float kBlank = 0.2f;

	Player* player_ = nullptr;
	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	uint32_t mapXIndex = 0;
	uint32_t mapYIndex = 0;

	bool isAlive = true;

	bool isMove = false;
	bool isLast_ = false;

	static uint32_t NextEnemyNum;
	uint32_t enemyNum_ = 0;

public:
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	void SetPlayer(Player* player) { player_ = player; }
	Vector3 GetWorldPosition();
	AABB GetAABB();
	const Vector3& GetVelocity() const { return velocity_; }
	bool GetIsAlive() { return isAlive; }
	bool GetIsMove() const { return isMove; }
	bool GetIsLast() const { return isLast_; }
	void SetIsLast(bool isLast) { isLast_ = isLast; }
};
