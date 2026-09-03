#pragma once

namespace Engine {

class WinApp;
class DirectXCommon;
class SrvUavManager;

/// <summary>
/// ImGuiマネージャクラス
/// ImGuiのコンテキスト管理、フレーム開始・終了、描画処理をカプセル化する
/// </summary>
class ImGuiManager {
public:
	/// <summary>
		// --- 列挙型 ---
	/// <summary>
	/// 表示モード
	/// </summary>
	enum class ViewMode {
		kEditor,       //!< エディタモード (全て表示)
		kDebugOverlay, //!< デバッグオーバーレイ (シーンビューなし、デバッグUIのみ)
		kGameOnly      //!< ゲームのみ (メニューバーのみ)
	};
		// --- メンバ関数 ---
	/// <summary>
	/// 初期化処理
	/// ImGuiのコンテキスト作成、スタイル設定、プラットフォーム/レンダラーの初期化を行う
	/// </summary>
	/// <param name="winApp">Windowsアプリケーション管理クラス</param>
	/// <param name="dxCommon">DirectX共通クラス</param>
	/// <param name="srvUavManager">SRV/UAV管理クラス (フォントテクスチャ用)</param>
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon, SrvUavManager* srvUavManager);

	/// <summary>
	/// 終了処理
	/// ImGuiのシャットダウンを行う
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui受付開始
	/// 新しいフレームを開始し、ImGuiコマンドを受け付ける状態にする
	/// </summary>
	void Begin();

	void DrawStatusOverlayContent();

	/// <summary>
	/// ImGui受付終了
	/// フレームの描画データを生成する
	/// </summary>
	void End();

	/// <summary>
	/// 画面への描画
	/// コマンドリストにImGuiの描画コマンドを積む
	/// </summary>
	void Draw();

private:
	/// <summary>
	/// メニューバーの描画
	/// </summary>
	void DrawMenuBar();
		// --- メンバ変数 ---
	// WindowsAPI
	WinApp* winApp_ = nullptr;
	// DirectXCommon
	DirectXCommon* dxCommon_ = nullptr;
	// SRV
	SrvUavManager* srvUavManager_ = nullptr;
	// 現在の表示モード
	ViewMode currentViewMode_ = ViewMode::kEditor;

public:
	/// <summary>
	/// 表示モードの取得
	/// </summary>
	ViewMode GetViewMode() const { return currentViewMode_; }
};

} // namespace Engine
