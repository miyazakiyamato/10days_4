#pragma once
#define _USE_MATH_DEFINES
#include <dxgidebug.h>
#include <cmath>
#include <vector>
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI

#include "D3DResourceLeakChecker.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

#include "WinApp.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "PipelineManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Logger.h"
#include "Camera.h"
#include "CameraManager.h"
#include "SrvUavManager.h"
#include "ImGuiManager.h"
#include "GlobalVariables.h"
#include "AudioManager.h"
#include "SceneManager.h"
#include "AbstractSceneFactory.h"
#include "ParticleManager.h"
#include "LightManager.h"
#include "TimeManager.h"
#include "Line3D.h"
#include "PostEffectManager.h"

namespace Engine {

/// <summary>
/// ゲームフレームワーク基底クラス
/// アプリケーションのライフサイクル（初期化、メインループ、終了）と
/// 各種マネージャクラスの統合管理を行う
/// </summary>
class Framework {
public:
		// --- メンバ関数 ---
	virtual ~Framework() = default;

	/// <summary>
	/// 初期化処理
	/// ウィンドウ生成、DirectX初期化、各マネージャの生成と初期化を行う
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 終了処理
	/// 確保したリソースやマネージャの解放を行う
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// 毎フレーム更新
	/// 入力、時間、各マネージャ、シーンの更新処理を行う
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理 (純粋仮想関数)
	/// 派生クラスで具体的な描画パスを実装する
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// アプリケーション実行
	/// ウィンドウが閉じられるまでメインループを回す
	/// </summary>
	void Run();

protected:
		// --- メンバ変数 ---
	// ポインタ
	// WindowsAPI
	std::unique_ptr<WinApp> winApp = nullptr;
	// DirecX
	std::unique_ptr<DirectXCommon> dxCommon = nullptr;
	// 入力
	Input* input_ = nullptr;
	// SRV/UAV
	std::unique_ptr<SrvUavManager> srvUavManager = nullptr;

	// マネージャ群
	std::unique_ptr<ImGuiManager> imGuiManager = nullptr;

	GlobalVariables* globalVariables_ = nullptr;
	// パイプラインマネージャ
	PipelineManager* pipelineManager = nullptr;
	// テクスチャマネージャ
	TextureManager* textureManager_ = nullptr;
	// パーティクルマネージャ
	ParticleManager* particleManager_ = nullptr;
	// モデルマネージャ
	ModelManager* modelManager_ = nullptr;
	// ラインマネージャ
	Line3dManager* lineManager_ = nullptr;
	// ライトマネージャ
	LightManager* lightManager_ = nullptr;
	// カメラマネージャ
	CameraManager* cameraManager_ = nullptr;
	// オーディオマネージャ
	AudioManager* audioManager_ = nullptr;
	// シーンマネージャ
	SceneManager* sceneManager_ = nullptr;
	// シーンファクトリー
	std::unique_ptr<AbstractSceneFactory> sceneFactory_ = nullptr;
	// シーン遷移エフェクトファクトリー
	std::unique_ptr<AbstractTransitionFactory> transitionFactory_ = nullptr;
	// タイムマネージャ
	TimeManager* timeManager_ = nullptr;
	// ポストエフェクトマネージャ
	PostEffectManager* postEffectManager_ = nullptr;
};

} // namespace Engine
