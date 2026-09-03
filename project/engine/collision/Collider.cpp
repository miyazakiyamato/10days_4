#include "Collider.h"
#include "ModelManager.h"

namespace Engine {

void Collider::Initialize() { 
	colliderModel_ = std::make_unique<Object3d>();
	colliderModel_->Initialize();
	ModelManager::GetInstance()->LoadModel("sphere/sphere.obj");
	colliderModel_->SetModel("sphere/sphere.obj");
}

void Collider::UpdateWorldTransform() {
	//ワールド座標をワールドトランスフォームに適用
	colliderModel_->SetTranslate(GetCenterPosition());
	colliderModel_->SetScale({ GetRadius(),GetRadius(),GetRadius() });
	colliderModel_->Update();
}

void Collider::Draw(){
	colliderModel_->Draw();
}

} // namespace Engine
