#pragma once

namespace Engine {
class SceneManager;
/// <summary>
/// 遷移シーン基底クラス
/// </summary>
class BaseTransition{
public: 
		// --- 構造体定義 ---
	/// <summary>
	/// 遷移エフェクトタイプ
	/// </summary>
	enum class Type {
		kIn,
		kOut,
	};
		 // --- メンバ関数 ---
    /// <summary>
    /// デストラクタ
    /// </summary>
    virtual ~BaseTransition() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;
protected:
		// --- 継承 ---
	SceneManager* sceneManager_ = nullptr;
	bool isFinished_ = false;

	// 遷移エフェクトタイプ
	Type type_ = Type::kOut;
public:
		// --- ゲッター セッター ---
	bool IsFinished() const { return isFinished_; }
	Type GetType() const { return type_; }

	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }
	/// <summary>
	/// 遷移エフェクトタイプの設定
	/// </summary>
	/// <param name="type">タイプ</param>
	void SetType(Type type) { type_ = type; }
};

} // namespace Engine
