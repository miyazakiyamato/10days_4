#include "Skydome.h"
#include "TextureManager.h"

namespace Engine {
	namespace {
		const Vector3 kSkydomeTranslate = { -1,0,0 };
		const Vector3 kSkydomeRotate = { 0,3.14f,0 };
		const Vector3 kSkydomeSize = { 10.0f,10.0f,10.0f };
		const int kTexNum = 0;
		const bool kIsEnableLighting = false;
	}
void Skydome::Initialize(){
	object3d = std::make_unique<Object3d>();
	object3d->Initialize();
	object3d->SetModel("skydome/skydome.obj");
	object3d->SetEnableLighting(kIsEnableLighting, kTexNum);
	object3d->SetTranslate(kSkydomeTranslate);
	object3d->SetRotate(kSkydomeRotate);
	object3d->SetScale(kSkydomeSize);
	object3d->Update();
	std::string objectName = "skydome";
}

void Skydome::Update(){
	object3d->Update();
}

void Skydome::Draw(){
	object3d->Draw();
}

} // namespace Engine
