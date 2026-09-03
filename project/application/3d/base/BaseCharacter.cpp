#include "BaseCharacter.h"
#include "TimeManager.h"
#include <numbers>

namespace Engine {

void BaseCharacter::Initialize(){
	Collider::Initialize();
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
}

BaseCharacter::~BaseCharacter() {
}

void BaseCharacter::Update(){
	Shaking();
	object3d_->Update();
}

void BaseCharacter::ChangeState(std::unique_ptr<BaseCharacterState> newState) {
	state_ = std::move(newState);
	state_->Initialize();
}

Vector3 BaseCharacter::GetCenterPosition() const{
	return object3d_->GetCenterPosition();
}

void BaseCharacter::ShakeStart(Quaternion shakeQuaternion, float kTime) {
	shake_.shakeQuaternion = shakeQuaternion;
	shake_.preShakeQuaternion = {};
	shake_.kTime = kTime;
	shake_.time = 0.0f;
	shake_.isShake = true;
}

void BaseCharacter::DamageKnockbackStart(const Vector3& hitDirection, float power, float duration)
{
	// 現在の回転を保存
	if (!shake_.isShake) {
		shake_.startQuaternion = object3d_->GetRotate().ToQuaternion();
	}
	// ダメージ方向に倒す回転
	Vector3 dir = hitDirection.Normalize();

	// 回転軸＝ダメージ方向と「上方向」の外積
	Vector3 axis = Vector3::Cross(dir, { 0,1,0 });
	if (axis.Length() < 0.0001f) axis = { 1,0,0 }; // 真上/真下対策
	axis.Normalize();

	// power（何度傾けるか）をラジアンに変換
	float rad = power * (3.14159f / 180.0f);

	Quaternion damageRot = Quaternion::MakeRotateAxisAngle(axis, rad).ToQuaternion();

	// 最終的に倒れるQuaternion = 現在 * damageRot
	Quaternion target = shake_.startQuaternion * damageRot;

	// Shake 開始
	ShakeStart(target, duration);
}

void BaseCharacter::Shaking() {
	if (!shake_.isShake) { return; }

	// TimeManager::deltaTime_ が静的メンバかどうか確認したが、
	// BaseEnemy.cpp等は TimeManager::deltaTime_ と書いている。
	// TimeManager.h を見ると static float deltaTime_; があると推測される。
	// 安全のため GetInstance() を使うか、既存コードに合わせる。
	// 既存コード合わせ: shake_.time += TimeManager::deltaTime_;
	// ただしヘッダーを見ていないので、GetInstance()->deltaTime_ が安全。
	// HpUIではGetInstance()->deltaTime_だった。
	// BaseEnemyはTimeManager::deltaTime_だった。
	// おそらく static メンバがある。
	
	// ここでは GetInstance() を使うのが確実。
	shake_.time += TimeManager::GetInstance()->deltaTime_;
	float t = shake_.time / shake_.kTime;

	if (t >= 1.0f) {
		// 回転を元に戻す
		object3d_->SetRotate(shake_.startQuaternion.ToEulerAngles());
		shake_.isShake = false;
		return;
	}

	// 線形補間で徐々に倒れ → 元に戻る
	float ease = (t < 0.5f)
		? (t * 2.0f)          // 0 → 1（倒れる）
		: (1.0f - (t - 0.5f) * 2.0f); // 1 → 0（戻る）

	Quaternion q = Quaternion::Slerp(
		shake_.startQuaternion,
		shake_.shakeQuaternion,
		ease
	);

	object3d_->SetRotate(q.ToEulerAngles());
}

} // namespace Engine
