#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "TitleScene.h"
#include "StageSelect.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "WinApp.h"

TitleScene* titleScene = nullptr;
StageSelect* stageSelectScene = nullptr;
GameScene* gameScene = nullptr;

enum class Scene {
	kUnknown = 0,

	kTitle,
	kStageSelect,
	kGame,
};
//現在のシーン
Scene scene = Scene::kUnknown;

void ChangeScene() {
	int nowStageNum;
	switch (scene) {
	case Scene::kUnknown:
		break;
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			// シーン変更
			scene = Scene::kStageSelect;
			// 旧シーンの解放
			delete titleScene;
			titleScene = nullptr;
			// 新シーンの生成初期化
			delete stageSelectScene;
			stageSelectScene = new StageSelect;
			stageSelectScene->Initialize();
		}
		break;
	case Scene::kStageSelect:
		if (stageSelectScene->IsFinished()) {
			nowStageNum = stageSelectScene->GetStageNum();
			// シーン変更
			scene = Scene::kGame;
			// 旧シーンの解放
			delete stageSelectScene;
			stageSelectScene = nullptr;
			// 新シーンの生成初期化
			delete gameScene;
			gameScene = new GameScene;
			gameScene->SetStageNum(nowStageNum);
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene->IsFinished()) {
			nowStageNum = gameScene->GetStageNum();
			// シーン変更
			if (gameScene->GetIsReturnSelect()) {
				scene = Scene::kTitle;
				delete gameScene;
				gameScene = new GameScene;
				gameScene->SetStageNum(nowStageNum);
				gameScene->Initialize();
				break;
			}

			if (!gameScene->GetIsClear()) {
				scene = Scene::kGame;
				delete gameScene;
				gameScene = new GameScene;
				gameScene->SetStageNum(nowStageNum);
				gameScene->Initialize();
				break;
			}
			scene = Scene::kStageSelect;
			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;
			// 新シーンの生成初期化
			delete stageSelectScene;
			stageSelectScene = new StageSelect;
			stageSelectScene->SetStageNum(nowStageNum);
			stageSelectScene->Initialize();
		}
		break;
	default:
		break;
	}
}

void UpdateScene() {
	switch (scene) {
	case Scene::kUnknown:
		break;
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kStageSelect:
		stageSelectScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	default:
		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kUnknown:
		break;
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kStageSelect:
		stageSelectScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	default:
		break;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"4042_道る記憶");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();
	uint32_t bgm = audio->LoadWave("B00203_kamatamago_Chick-flying-in-the-sky.wav");
	audio->PlayWave(bgm, true);

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion
	//最初のシーンの初期化
	scene = Scene::kTitle;

	#ifdef _DEBUG
	scene = Scene::kStageSelect;
	#endif // _DEBUG


	titleScene = new TitleScene();
	titleScene->Initialize();
	
	stageSelectScene = new StageSelect;
	stageSelectScene->Initialize();

	// ゲームシーンの初期化
	//gameScene = new GameScene();
	//gameScene->Initialize();
	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();
		//シーンの毎フレーム処理
		ChangeScene();
		UpdateScene();

		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();
		//シーンの描画
		DrawScene();
		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}
	// 各種解放
	delete titleScene;
	delete gameScene;
	delete stageSelectScene;
	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}