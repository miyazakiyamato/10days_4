#pragma once
#include <memory>
#include "Matrix4x4.h"
#include "Object3d.h"
#include "BaseBullet.h"

namespace Engine {

/// <summary>
/// 敵の弾
/// </summary>
class EnemyBullet : public BaseBullet {
public:
	~EnemyBullet() {}
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position,const Vector3& velocity);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突時コールバック
	/// </summary>
	/// <param name="other">衝突相手</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

private:

public:
	
};

} // namespace Engine
