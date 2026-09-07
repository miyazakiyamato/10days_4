#include "Collider.h"

void Collider::Initialize() { worldTransform.Initialize(); }

void Collider::UpdateWorldTransform() {
	//ワールド座標をワールドトランスフォームに適用
	worldTransform.translation_ = GetCenterPosition();
	worldTransform.scale_ = {
	    GetRadius(), GetRadius(), GetRadius()
	};
	worldTransform.UpdateMatrix();
}

void Collider::Draw(Model* model, const ViewProjection& viewProjection) { model->Draw(worldTransform, viewProjection); }
