#pragma once
#include "Camera.h"
#include "Object3d.h"

namespace Engine {

// 前方宣言
class LockOn;

/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera {
public:
	struct Shake {
		Vector2 move;
		Vector2 preMove;
		float kTime;
		float time;
		bool isShake = false;
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void Reset();

	void ShakeStart(Vector2 move, float kTime);
private:
	void ApplyGlobalVariables();
	void Shaking();
	// 追従対象からのオフセットを計算する
	Vector3 MakeOffset();

	// カメラ
	Camera* camera_ = nullptr;
	std::unique_ptr<Object3d> object3d_;

	// 追従対象
	const Object3d* target_ = nullptr;
	const Object3d* rotationTarget_ = nullptr;
	// 追従対象の残像座標（補間用）
	Vector3 interTarget_{};
	// 追従対象の残像回転（補間用）
	Quaternion interRotation_{};
	
	// 追従対象からのオフセット
	Vector3 offset_ = { 0.0f, 5.0f, -10.0f };
	Vector3 rotateOffset_ = { 0.0f, 0.0f, 0.0f };

	Shake shake_{};
	// ロックオン
	const LockOn* lockOn_ = nullptr;

public:
		// --- ゲッター・セッター ---
	const Camera* GetCamera() const { return camera_; }
	void SetTarget(const Object3d* target);
	void SetRotationTarget(const Object3d* target) { rotationTarget_ = target; }
	void SetLockOn(const LockOn* lockOn) { lockOn_ = lockOn; }
};

} // namespace Engine
