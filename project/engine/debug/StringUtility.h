#pragma once
#include <string>

namespace Engine {

/// <summary>
/// 文字列ユーティリティ名前空間
/// 文字コード変換などのヘルパー関数を提供する
/// </summary>
namespace StringUtility {
	/// <summary>
	/// string(マルチバイト文字列)をwstring(ワイド文字列)に変換する
	/// </summary>
	/// <param name="str">変換元のstd::string</param>
	/// <returns>変換後のstd::wstring</returns>
	std::wstring ConvertString(const std::string& str);

	/// <summary>
	/// wstring(ワイド文字列)をstring(マルチバイト文字列)に変換する
	/// </summary>
	/// <param name="str">変換元のstd::wstring</param>
	/// <returns>変換後のstd::string</returns>
	std::string ConvertString(const std::wstring& str);
}

} // namespace Engine
