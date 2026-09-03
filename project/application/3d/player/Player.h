#pragma once
#include "BaseCharacter.h"
//#include "LockOn.h"
#include "ContactRecord.h"
#include "Sprite.h"
#include "ReticleUI.h"
#include "BaseWeapon.h"
#include "FollowCamera.h"

namespace Engine {

class Input;
class Camera;
class ParticleSystem;
class RailCamera;
/// <summary>
/// プレイヤー
/// </summary>
class Player : public BaseCharacter{
public://メンバ関数
	Player() {}

	~Player() {}
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 衝突時コールバック
	/// </summary>
	/// <param name="other">衝突相手</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();
	/// <summary>
	/// レティクル更新
	/// </summary>
	void ReticleUpdate();

	/// <summary>
	/// カメラ追従を停止
	/// </summary>
	void StopCameraFollow();

	void Damage(int damage, const Vector3& hitDirection);

private:
	//メンバ変数
	Input* input_ = nullptr;
	const Camera* camera_ = nullptr;
	ParticleSystem* particleSystem_ = nullptr;
	RailCamera* railCamera_ = nullptr;
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;
	//武器
	std::unique_ptr<BaseWeapon> weapon_ = nullptr;
	std::unique_ptr<Object3d> hand_ = nullptr;
	Vector3 handOffset_{ 0.0f,-1.5f,0.0f };
	//3Dレティクル
	std::unique_ptr<Object3d> reticle3d_ = nullptr;
	ReticleUI* reticleUI_ = nullptr;
	//移動関連
	Vector3 velocity_{};
	Vector3 acceleration_{};
	float airResistance = 0.9f;
	Vector3 moveSpeed_{1.5f,3.0f,10.0f};
	Vector3 velocityLimit_{ 100.0f,100.0f,100.0f };
	Vector3 moveLimit_{ 10.0f,7.0f,100.0f };

	//体力
	int hp_ = 10;
	bool isAlive_ = true;
	//ロックオン
	//LockOn* lockOn_ = nullptr;
public://ゲッターセッター
	Vector3 GetWorldPosition();
	int GetHp() { return hp_; }
	bool GetIsAlive() const { return isAlive_; }
	Object3d* GetReticle3d() { return reticle3d_.get(); }
	BaseWeapon* GetWeapon() { return weapon_.get(); }
	ParticleSystem* GetParticleSystem() { return particleSystem_; }

	//void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }
	void SetReticleUI(ReticleUI* reticleUI) { reticleUI_ = reticleUI; }
	void SetParent(Object3d* object3d);
	void SetCamera(const Camera* camera) { camera_ = camera; }
	void SetParticleSystem(ParticleSystem* particleSystem) { particleSystem_ = particleSystem; }
	void SetRailCamera(RailCamera* railCamera);
	void SetWeapon(std::unique_ptr<BaseWeapon> weapon);
	void SetIsCpuMode(bool isCpuMode) { isCpuMode_ = isCpuMode; }
	bool GetIsCpuMode() const { return isCpuMode_; }

private:
	bool isCpuMode_ = false;
};


} // namespace Engine
