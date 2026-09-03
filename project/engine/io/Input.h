#pragma once
#define DIRECTINPUT_VERSION 0x800
#include <Windows.h>
#include <dinput.h>
#include <XInput.h>
#include <wrl.h>
#include <array>
#include <memory>
#include "Vector2.h"

namespace Engine {

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "xinput.lib")

class WinApp;

/// <summary>
/// 入力管理クラス
/// キーボード、マウス、コントローラーの入力を管理するシングルトン
/// </summary>
class Input {
private:
	    // --- namespace省略 ---
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	    // --- 構造体 ---
    /// <summary>
    /// コントローラー状態構造体
    /// </summary>
    struct ControllerState {
        XINPUT_STATE nowState = {}; // 現在のフレームの状態
        XINPUT_STATE preState = {}; // 1フレーム前の状態
    };

        // --- メンバ関数 ---
    /// <summary>
    /// シングルトンインスタンスの取得
    /// </summary>
    static Input* GetInstance();

    /// <summary>
    /// 初期化処理
    /// DirectInputの初期化、デバイスの生成を行う
    /// </summary>
    /// <param name="winApp">Windowsアプリケーションクラス</param>
    void Initialize(WinApp* winApp);

    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize();

    /// <summary>
    /// 更新処理
    /// キーボード、マウス、コントローラーの状態を更新する
    /// </summary>
    void Update();

    // --- キーボード操作 ---

    /// <summary>
    /// キーが押されているかチェック (Press)
    /// </summary>
    /// <param name="keyNumber">DIK_で始まるキーコード</param>
    /// <returns>押されていればtrue</returns>
    bool PushKey(BYTE keyNumber);

    /// <summary>
    /// キーが押された瞬間かチェック (Trigger)
    /// </summary>
    /// <param name="keyNumber">DIK_で始まるキーコード</param>
    /// <returns>押された瞬間ならtrue</returns>
    bool TriggerKey(BYTE keyNumber);

    // --- コントローラー操作 ---

    /// <summary>
    /// コントローラーのボタンが押されているかチェック (Press)
    /// </summary>
    /// <param name="button">XINPUT_GAMEPAD_で始まるボタン定義</param>
    bool PushControllerButton(WORD button);

    /// <summary>
    /// コントローラーのボタンが押された瞬間かチェック (Trigger)
    /// </summary>
    /// <param name="button">XINPUT_GAMEPAD_で始まるボタン定義</param>
    bool TriggerControllerButton(WORD button);

    // スティック入力取得 (デッドゾーン処理込み)
    float GetControllerStickLX();
    float GetControllerStickLY();
    float GetControllerStickRX();
    float GetControllerStickRY();

    // --- マウス操作 ---

    /// <summary>
    /// マウスボタンが押されているかチェック
    /// </summary>
    /// <param name="button">0:左, 1:右, 2:中, 3~:その他</param>
    bool PushMouseButton(int button);

    /// <summary>
    /// マウスボタンが押された瞬間かチェック
    /// </summary>
    /// <param name="button">0:左, 1:右, 2:中, 3~:その他</param>
    bool TriggerMouseButton(int button);

    /// <summary>
    /// マウス位置を固定する
    /// </summary>
    /// <param name="v">固定するスクリーン座標</param>
    void LockMousePosition(Vector2 v);

    /// <summary>
    /// マウスの移動範囲を制限する
    /// </summary>
    /// <param name="leftTop">左上座標</param>
    /// <param name="rightBottom">右下座標</param>
    void LockMouseRangePosition(Vector2 leftTop, Vector2 rightBottom);

    /// <summary>
    /// ImGuiでのパラメータ調整用
    /// </summary>
    void ImGuiUpdate();

private:
	// 外部からは絶対にインスタンス化できない「鍵」となる構造体を定義
	struct PrivateToken {};
public:
	// シングルトン(make_uniqueする用)
	explicit Input(PrivateToken) {}
	~Input() = default;
	// コピー・ムーブの禁止
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

private:
	    // --- シングルトン ---
    static std::unique_ptr<Input> instance;

        // --- メンバ変数 ---
    WinApp* winApp_ = nullptr;

    // DirectInput関連
    ComPtr<IDirectInput8> directInput;
    ComPtr<IDirectInputDevice8> keyboard;
    ComPtr<IDirectInputDevice8> mouse;

    // キーボード状態
    BYTE key[256] = {};
    BYTE keyPre[256] = {};

    // マウス状態
    DIMOUSESTATE2 mouseState = {};
    DIMOUSESTATE2 mouseStatePre = {};
    float mouseSensitivity = 1.00f; // マウス感度（デフォルト1.0）
    POINT mousePosition = { 0, 0 }; // マウスの現在の位置（絶対座標）

    Vector2 preMousePosition = {};
    Vector2 mouseMoving = {}; // マウスの移動量

    // マウス制御フラグ
    bool isMouseLocked_ = false;  // マウス固定フラグ
    Vector2 lockPosition{};       // 固定する位置
    bool isMouseLockedLange_ = false; // マウスの範囲固定フラグ
    Vector2 leftTop_{};
    Vector2 rightBottom_ = { 1280.0f, 720.0f + 30.0f };

    // コントローラー状態
    ControllerState controller_ = {};
    Vector2 deadZone_ = { 3000.0f, 3000.0f }; // スティックのデッドゾーン

public:
        // --- ゲッター ---
    // マウス移動量の取得
    float GetMouseX();
    float GetMouseY();

    /// <summary>
    /// マウス位置（ウィンドウ基準）を取得
    /// </summary>
    Vector2 GetMousePosition();

    bool GetIsLockMouse() const { return isMouseLocked_; }

    bool GetIsLockedLange() const { return isMouseLockedLange_; }

	    // --- セッター ---
    /// <summary>
    /// マウス感度の設定
    /// </summary>
    void SetMouseSensitivity(float sensitivity);

    /// <summary>
    /// マウス位置を強制的に設定する
    /// </summary>
    void SetMousePosition(Vector2 v);

    void SetIsLockMouse(bool isMouseLock) { isMouseLocked_ = isMouseLock; }
    void SetIsLockedLange(bool isMouseLock) { isMouseLockedLange_ = isMouseLock; }
};

} // namespace Engine
