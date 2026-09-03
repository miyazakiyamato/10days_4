#pragma once
#include "BaseEventObject.h"
#include <vector>
#include <string>
#include "Vector3.h"

namespace Engine {

class StageManager;

/// <summary>
/// エネミー出現データ
/// </summary>
/// <summary>
/// レールデータ
/// </summary>
struct RailData {
	std::string name;
	std::vector<Vector3> points;
};

/// <summary>
/// エネミー出現データ
/// </summary>
struct EnemySpawnData {
	Vector3 translation; // イベント発生地点からの相対座標
	Quaternion rotation; // 回転
	std::vector<RailData> rails; // レールデータ
};
/// <summary>
/// 敵出現イベント
/// </summary>
class EnemyPopEvent : public BaseEventObject {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 衝突時コールバック
	/// </summary>
	/// <param name="other"></param>
	void OnCollision(Collider* other) override;

	/// <summary>
	/// ステージマネージャーのセット
	/// </summary>
	/// <param name="stageManager"></param>
	void SetStageManager(StageManager* stageManager) { stageManager_ = stageManager; }

	/// <summary>
	/// エネミー出現データの追加
	/// </summary>
	/// <param name="data"></param>
	void AddEnemySpawnData(const EnemySpawnData& data) { enemySpawnData_.push_back(data); }

private:
	StageManager* stageManager_ = nullptr;
	std::vector<EnemySpawnData> enemySpawnData_;
};

} // namespace Engine
