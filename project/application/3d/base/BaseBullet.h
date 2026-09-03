#pragma once
#include <memory>
#include "Matrix4x4.h"
#include "Object3d.h"
#include "Collider.h"

namespace Engine {

/// <summary>
/// 弾基底クラス
/// </summary>
class BaseBullet : public Collider {
public:
	virtual ~BaseBullet() {}
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 衝突時コールバック
	/// </summary>
	/// <param name="other">衝突相手</param>
	virtual void OnCollision([[maybe_unused]] Collider* other) override;

protected:
	// 寿命
	static inline const float kLifeTime_ = 5.0f;

	Vector3 velocity_{};
	// モデル
	std::unique_ptr<Object3d> object3d_ = nullptr;
	//デスタイマー
	float deathTimer_ = kLifeTime_;
	//生存フラグ
	bool isAlive_ = true;
	//
	/// <summary>
	/// 速度に合わせて回転させる
	/// </summary>
	/// <param name="velocity">速度ベクトル</param>
	void Rotate(const Vector3& velocity);

public:
	bool isAlive() const { return isAlive_; }
	Vector3 GetCenterPosition()const override;
	Object3d* GetObject3d() const {return object3d_.get();}
};

} // namespace Engine
