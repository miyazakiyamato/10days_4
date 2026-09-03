#include "Ground.h"
#include "TextureManager.h"

namespace Engine {
	namespace {
		const Vector3 kGroundTranslate = { -1,0,0 };
		const Vector3 kGroundRotate = { 0,3.14f,0 };
		const Vector3 kGroundSize = { 1.0f,1.0f,1.0f };
		const Vector3 kGroundUvScale = { 100.0f, 100.0f,1.0f };
		const int kTexNum = 0;
	}
void Ground::Initialize() {
	object3d = std::make_unique<Object3d>();
	object3d->Initialize();
	object3d->SetModel("ground/ground.obj");
	object3d->SetTranslate(kGroundTranslate);
	object3d->SetRotate(kGroundRotate);
	object3d->SetScale(kGroundSize);
	object3d->SetUvScale(kGroundUvScale, kTexNum);
	object3d->Update();
	std::string objectName = "ground";
}

void Ground::Update() {
	object3d->Update();
}

void Ground::Draw() {
	object3d->Draw();
}

} // namespace Engine
