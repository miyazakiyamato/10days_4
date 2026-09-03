#pragma once
#include <vector>
#include <memory>
#include "Sprite.h"
#include "BaseUI.h"

namespace Engine {

class Player;
/// <summary>
/// HP UIクラス
/// プレイヤーのHPバーを表示する
/// </summary>
class HpUI : public BaseUI {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Initialize(Player* player);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	/// <summary>
	/// ImGui更新
	/// </summary>
	void ImGuiUpdate() override;
private:
	//プレイヤーデータ
	Player* player_ = nullptr;
	float maxHp_ = 5;
	float preHp_ = 5;
	float damageHp_ = 0;
	float easingDamageHp_ = 0.0f;
	float damegeHpTimer_ = 0.0f;
	float damegeHpLimitTime_ = 2.0f;
	Vector2 hpSize_ = { 300.0f,50.0f };
	//スプライト
	std::vector< std::unique_ptr<Sprite>> sprites_;
};


} // namespace Engine
