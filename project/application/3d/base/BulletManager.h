#pragma once
#include "BaseBullet.h"
#include <memory>

namespace Engine {

class CollisionManager;
/// <summary>
/// 弾丸マネージャ
/// </summary>
class BulletManager{
public:
	BulletManager() = default;
	~BulletManager() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 弾を追加
	/// </summary>
	/// <param name="bullet">追加する弾</param>
	void AddBullet(std::unique_ptr<BaseBullet> bullet);
	/// <summary>
	/// コライダーを追加
	/// </summary>
	/// <param name="collisionManager">コリジョンマネージャー</param>
	void AddCollider(CollisionManager* collisionManager);
private:
	std::list<std::unique_ptr<BaseBullet>> bullets_; // 弾のリスト
};


} // namespace Engine
