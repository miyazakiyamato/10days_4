#pragma once
#include "Collider.h"
#include "Object3d.h"
#include "BaseCharacterState.h"
#include "Quaternion.h"

namespace Engine {

/// <summary>
/// キャラクター基底クラス
/// </summary>
class BaseCharacter : public Collider{
public:
	struct Shake {
		Quaternion shakeQuaternion{};        // ターゲット回転（のけぞり先）
		Quaternion preShakeQuaternion{};     // 1フレーム前
		Quaternion startQuaternion{};        // 開始時の回転
		float kTime;
		float time;
		bool isShake = false;

		Vector3 move{};
		Vector3 preMove{};
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseCharacter();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 状態変更
	/// </summary>
	/// <param name="newState">新しい状態</param>
	void ChangeState(std::unique_ptr<BaseCharacterState> newState);

protected:
	void ShakeStart(Quaternion shakeQuaternion, float kTime);
	void DamageKnockbackStart(const Vector3& hitDirection, float power, float duration);
	void Shaking();

protected:
	// ワールドデータ
	std::unique_ptr<Object3d> object3d_ = nullptr;
	// キャラクター状態
	std::unique_ptr<BaseCharacterState> state_ = nullptr;
	// シェイク
	Shake shake_{};
public:
	/// <summary>
	/// 中心座標取得
	/// </summary>
	/// <returns>中心座標</returns>
	virtual Vector3 GetCenterPosition() const override;
	
	/// <summary>
	/// 3Dオブジェクト取得
	/// </summary>
	/// <returns>3Dオブジェクト</returns>
	Object3d* GetObject3d() { return object3d_.get(); }
};


} // namespace Engine
