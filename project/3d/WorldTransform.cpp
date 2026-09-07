#include "WorldTransform.h"
#include "MyMtMatrix.h"

void WorldTransform::UpdateMatrix() {
	matWorld_ = MyMtMatrix::MakeAffineMatrix(scale_,rotation_,translation_);
	TransferMatrix();
}