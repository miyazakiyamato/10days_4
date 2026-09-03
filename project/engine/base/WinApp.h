#pragma once
#include <Windows.h>
#include <cstdint>

namespace Engine {

/// <summary>
/// Windowsアプリケーションクラス
/// ウィンドウの生成とメッセージプロシージャの管理を行う
/// </summary>
class WinApp {
public:
		// --- 静的メンバ関数 ---
	/// <summary>
	/// ウィンドウプロシージャ
	/// OSからのメッセージを処理するコールバック関数
	/// </summary>
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		// --- メンバ関数 ---
	/// <summary>
	/// 初期化処理
	/// ウィンドウクラスの登録とウィンドウ生成を行う
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 終了処理
	/// ウィンドウの破棄と登録解除を行う
	/// </summary>
	void Finalize();

	/// <summary>
	/// メッセージ処理
	/// メッセージキューを確認し、終了メッセージが来たらtrueを返す
	/// </summary>
	/// <returns>終了すべきならtrue</returns>
	bool ProcessMessage();

	/// <summary>
	/// ウィンドウの初期表示位置を取得
	/// </summary>
	POINT GetWindowStartPosition() const;

public:
		// --- 定数 ---
	// クライアント領域のサイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

private:
		// --- メンバ変数 ---
	// ウインドウクラスの設定
	WNDCLASS wc{};
	// ウインドウハンドル
	HWND hwnd_ = nullptr;

public:
		// --- ゲッター ---
	HINSTANCE GetHInstance() const { return wc.hInstance; }
	HWND GetHwnd() const { return hwnd_; }
};

} // namespace Engine
