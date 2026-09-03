#include "EnemyPopEvent.h"
#include "SmallDrone.h"
#include "StageManager.h"
#include "SplineRail.h"

namespace Engine {

void EnemyPopEvent::Initialize() {
	BaseEventObject::Initialize();
	Collider::SetRadius(10.0f);
}

void EnemyPopEvent::OnCollision(Collider* other) {
	// プレイヤーと衝突したら
	if (other->GetTypeID() == static_cast<uint32_t>(CollisionTypeIdDef::kPlayer)) {
		if (stageManager_) {
			// 登録されたデータに基づいてエネミーを生成
			for (const auto& data : enemySpawnData_) {
				std::unique_ptr<BaseEnemy> enemy = std::make_unique<SmallDrone>();

				Vector3 spawnPosition = object3d_->GetTranslate() + data.translation;
				
				// レール追加
				for (const auto& railData : data.rails) {
					std::unique_ptr<SplineRail> rail = std::make_unique<SplineRail>();
					
					// 相対座標をワールド座標に変換
					std::vector<Vector3> worldPoints;
					worldPoints.reserve(railData.points.size());
					for (const auto& point : railData.points) {
						worldPoints.push_back(spawnPosition + point);
					}
					rail->Initialize(worldPoints);

					// Blenderのサフィックス除去
					std::string railName = railData.name;
					size_t dotPos = railName.rfind('.');
					if (dotPos != std::string::npos) {
						railName = railName.substr(0, dotPos);
					}
					
					enemy->AddRail(railName, std::move(rail));
				}

				stageManager_->AddEnemy(std::move(enemy), spawnPosition);
			}
			
			isFinished_ = true;
		}
	}
}

} // namespace Engine
