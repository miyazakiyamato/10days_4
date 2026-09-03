#include "PlayerStateClear.h"
#include "Player.h"
#include "Input.h"
#include "TimeManager.h"
#include "PlayerStateRoot.h"
#include "PlayerBullet.h"

namespace Engine {

// 初期化
void PlayerStateClear::Initialize(){
	input_ = Input::GetInstance();
	timeManager_ = TimeManager::GetInstance();
	player_ = static_cast<Player*>(character_);
}

// 更新
void PlayerStateClear::Update(){
	Object3d* playerObject = player_->GetObject3d();
	// 回転スピード
	float revolveSpeed = 1.0f * 3.141592f / 180.0f; // 1度/フレーム
	float rotateSpeed = revolveSpeed;             // 1周ごとに1回転 → 公転と同じ増え方


	// 公転
	revolveAngle_ += revolveSpeed;

	// 半径
	float radius = 5.0f;

	// 位置 = 原点から見た円運動
	Vector3 newPos = {
		radius * std::sin(revolveAngle_) + 8.0f,
		1.0f,
		radius * std::cos(revolveAngle_) + 30.0f
	};
	Vector3 nextPos = {
		radius * std::sin(revolveAngle_ + revolveSpeed) + 8.0f,
		1.0f,
		radius * std::cos(revolveAngle_ + revolveSpeed) + 30.0f
	};

	// プレイヤーの位置を更新
	playerObject->SetTranslate(newPos);
	// 向きの更新
	Vector3 direction = Vector3::Subtract(nextPos, newPos);
	Vector3 rotate = playerObject->GetRotate();
	rotate.y = std::atan2f(direction.x, direction.z);
	Vector3 velocityZ = Matrix4x4::Transform(direction, Matrix4x4::MakeRotateYMatrix(-rotate.y));
	rotate.x = std::atan2f(-velocityZ.y, velocityZ.z);
	playerObject->SetRotate(rotate);
}

} // namespace Engine
