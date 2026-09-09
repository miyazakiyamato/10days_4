#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMtMatrix.h"

enum class LRDirection {
	kRight,
	kLeft,
};
enum Corner{
	kRightBottom,//右下
	kLeftBottom,//左下
	kRightTop,//右上
	kLeftTop,//左下

	kNumCorner,//要素数
};
enum class AnimState {
	kIdle, // 待機中
	kRun,  // 移動中
	kJump, // ジャンプ中
	kLand, // 着地時の潰れ（一定時間ロック）
};
//マップとの当たり判定
struct CollisionMapInfo {
	bool isCeilingCollision = false;
	bool landing = false;
	bool hitWall = false;
	Vector3 amountMovement;
};
class Input;
class MapChipField;
class Enemy;
class GoalBlock;
class MovingBlock;
class BrokenBlock;

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	Player() {}

	~Player() {
	
	}
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model,uint32_t textureHandle,ViewProjection* viewProjection,const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void Move();
	void MapCollision(CollisionMapInfo& info);
	void MapCollisionLeft(CollisionMapInfo& info);
	void MapCollisionRight(CollisionMapInfo& info);
	void MapCollisionBottom(CollisionMapInfo& info);
	void MapCollisionTop(CollisionMapInfo& info);
	void MoveToReflectCollision(CollisionMapInfo& info);
	void OnCeilingCollision(const CollisionMapInfo& info);
	void GroundingSwitch(const CollisionMapInfo& info);
	void onHitWall(const CollisionMapInfo& info);
	Vector3 CornerPosition(const Vector3& center, Corner corner);
	void OnCollision(const Enemy* enemy);
	void OnCollision(const GoalBlock* goalBlock);
	void OnCollision(MovingBlock* movingBlock);
	void OnCollision(BrokenBlock* brokenBlock);

	void ResolveBlockCollision(const AABB& b, bool isMovingBlock, MovingBlock* movingBlock = nullptr);

	void ProcessMoveAndJump();

	void ProcessAnimation();

	void UpdateScaleInterpolation();

private:

	//ワールド返還データ
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	//
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//
	static inline const float kLimitRunSpeed = 0.5f;
	Vector3 velocity_ = {};

	LRDirection lrDirection_ = LRDirection::kRight;
	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	//旋回タイマー
	float turnTimer_ = 0.0f;
	//旋回時間
	static inline const float kTimeTurn = 0.3f;
	//着地フラグ
	bool onGround_ = true;
	bool wasOnGround_ = true;
	bool isOnCustomBlock_ = false; // 動く・壊れたブロックの上にいるか
	//移動速度 減衰速度
	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.1f;

	//重力加速度下
	static inline const float kGravityAcceleration = 0.045f;
	//最大落下速度下
	static inline const float kLimitFallSpeed = kGravityAcceleration * 100.f;
	//ジャンプ初速
	static inline const float kJumpAcceleration = 0.65f;
	//着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.1f;
	//着地時の速度減衰率
	static inline const float kAttenuationWall = 0.1f;

	//マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
	//lastEnemy
	Enemy* lastEnemy = nullptr;
	//キャラクターの当たり判定サイズ
	static inline const float kWidth = 1.8f;
	static inline const float kHeight = 1.8f;

	//
	static inline const float kBlank = 0.2f;

	//
	bool isAlive_ = true;

	Vector3 enemyVelocity_{};
	bool isPush_ = false;
	bool isMakeBlock = false;
	bool isMoveBlock_ = false;

	bool isClear = false;

	float maxPushTime = 0.5f;
	float pushTime = 0;

	bool preSpace_ = false;
	bool isPushSpace_ = false;

	MovingBlock* currentRideBlock_ = nullptr;
	
	// スライムアニメーション用のスケール管理
	Vector3 baseScale_ = {1.0f, 1.0f, 1.0f};
	Vector3 targetScale_ = {1.0f, 1.0f, 1.0f};

	// 待機・移動アニメーション用タイマー
	float animTimer_ = 0.0f;
	bool landedOnBlock_ = false; // ブロックに着地した瞬間フラグ

	AnimState animState_ = AnimState::kIdle;
	float animTimeCount_ = 0.0f; // アニメーションの個別タイマー
public:
	WorldTransform* GetWorldTransform() { return &worldTransform_;}
	const Vector3& GetVelocity() const { return velocity_; }
	Vector3 GetWorldPosition();
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	void SetLastEnemy(Enemy* enemy) { lastEnemy = enemy; }
	AABB GetAABB();
	AABB GetCrushAABB();
	LRDirection GetLRDirection() const { return lrDirection_; }
	bool GetIsAlive() { return isAlive_; }
	bool GetIsPush() { return isPush_; }
	bool GetIsMakeBlock() { return isMakeBlock; }
	bool GetIsMoveBlock() { return isMoveBlock_; }
	bool GetIsClear() { return isClear; }
	void SetIsAlive(bool isAlive) { isAlive_ = isAlive; }
	void SetIsMoveBlock(bool isMoveBlock) { isMoveBlock_ = isMoveBlock; }
};
