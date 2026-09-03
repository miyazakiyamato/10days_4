#pragma once
#include "Vector3.h"
#include <Object3d.h>

namespace Engine {

/// <summary>
/// コライダー基底クラス
/// 衝突判定を持つオブジェクトが継承するクラス
/// </summary>
class Collider {
public:
	virtual ~Collider() = default;

		// --- メンバ関数 ---
	/// <summary>
	/// 初期化
	/// デバッグ用モデルの読み込みなどを行う
	/// </summary>
	void Initialize();

	/// <summary>
	/// ワールドトランスフォームの更新
	/// デバッグ表示位置の更新などを行う
	/// </summary>
	void UpdateWorldTransform();

	/// <summary>
	/// 描画
	/// デバッグ用のコライダー形状を描画する
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突時に呼ばれるコールバック関数
	/// </summary>
	/// <param name="other">衝突した相手のコライダー</param>
	virtual void OnCollision([[maybe_unused]] Collider* other) {}

private:
		// --- メンバ変数 ---
	// 衝突半径
	float radius_ = 1.5f;
	// 識別ID (CollisionTypeIdDef参照)
	uint32_t typeID_ = 0u;
	// コライダーモデル (デバッグ表示用)
	std::unique_ptr<Object3d> colliderModel_ = nullptr;

public:
		// --- ゲッター ---
	float GetRadius() { return radius_; }

	/// <summary>
	/// 中心座標を取得 (純粋仮想関数)
	/// オブジェクトの実座標を返すように実装する
	/// </summary>
	virtual Vector3 GetCenterPosition() const = 0;

	// 識別IDを取得
	uint32_t GetTypeID() const { return typeID_; }
		// --- セッター ---
	void SetRadius(float radius) { radius_ = radius; }
	// 識別IDを設定
	void SetTypeID(uint32_t typeID) { typeID_ = typeID; }
};

} // namespace Engine
