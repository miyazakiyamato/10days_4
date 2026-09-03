#pragma once
#include <stdio.h>
#include <functional>

namespace Engine {

/// <summary>
/// 指定時間後に関数を呼び出すクラス
/// </summary>
class TimedCall {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="f">コールバック関数</param>
	/// <param name="time">遅延時間</param>
	TimedCall(std::function<void(void)> f, float time) : f_(f), time_(time) {}
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 終了フラグ取得
	/// </summary>
	/// <returns>終了したか</returns>
	bool IsFinished() { return IsFinish_; }

private:
	//コールバック
	std::function<void(void)> f_;
	//
	float time_;
	bool IsFinish_ = false;
};

} // namespace Engine
