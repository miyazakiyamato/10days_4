#pragma once
#include <memory>
#include <unordered_map>
#include "Camera.h"

namespace Engine {

/// <summary>
/// カメラマネージャクラス（シングルトン）
/// 複数のカメラインスタンスを管理し、アクティブなカメラを切り替える
/// </summary>
class CameraManager {
public:
		// --- メンバ関数 ---
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	static CameraManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGuiでの編集用更新
	/// 現在のカメラパラメータ調整用UIを表示
	/// </summary>
	void ImGuiUpdate();

	/// <summary>
	/// 指定した名前のカメラを生成またはアクティブにする
	/// </summary>
	/// <param name="cameraName">カメラ識別名</param>
	void SetCamera(const std::string& cameraName);

	/// <summary>
	/// カメラを検索する（デバッグ用・存在確認用）
	/// </summary>
	/// <param name="cameraName">検索するカメラ名</param>
	void FindCamera(const std::string& cameraName);

private:
	// 外部からは絶対にインスタンス化できない「鍵」となる構造体を定義
	struct PrivateToken {};
public:
	// シングルトン(make_uniqueする用)
	explicit CameraManager(PrivateToken) {}
	~CameraManager() = default;
	// コピー・ムーブの禁止
	CameraManager(const CameraManager&) = delete;
	CameraManager& operator=(const CameraManager&) = delete;

private:
		// ---シングルトン---
	static std::unique_ptr<CameraManager> instance;

		// ---メンバ変数---
	// カメラデータ格納用マップ
	std::unordered_map<std::string, std::unique_ptr<Camera>> cameras;

	std::string nowCameraName_ = "";
	Camera* camera_ = nullptr; // 現在アクティブなカメラへのポインタ

public:
		// --- ゲッター ---
	/// <summary>
	/// 現在アクティブなカメラを取得する
	/// </summary>
	Camera* GetCamera() const { return camera_; }
};

} // namespace Engine
