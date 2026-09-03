#pragma once
#include <vector>

namespace Engine {

/// <summary>
/// 接触記録クラス
/// オブジェクトが接触した履歴を管理し、重複ヒット防止などに利用する
/// </summary>
class ContactRecord {
public:
		// --- メンバ関数 ---
	/// <summary>
	/// 履歴の追加
	/// </summary>
	/// <param name="number">記録する識別番号 (IDなど)</param>
	void AddRecord(uint32_t number);

	/// <summary>
	/// 履歴のチェック
	/// 指定した番号が既に記録されているかを確認する
	/// </summary>
	/// <param name="number">確認する識別番号</param>
	/// <returns>既に記録されていれば true</returns>
	bool CheckRecord(uint32_t number);

	/// <summary>
	/// 履歴の抹消
	/// 記録された全てのデータをクリアする
	/// </summary>
	void Clear();

private:
		// --- メンバ変数 ---
	// 履歴データ
	std::vector<uint32_t> history_;
};

} // namespace Engine
