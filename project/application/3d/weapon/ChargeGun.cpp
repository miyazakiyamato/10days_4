#include "ChargeGun.h"
#include "Quaternion.h"
#include "Player.h"
#include "Input.h"
#include "TimeManager.h"
#include "PlayerStateShoot.h"
#include "PlayerStateShoot.h"
#include "PlayerStateChargeShoot.h"
#include "CollisionTypeIdDef.h"
#include "EnemyBullet.h"
#include "GlobalVariables.h"

namespace Engine {

// 初期化
void ChargeGun::Initialize(){
	BaseWeapon::Initialize();
	input_ = Input::GetInstance();
	timeManager_ = TimeManager::GetInstance();

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));

	GlobalVariables* gv = GlobalVariables::GetInstance();
	const std::string groupName = "ChargeGun";
	gv->CreateGroup(groupName);
	gv->AddItem(groupName, "ColliderRadius", 0.5f);
	gv->AddItem(groupName, "Scale", Vector3(0.8f, 0.8f, 0.8f));
	gv->AddItem(groupName, "GunBarrelTranslate", Vector3(0.0f, 0.0f, 4.0f));

	gv->AddItem(groupName, "RootBulletSpeed", 150.0f);
	gv->AddItem(groupName, "RootBulletSize", 0.3f);
	gv->AddItem(groupName, "RootCoolTime", 0.2f);
	gv->AddItem(groupName, "RootBulletCount", 3);

	gv->AddItem(groupName, "ChargeBulletSpeed", 100.0f);
	gv->AddItem(groupName, "ChargeBulletSize", 0.3f);
	gv->AddItem(groupName, "ChargeCoolTime", 0.5f);
	gv->AddItem(groupName, "ChargeBulletCount", 1);
	gv->AddItem(groupName, "ChargeLimitSize", 10.0f);

	// 初期値の適用
	Collider::SetRadius(gv->GetValue<float>(groupName, "ColliderRadius"));
	object3d_->SetModel("chargeGun/chargeGun.obj");
	object3d_->SetScale(gv->GetValue<Vector3>(groupName, "Scale"));
	gunBarrel_->SetTranslate(gv->GetValue<Vector3>(groupName, "GunBarrelTranslate"));
	gunBarrel_->SetParent(object3d_.get());

	// 攻撃データの設定
	AttackData& rootAttackData = attackData_[static_cast<uint32_t>(AttackType::kRoot)];
	rootAttackData.kBulletSpeed = gv->GetValue<float>(groupName, "RootBulletSpeed");
	rootAttackData.bulletSize = gv->GetValue<float>(groupName, "RootBulletSize");
	rootAttackData.kCoolTime = gv->GetValue<float>(groupName, "RootCoolTime");
	rootAttackData.kBulletCount = gv->GetValue<int>(groupName, "RootBulletCount");

	AttackData& chargeAttackData = attackData_[static_cast<uint32_t>(AttackType::kCharge)];
	chargeAttackData.kBulletSpeed = gv->GetValue<float>(groupName, "ChargeBulletSpeed");
	chargeAttackData.bulletSize = gv->GetValue<float>(groupName, "ChargeBulletSize");
	chargeAttackData.kCoolTime = gv->GetValue<float>(groupName, "ChargeCoolTime");
	chargeAttackData.kBulletCount = gv->GetValue<int>(groupName, "ChargeBulletCount");
}

// 更新
void ChargeGun::Update(){
	GlobalVariables* gv = GlobalVariables::GetInstance();
	const std::string groupName = "ChargeGun";

	// リアルタイム調整の反映
	Collider::SetRadius(gv->GetValue<float>(groupName, "ColliderRadius"));
	object3d_->SetScale(gv->GetValue<Vector3>(groupName, "Scale"));
	gunBarrel_->SetTranslate(gv->GetValue<Vector3>(groupName, "GunBarrelTranslate"));

	AttackData& rootAttackData = attackData_[static_cast<uint32_t>(AttackType::kRoot)];
	rootAttackData.kBulletSpeed = gv->GetValue<float>(groupName, "RootBulletSpeed");
	rootAttackData.bulletSize = gv->GetValue<float>(groupName, "RootBulletSize");
	rootAttackData.kCoolTime = gv->GetValue<float>(groupName, "RootCoolTime");
	rootAttackData.kBulletCount = gv->GetValue<int>(groupName, "RootBulletCount");

	AttackData& chargeAttackData = attackData_[static_cast<uint32_t>(AttackType::kCharge)];
	chargeAttackData.kBulletSpeed = gv->GetValue<float>(groupName, "ChargeBulletSpeed");
	chargeAttackData.kCoolTime = gv->GetValue<float>(groupName, "ChargeCoolTime");
	chargeAttackData.kBulletCount = gv->GetValue<int>(groupName, "ChargeBulletCount");

	// 銃の向きを更新
	if (target_) {
		targetPosition_ = target_->GetCenterPosition();
		
		Vector3 direction = Vector3::Subtract(targetPosition_, object3d_->GetCenterPosition()).Normalize();
		
		// 親のワールド行列から回転成分のみ抽出して逆クォータニオンを作成（親の回転の影響を排除＝ローカル空間への変換）
		Matrix4x4 parentMatrix = object3d_->GetParent()->GetWorldMatrix();
		Quaternion parentQuaternion = parentMatrix.ToQuaternion();
		Quaternion inverseParentQuaternion = Quaternion::Inverse(parentQuaternion);

		// ワールド空間の方向ベクトルを親のローカル空間（回転のみ考慮）へ変換
		Vector3 localDirection = Quaternion::RotateVector(direction, inverseParentQuaternion);

		// 親の回転が適用された状態（ローカル空間）での、ターゲットへの方向回転を計算
		Quaternion quaternion = Quaternion::DirectionToDirection({ 0.0f, 0.0f, 1.0f }, localDirection).ToQuaternion();

		object3d_->SetRotate(quaternion.ToEulerAngles());
	}
	BaseWeapon::Update();
}

// 描画
void ChargeGun::Draw(){
	BaseWeapon::Draw();
}

// 衝突検知時の処理
void ChargeGun::OnCollision([[maybe_unused]] Collider* other){
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	//衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemyBullet)) {
		EnemyBullet* enemyBullet = static_cast<EnemyBullet*>(other);
		Vector3 distance = enemyBullet->GetCenterPosition() - player_->GetWorldPosition();
		if (player_) {
			player_->Damage(1, distance.Normalize());
		}
	}
}

void ChargeGun::Charge(){
	BaseWeapon::Charge();
	GlobalVariables* gv = GlobalVariables::GetInstance();
	const std::string groupName = "ChargeGun";
	AttackData& attackData = attackData_[static_cast<uint32_t>(AttackType::kCharge)];
	attackData.bulletSize = std::clamp(chargeCount_, 0.0f, gv->GetValue<float>(groupName, "ChargeLimitSize"));
}

} // namespace Engine
