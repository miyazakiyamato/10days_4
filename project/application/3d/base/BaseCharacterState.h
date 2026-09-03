#pragma once
#include <string>

namespace Engine {

class BaseCharacter;
/// <summary>
/// キャラクター状態基底クラス
/// </summary>
class BaseCharacterState {
public:
	// コンストラクタ / デストラクタ
	BaseCharacterState(const std::string& name, BaseCharacter* character) : name_(name), character_(character){};
	virtual ~BaseCharacterState() {}
		// --- メンバ関数 ---
	
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
protected:
	// 状態名
	std::string name_;
	// キャラクターへのポインタ
	BaseCharacter* character_ = nullptr;
};

} // namespace Engine
