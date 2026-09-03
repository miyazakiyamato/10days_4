#include "SmallDrone.h"

namespace Engine {

void SmallDrone::Initialize(){
	BaseEnemy::Initialize();
	object3d_->SetModel("drone/drone.obj");
	object3d_->SetRotate({ 0,3.14f,0 });
	object3d_->Update();
}

void SmallDrone::Update(){
	BaseEnemy::Update();
}

void SmallDrone::Draw(){
	object3d_->Draw();
}

void SmallDrone::OnCollision(Collider* other){
	BaseEnemy::OnCollision(other);
}

} // namespace Engine
