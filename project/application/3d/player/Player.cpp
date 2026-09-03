#include "Player.h"
#include "Input.h"
#include "CameraManager.h"
#include <algorithm>
#include "BulletManager.h"
#include "AudioManager.h"
#include "CollisionTypeIdDef.h"
#include "PlayerBullet.h"
#include "ParticleSystem.h"
#include "EmitterSphere.h"
#include "EnemyBullet.h"
#include "RailCamera.h"
#include "TimeManager.h"
#include "PlayerStateRoot.h"
#include "ChargeGun.h"
#include "Object3d.h"
#include "GlobalVariables.h"

namespace Engine {

void Player::Initialize(){
	BaseCharacter::Initialize();
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));
	input_ = Input::GetInstance();

	// GlobalVariablesに初期値を登録
	GlobalVariables* gv = GlobalVariables::GetInstance();
	const std::string groupName = "Player";
	gv->CreateGroup(groupName);
	gv->AddItem(groupName, "InitialTranslate", Vector3(0.0f, 0.0f, 30.0f));
	gv->AddItem(groupName, "ReticleInitialTranslate", Vector3(0.0f, 0.0f, 10.0f));
	gv->AddItem(groupName, "MoveSpeed", Vector3(1.5f, 3.0f, 10.0f));
	gv->AddItem(groupName, "MoveLimit", Vector3(10.0f, 7.0f, 100.0f));
	gv->AddItem(groupName, "AirResistance", 0.9f);
	gv->AddItem(groupName, "Hp", 10);
	gv->AddItem(groupName, "NormalSpeed", 15.0f);
	gv->AddItem(groupName, "ShiftSpeed", 30.0f);
	gv->AddItem(groupName, "JumpPower", 40.0f);
	gv->AddItem(groupName, "ReticleSpeed", 10.0f);
	gv->AddItem(groupName, "ReticleDistance", 100.0f);
	gv->AddItem(groupName, "DamageKnockbackAngle", 30.0f);
	gv->AddItem(groupName, "DamageKnockbackTime", 0.3f);
	gv->AddItem(groupName, "ColliderRadius", 1.0f);

	// 値の適用
	Collider::SetRadius(gv->GetValue<float>(groupName, "ColliderRadius"));
	hp_ = gv->GetValue<int>(groupName, "Hp");
	airResistance = gv->GetValue<float>(groupName, "AirResistance");
	moveSpeed_ = gv->GetValue<Vector3>(groupName, "MoveSpeed");
	moveLimit_ = gv->GetValue<Vector3>(groupName, "MoveLimit");

	object3d_->SetModel("drone/drone.obj");
	object3d_->SetTranslate(gv->GetValue<Vector3>(groupName, "InitialTranslate"));
	object3d_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f }, 0);
	object3d_->Update();

	//レティクル3D
	reticle3d_ = std::make_unique<Object3d>();
	reticle3d_->Initialize();
	reticle3d_->SetModel("sphere/sphere.obj");
	reticle3d_->SetTranslate(gv->GetValue<Vector3>(groupName, "ReticleInitialTranslate"));
	reticle3d_->Update();

	//手
	hand_ = std::make_unique<Object3d>();
	hand_->Initialize();
	hand_->SetTranslate(handOffset_);
	hand_->SetParent(object3d_.get());
	hand_->Update();

	//プレイヤーの初期行動ステート
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(object3d_.get());
	ChangeState(std::make_unique<PlayerStateRoot>(this));
}

void Player::Update(){
	// 親（レールカメラ）の更新を反映させるために行列のみ更新
	object3d_->Update();
	hand_->Update();
	if (weapon_) {
		weapon_->Update();
	}

	followCamera_->Update();
	state_->Update();
	BaseCharacter::Update();
}

void Player::Draw(){
	object3d_->Draw();
	if (weapon_) {
		weapon_->Draw();
	}
}

