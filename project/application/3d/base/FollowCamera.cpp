#include "FollowCamera.h"
#include <cmath>
#include "CameraManager.h"
#include "TimeManager.h"
#include "GlobalVariables.h"
#include "Quaternion.h"
#include "Vector3.h"

namespace Engine {

void FollowCamera::Initialize() {
	// Object3d作成 (RailCameraと同様)
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	
	// カメラの登録と取得
	CameraManager::GetInstance()->SetCamera("FollowCamera");
	CameraManager::GetInstance()->FindCamera("FollowCamera");
	camera_ = CameraManager::GetInstance()->GetCamera();
	
	// グローバル変数の初期化
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "offset", offset_);
	globalVariables->AddItem(groupName, "rotateOffset", rotateOffset_);

	// 初期値をロード
	offset_ = globalVariables->GetValue<Vector3>(groupName, "offset");
	rotateOffset_ = globalVariables->GetValue<Vector3>(groupName, "rotateOffset");
	
	// インターポレーション変数の初期化 (QuaternionはIdentityで初期化)
	interRotation_ = Quaternion::Identity();
}

void FollowCamera::Update() {
	ApplyGlobalVariables();

	if (target_) {
		// 回転の補間
		// ターゲットのワールド行列を取得
		Matrix4x4 worldMat = target_->GetWorldMatrix();
		
		// 位置の補間
		// ターゲットのワールド座標へ徐々に近づく (Lerp)
		Vector3 targetWorldPos = { worldMat.m[3][0], worldMat.m[3][1], worldMat.m[3][2] };
		float positionLerpFactor = 0.2f;
		interTarget_ = Vector3::Lerp(interTarget_, targetWorldPos, positionLerpFactor);

		// ターゲットのワールド回転を取得
		Quaternion targetRotation; 
		if (rotationTarget_) {
			targetRotation = rotationTarget_->GetWorldMatrix().ToQuaternion();
		} else {
			targetRotation = worldMat.ToQuaternion();
		}
		
		float rotationLerpFactor = 0.1f;
		interRotation_ = Quaternion::Slerp(interRotation_, targetRotation, rotationLerpFactor);

		// カメラ座標と回転の決定
		// オフセット回転（Euler -> Quaternion）
		Quaternion rotateOffsetQuat = Quaternion::MakeRotateAxisAngleQuaternion({ 1,0,0 }, rotateOffset_.x) *
			Quaternion::MakeRotateAxisAngleQuaternion({ 0,1,0 }, rotateOffset_.y) *
			Quaternion::MakeRotateAxisAngleQuaternion({ 0,0,1 }, rotateOffset_.z);
			
		// 最終的な回転は、補間回転 * オフセット回転
		Quaternion finalRotation = interRotation_ * rotateOffsetQuat;
		
		Vector3 offset = Quaternion::RotateVector(offset_, finalRotation);
		
		object3d_->SetRotate(finalRotation.ToEulerAngles());
		object3d_->SetTranslate(interTarget_ + offset);
	}

	// 振動処理
	Shaking();

	// カメラへの反映
	if (camera_) {
		camera_->SetTranslate(object3d_->GetTranslate());
		camera_->SetRotate(object3d_->GetRotate());
		camera_->Update();
	}
	
	object3d_->Update();
}

void FollowCamera::Reset() {
	if (target_) {
		Matrix4x4 worldMat = target_->GetWorldMatrix();
		interTarget_ = { worldMat.m[3][0], worldMat.m[3][1], worldMat.m[3][2] };
		// 回転も初期化
		Quaternion targetRotation;
		if (rotationTarget_) {
			targetRotation = rotationTarget_->GetWorldMatrix().ToQuaternion();
		} else {
			targetRotation = worldMat.ToQuaternion();
		}
		interRotation_ = targetRotation;
	}
	
	// オフセット回転（Euler -> Quaternion）
	Quaternion rotateOffsetQuat = Quaternion::MakeRotateAxisAngleQuaternion({ 1,0,0 }, rotateOffset_.x) *
		Quaternion::MakeRotateAxisAngleQuaternion({ 0,1,0 }, rotateOffset_.y) *
		Quaternion::MakeRotateAxisAngleQuaternion({ 0,0,1 }, rotateOffset_.z);

	// 最終的な回転は、補間回転 * オフセット回転
	Quaternion finalRotation = interRotation_ * rotateOffsetQuat;

	Vector3 offset = Quaternion::RotateVector(offset_, finalRotation);

	if (object3d_) {
		object3d_->SetTranslate(interTarget_ + offset);
		object3d_->SetRotate(finalRotation.ToEulerAngles());
	}
}

void FollowCamera::ShakeStart(Vector2 move, float kTime) {
	shake_.move = move;
	shake_.preMove = {};
	shake_.kTime = kTime;
	shake_.time = 0.0f;
	shake_.isShake = true;
}

void FollowCamera::Shaking() {
	if (!shake_.isShake) {
		return;
	}
	shake_.time += TimeManager::deltaTime_;
	
	object3d_->SetTranslate(object3d_->GetTranslate() - Vector3(shake_.preMove.x, shake_.preMove.y, 0));

	float timeCount = shake_.time / shake_.kTime;
	if (timeCount > 1.0f) {
		shake_.isShake = false;
		return;
	}

	static std::random_device rd;
	static std::mt19937 gen(rd());
	float shakeSize = 0;
	if (timeCount <= 0.5f) {
		shakeSize = timeCount * 2.0f;
	} else {
		shakeSize = 1.0f - (timeCount * 2.0f - 0.5f);
	}
	static std::uniform_real_distribution<float> distX(-shake_.move.x * shakeSize, shake_.move.x * shakeSize);
	static std::uniform_real_distribution<float> distY(-shake_.move.y * shakeSize, shake_.move.y * shakeSize);

	static std::uniform_real_distribution<float> distZ(-shake_.move.x * shakeSize, shake_.move.x * shakeSize);

	shake_.preMove = { distX(gen), distY(gen) };
	
	// 今回の振動分を加算
	object3d_->SetTranslate(object3d_->GetTranslate() + Vector3(shake_.preMove.x, shake_.preMove.y, 0));
}

Vector3 FollowCamera::MakeOffset() {
	// オフセットをカメラの回転(interRotation_)に合わせて回転させる
	return Quaternion::RotateVector(offset_, interRotation_);
}

void FollowCamera::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";
	offset_ = globalVariables->GetValue<Vector3>(groupName, "offset");
	rotateOffset_ = globalVariables->GetValue<Vector3>(groupName, "rotateOffset");
}

void FollowCamera::SetTarget(const Object3d* target){
	target_ = target;
	// ターゲットが設定された場合のみリセットを行う（外れた場合はその場にとどまるためリセットしない）
	if (target_) {
		Reset();
	}
}

} // namespace Engine
