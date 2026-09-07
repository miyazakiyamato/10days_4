#include "CameraController.h"
#include "Player.h"
#include "MyMtMatrix.h"
#include <algorithm>

void CameraController::Initialize(ViewProjection* viewProjection) {
	viewProjection_ = viewProjection;
	viewProjection_->translation_ = MyMtVector3::Add(viewProjection_->translation_, targetOffset_);
	viewProjection_->UpdateMatrix();
}

void CameraController::Update() { Reset(); }

void CameraController::Reset() {
	////追従対象のワールドトランスフォームを参照
	//const WorldTransform& targetWorldTransform = *target_->GetWorldTransform();
	////追従対象とオフセットからカメラの座標を計算
	//targetPosition_ = MyMtVector3::Add(MyMtVector3::Add(targetWorldTransform.translation_, targetOffset_), MyMtVector3::Multiply(kVelocityBias, {target_->GetVelocity().x,}));
	//
	////座標補完によりゆったり追従
	//viewProjection_->translation_ = MyMtVector3::Lerp(viewProjection_->translation_, targetPosition_, kInterpolationRate);
	////追従対象が画面外に出ないように補正
	//viewProjection_->translation_.x = std::clamp(viewProjection_->translation_.x, target_->GetWorldPosition().x + margin.left, target_->GetWorldPosition().x + margin.right);
	//viewProjection_->translation_.y = std::clamp(viewProjection_->translation_.y, target_->GetWorldPosition().y + margin.bottom, target_->GetWorldPosition().y + margin.top);

	////移動範囲制限
	//viewProjection_->translation_.x = std::clamp(viewProjection_->translation_.x, movableArea_.left, movableArea_.right);
	//viewProjection_->translation_.y = std::clamp(viewProjection_->translation_.y, movableArea_.bottom, movableArea_.top);

	viewProjection_->UpdateMatrix();

}