void Player::OnCollision([[maybe_unused]] Collider* other){
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	//衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemyBullet)) {
		EnemyBullet* enemyBullet = static_cast<EnemyBullet*>(other);
		Vector3 distance = enemyBullet->GetCenterPosition() - object3d_->GetCenterPosition();
		Damage(1, distance.Normalize());
	}
	
}

void Player::Move(){// 移動量
	Vector3 direction = {};
	acceleration_ = {};

	direction.x += input_->GetControllerStickLX();
	direction.y += input_->GetControllerStickLY();

	GlobalVariables* gv = GlobalVariables::GetInstance();
	const std::string groupName = "Player";
	// リアルタイム調整の反映
	airResistance = gv->GetValue<float>(groupName, "AirResistance");
	moveSpeed_ = gv->GetValue<Vector3>(groupName, "MoveSpeed");
	moveLimit_ = gv->GetValue<Vector3>(groupName, "MoveLimit");

	if (direction.Length() == 0) {
		if (input_->PushKey(DIK_A)) {
			direction.x -= 1.0f;
		}
		if (input_->PushKey(DIK_D)) {
			direction.x += 1.0f;
		}
		if (input_->PushKey(DIK_W)) {
			direction.y += 1.0f;
		}
		if (input_->PushKey(DIK_S)) {
			direction.y -= 1.0f;
		}
	}

	if (isCpuMode_) {
		direction = {};
	}
	if (input_->PushKey(DIK_LSHIFT) || input_->PushControllerButton(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		if (!isCpuMode_) {
			railCamera_->SetVelocity(gv->GetValue<float>(groupName, "ShiftSpeed"));
		}
	} else {
		railCamera_->SetVelocity(gv->GetValue<float>(groupName, "NormalSpeed"));
	}

	if (direction.Length() != 0.0f) {
		direction.Normalize();
		acceleration_.x += direction.x * moveSpeed_.x;
		acceleration_.y += direction.y * moveSpeed_.y;
	}
	
	// 移動処理
	velocity_ += acceleration_;
	velocity_ *= airResistance;
	velocity_ = Vector3::Clamp(velocity_, -velocityLimit_, velocityLimit_);
	Vector3 objectPosition = object3d_->GetTranslate();
	objectPosition += velocity_ * TimeManager::GetInstance()->deltaTime_;

	if (objectPosition.x < -moveLimit_.x || objectPosition.x > moveLimit_.x) {
		velocity_.x = 0.0f;
	}
	if (objectPosition.y < -moveLimit_.y || objectPosition.y > moveLimit_.y) {
		velocity_.y = 0.0f;
	}
	objectPosition = Vector3::Clamp(objectPosition, -moveLimit_, moveLimit_);

	object3d_->SetTranslate(objectPosition);
	object3d_->SetTranslate(objectPosition);
	// 地面での跳ね返り処理
	float jumpPower = gv->GetValue<float>(groupName, "JumpPower");
	bool isHitGround = object3d_->GetCenterPosition().y < GetRadius();
	if (weapon_) {
		if (weapon_->GetCenterPosition().y < weapon_->GetRadius()) {
			isHitGround = true;
		}
	}
	if (isHitGround) {
		velocity_.y = jumpPower;
		Damage(1, Vector3(1.0f, 0.0f, 1.0f).Normalize());
	}
	
	static_cast<EmitterSphere*>(particleSystem_->FindEmitter("airEffect"))->SetTranslate(object3d_->GetCenterPosition());
}

void Player::ReticleUpdate(){
	GlobalVariables* gv = GlobalVariables::GetInstance();
	const std::string groupName = "Player";

	Vector2 move{};
	float speed = gv->GetValue<float>(groupName, "ReticleSpeed");

	move.x += input_->GetControllerStickRX();
	move.y -= input_->GetControllerStickRY();

	if (move.Length() == 0) {
		if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			move += { -1.0f, 0.0f};
		}
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			move += { 1.0f, 0.0f};
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			move += { 0.0f, -1.0f };
		}
		if (Input::GetInstance()->PushKey(DIK_DOWN)) {
			move += { 0.0f, 1.0f};
		}
	}

	if (isCpuMode_) {
		move = {};
	}
	if (move.Length() != 0) {
		move.Normalize();
		move *= speed;
		Vector2 spritePosition = reticleUI_->GetPosition();
		spritePosition += move;
		spritePosition.x = std::clamp(spritePosition.x, 0.0f, (float)WinApp::kClientWidth);
		spritePosition.y = std::clamp(spritePosition.y, 0.0f, (float)WinApp::kClientHeight);
		reticleUI_->SetPosition(spritePosition);
	}
	//マウスの位置から3Dレティクルの位置を計算
	Matrix4x4 matViewport = Matrix4x4::MakeViewportMatrix(0, 0, WinApp::kClientWidth, WinApp::kClientHeight, 0, 1);
	Matrix4x4 matVPV = Matrix4x4(followCamera_->GetCamera()->GetViewMatrix()) * followCamera_->GetCamera()->GetProjectionMatrix() * matViewport;
	Matrix4x4 matInverseVPV = Matrix4x4::Inverse(matVPV);
	Vector3 posNear = Vector3(reticleUI_->GetPosition().x, reticleUI_->GetPosition().y, 0);
	Vector3 posFar = Vector3(reticleUI_->GetPosition().x, reticleUI_->GetPosition().y, 1);
	posNear = Matrix4x4::Transform(posNear, matInverseVPV);
	posFar = Matrix4x4::Transform(posFar, matInverseVPV);
	Vector3 mouseDirection = Vector3::Subtract(posFar, posNear);
	mouseDirection = Vector3::Normalize(mouseDirection);
	
	//　3Dレティクルの位置を更新
	float distanceTestObject = gv->GetValue<float>(groupName, "ReticleDistance");
	reticle3d_->SetTranslate(Vector3::Add(posNear, Vector3::Multiply(distanceTestObject, mouseDirection)));
	reticle3d_->Update();
	reticleUI_->Update();
}

