#include "Skydome.h"
#include <cassert>

Skydome::~Skydome() { delete objectColor_; }

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {3.0f, 3.0f, 3.0f};
	worldTransform_.UpdateMatrix();
	viewProjection_ = viewProjection;
	objectColor_ = new ObjectColor;
	objectColor_->Initialize();
	objectColor_->SetColor({0.7f, 0.7f, 0.7f, 1});
	objectColor_->TransferMatrix();
}

void Skydome::Update() {}

void Skydome::Draw() { model_->Draw(worldTransform_, *viewProjection_, objectColor_);
}

