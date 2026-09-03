#pragma once
#include <vector>
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "ParticleSystem.h"
#include "CollisionManager.h"
#include "AccelerationField.h"
#include "LevelDataManager.h"

namespace Engine {

/// <summary>
/// パーティクルシーン
/// </summary>
class DParticleScene : public BaseScene {
public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
private:
	//衝突判定と応答 
	void CheckAllCollisions();
private://メンバ変数
	//衝突マネージャ
	std::unique_ptr<CollisionManager> collisionManager_;

	bool isAccelerationField = false;
	std::unique_ptr<AccelerationField> accelerationField_ = nullptr;

	std::vector<std::unique_ptr<Object3d>> object3ds_;
	std::vector< std::unique_ptr<Sprite>> sprites_;

	std::unique_ptr<ParticleSystem> particleSystem_ = nullptr;
	std::unique_ptr<LevelDataManager> levelDataManager_ = nullptr;
};


} // namespace Engine
