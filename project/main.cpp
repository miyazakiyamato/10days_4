#include "Framework.h"
#include "MyGame.h"

using namespace Engine;

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	//リークチェック
	D3DResourceLeakChecker leakCheck;
	std::unique_ptr<Framework> game = std::make_unique<MyGame>();

	game->Run();

	return 0;
}
