#pragma once
#include <memory>
#include <map>
#include <string>
#include "BaseCharacter.h"
#include "TimedCall.h"
#include "BaseEnemyState.h"
#include "SplineRail.h"

namespace Engine {

class BulletManager;
class ParticleSystem;
class Player;
/// <summary>
/// 敵基底クラス
/// </summary>
class BaseEnemy : public BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 衝突時コールバック
	/// </summary>
	/// <param name="other">衝突相手</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

	/// <summary>
	/// ステート変更
	/// </summary>
	/// <param name="state">新しいステート</param>
	void ChangeState(std::unique_ptr<BaseEnemyState> state);

	/// <summary>
	/// レールの追加
	/// </summary>
	/// <param name="name">レール名</param>
	/// <param name="rail">レール</param>
	void AddRail(const std::string& name, std::unique_ptr<SplineRail> rail);

	/// <summary>
	/// レールの取得
	/// </summary>
	/// <param name="name">レール名</param>
	/// <returns>レール</returns>
	const SplineRail* GetRail(const std::string& name) const;
protected:
	void Damage(int damage, const Vector3& hitDirection);

	// メンバ変数
	BulletManager* bulletManager_ = nullptr; // バレットマネージャー
	ParticleSystem* particleSystem_ = nullptr;
	Player* player_ = nullptr; // プレイヤーへの参照
	int hp_ = 3; // ヒットポイント
	bool isAlive_ = true; // 生存状態

	// 発射間隔
	float kFireInterval_ = 3.0f;

	// ステート
	std::unique_ptr<BaseEnemyState> state_;

	// レール
	std::map<std::string, std::unique_ptr<SplineRail>> rails_;

public:
	/// <summary>
	/// パーティクルシステムの設定
	/// </summary>
	void SetParticleSystem(ParticleSystem* particleSystem) { particleSystem_ = particleSystem; }
	void SetBulletManager(BulletManager* bulletManager) { bulletManager_ = bulletManager; }
	void SetPlayer(Player* player) { player_ = player; }
	void SetPosition(const Vector3& position) { object3d_->SetTranslate(position); }
	void SetIsAlive(bool isAlive) { isAlive_ = isAlive; }
	void SetHP(int hp) { hp_ = hp; }

	bool GetIsAlive() const { return isAlive_; }
	int GetHP() const { return hp_; }
	float GetFireInterval() const { return kFireInterval_; }
	Player* GetPlayer() const { return player_; }
	BulletManager* GetBulletManager() const { return bulletManager_; }
};


} // namespace Engine
