#pragma once
#include <string>

namespace Engine {

class BaseEnemy;

/// <summary>
/// 敵の状態基底クラス
/// </summary>
class BaseEnemyState {
public:
	BaseEnemyState(const std::string& name, BaseEnemy* enemy) : name_(name), enemy_(enemy){};

	virtual ~BaseEnemyState() {}

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// デバッグログ出力
	/// </summary>
	virtual void DebugLog();

protected:
	std::string name_;
	   
	BaseEnemy* enemy_ = nullptr;
};

} // namespace Engine
