#include "BaseEventObject.h"

namespace Engine {

void BaseEventObject::Initialize() {
	Collider::Initialize();
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEvent));
	Collider::SetRadius(1.0f);
	
	// デバッグ用モデルなどが必要ならここで設定
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	// 仮のモデルを設定 (必要に応じて変更)
	// object3d_->SetModel("sphere/sphere.obj");
}

void BaseEventObject::Update() {
	object3d_->Update();
}

void BaseEventObject::Draw() {
	object3d_->Draw();
}

void BaseEventObject::OnCollision([[maybe_unused]] Collider* other) {
	// オーバーライドして使用
}

Vector3 BaseEventObject::GetCenterPosition() const {
	return object3d_->GetCenterPosition();
}

} // namespace Engine
