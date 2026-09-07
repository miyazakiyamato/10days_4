#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Skydome {
public:
	~Skydome();

	void Initialize(Model* model, ViewProjection* viewProjection);
	void Update();
	void Draw();

private:
	ObjectColor* objectColor_;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	Model* model_ = nullptr;
};