void Player::StopCameraFollow() {
	if (followCamera_) {
		followCamera_->SetTarget(nullptr);
	}
}

void Player::Damage(int damage, const Vector3& hitDirection){
	static_cast<EmitterSphere*>(particleSystem_->FindEmitter("hitEffect"))->SetTranslate(object3d_->GetCenterPosition() + hitDirection * GetRadius());
	particleSystem_->Emit("hitEffect");
	//railCamera_->ShakeStart({ 5.0f + velocity_.Length() / 10.0f,5.0f + velocity_.Length() / 10.0f }, 0.3f);
	followCamera_->ShakeStart({ 5.0f + velocity_.Length() / 10.0f,5.0f + velocity_.Length() / 10.0f }, 0.3f);
	GlobalVariables* gv = GlobalVariables::GetInstance();
	const std::string groupName = "Player";
	DamageKnockbackStart(
		hitDirection,     // ダメージ方向
		gv->GetValue<float>(groupName, "DamageKnockbackAngle"),      // どれくらい倒すか（度数）
		gv->GetValue<float>(groupName, "DamageKnockbackTime")       // 戻るまでの時間
	);

	hp_ -= damage;
	if (hp_ <= 0) {
		isAlive_ = false;
		hp_ = 0;
	}
}

Vector3 Player::GetWorldPosition(){
	return object3d_->GetCenterPosition();
}

void Player::SetParent(Object3d* object3d){
	object3d_->SetParent(object3d);
}

void Player::SetWeapon(std::unique_ptr<BaseWeapon> weapon){
	weapon_ = std::move(weapon);
	weapon_->SetPlayer(this);
	weapon_->GetObject3d()->SetParent(hand_.get());
	weapon_->SetTarget(reticle3d_.get());
	weapon_->Update();
}

void Player::SetRailCamera(RailCamera* railCamera) {
	railCamera_ = railCamera;
	if (followCamera_) {
		followCamera_->SetRotationTarget(railCamera_->GetObject3d());
	}
}

} // namespace Engine
