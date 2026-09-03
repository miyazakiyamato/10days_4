#pragma once
#include <memory>
#include "Matrix4x4.h"
#include "Object3d.h"
#include "Collider.h"

namespace Engine {

enum class AttackType {
	kRoot,
	kCharge
};

class BulletManager;
class Player;
/// <summary>
/// 武器基底クラス
/// </summary>
class BaseWeapon : public Collider {
public:
		// --- 構造体 ---
	struct AttackData {
		float kBulletSpeed = 100.0f;
		float bulletSize = 0.3f;
		float kCoolTime = 0.3f;
		float coolTimeCount = 0.0f;
		uint32_t kBulletCount = 3;
		uint32_t bulletCount = 0;
	};
		//　--- メンバ関数 ---
	// 仮想デストラクタ
	virtual ~BaseWeapon() override {}

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新
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

	/// <summary>
	/// 通常ショット
	/// </summary>
	/// <param name="attackType">攻撃タイプ</param>
	void Shoot(AttackType attackType);
	/// <summary>
	/// チャージ
	/// </summary>
	virtual void Charge();
private:
	void Rotate(const Vector3& velocity);
protected:
	// 弾丸管理者
	BulletManager* bulletManager_ = nullptr;
	// モデル
	std::unique_ptr<Object3d> object3d_ = nullptr;
	std::unique_ptr<Object3d> gunBarrel_ = nullptr;
	//ターゲットオブジェクト
	Object3d* target_ = nullptr;
	Vector3 targetPosition_{0.0f,0.0f,1.0f};
	//攻撃データ
	float kChargeTime_ = 0.3f;
	float chargeCount_ = 0.0f;
	std::vector<AttackData> attackData_;
	//生存フラグ
	bool isAlive_ = true;
public:
	// ゲッター
	bool isAlive() const { return isAlive_; }
	Vector3 GetCenterPosition()const override;
	Object3d* GetObject3d() const {return object3d_.get();}
	Object3d* GetGunBarrel() const { return gunBarrel_.get(); }
	float GetKChargeTime() const { return kChargeTime_; }
	float GetChargeCount() const { return chargeCount_; }
	const AttackData& GetAttackData(AttackType attackType) const {return attackData_[static_cast<size_t>(attackType)];}
	
	// セッター
	void SetBulletManager(BulletManager* bulletManager) { bulletManager_ = bulletManager; }
	void SetTarget(Object3d* target) { target_ = target; }
	void SetChargeCount(float chargeCount) { chargeCount_ = chargeCount; }
	void SetPlayer(Player* player) { player_ = player; }

protected:
	Player* player_ = nullptr;
};

} // namespace Engine
