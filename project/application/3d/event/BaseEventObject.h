#pragma once
#include "Collider.h"
#include "Object3d.h"
#include "ModelManager.h"
#include "CollisionTypeIdDef.h"

namespace Engine {

/// <summary>
/// イベントオブジェクト基底クラス
/// </summary>
class BaseEventObject : public Collider {
public:
	virtual ~BaseEventObject() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 衝突時コールバック
	/// </summary>
	/// <param name="other"></param>
	virtual void OnCollision([[maybe_unused]] Collider* other) override;

	/// <summary>
	/// 中心座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenterPosition() const override;

	/// <summary>
	/// 終了フラグ取得
	/// </summary>
	/// <returns></returns>
	bool GetIsFinished() const { return isFinished_; }

	/// <summary>
	/// 座標設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector3& position) { object3d_->SetTranslate(position); }

protected:
	// 3Dオブジェクト
	std::unique_ptr<Object3d> object3d_;
	// 終了フラグ
	bool isFinished_ = false;
public:
	Object3d* GetObject3d() { return object3d_.get(); }
};

} // namespace Engine
