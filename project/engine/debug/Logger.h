#pragma once
#include <string>

namespace Engine {

/// <summary>
/// ログ出力名前空間
/// デバッグログの出力機能を提供する
/// </summary>
namespace Logger {
	/// <summary>
	/// ログ出力
	/// Visual Studioの出力ウィンドウにメッセージを表示する
	/// </summary>
	/// <param name="message">表示するメッセージ</param>
	void Log(const std::string& message);
}

} // namespace Engine
