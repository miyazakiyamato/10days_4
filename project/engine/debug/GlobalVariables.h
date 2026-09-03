#pragma once
#include <variant>
#include <map>
#include <string>
#include <json.hpp>
#include <memory>
#include "Matrix4x4.h"

namespace Engine {

/// <summary>
/// グローバル変数管理クラス
/// ImGuiを使ってアプリケーション実行中にパラメータを調整し、JSONファイルとして保存・読み込みを行うシングルトン
/// </summary>
class GlobalVariables {
public:
		// --- メンバ関数 ---
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	static GlobalVariables* GetInstance();

	/// <summary>
	/// グループの作成
	/// 新しいパラメータグループを作成する
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// 毎フレーム処理
	/// ImGuiによるパラメータ調整UIの描画を行う
	/// </summary>
	void Update();

	/// <summary>
	/// コンボボックスの表示 (std::string版)
	/// </summary>
	void ShowCombo(const std::string& label, const std::vector<std::string>& items, int& selectedIndex, std::function<void(const std::string&)> onSelect);

	/// <summary>
	/// コンボボックスの表示 (intインデックス版)
	/// </summary>
	void ShowCombo(const std::string& label, const std::vector<std::string>& items, int& selectedIndex, std::function<void(int)> onSelect);

	/// <summary>
	/// ファイルに書き出し
	/// 指定したグループのパラメータをJSONファイルに保存する
	/// </summary>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// ファイルから読み込む 
	/// 指定したグループのJSONファイルを読み込む
	/// </summary>
	void LoadFile(const std::string& groupName);

	/// <summary>
	/// 値のセット
	/// </summary>
	/// <typeparam name="T">値の型 (int, float, Vector3など)</typeparam>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">項目名</param>
	/// <param name="value">設定する値</param>
	template <typename T>
	void SetValue(const std::string& groupName, const std::string& key, T value);

	/// <summary>
	/// 項目の追加
	/// まだ項目が存在しない場合のみ追加する
	/// </summary>
	/// <typeparam name="T">値の型</typeparam>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">項目名</param>
	/// <param name="value">初期値</param>
	template<typename T>
	void AddItem(const std::string& groupName, const std::string& key, T value);

	/// <summary>
	/// 値を取得
	/// </summary>
	/// <typeparam name="T">取得したい型</typeparam>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">項目名</param>
	/// <returns>現在の値</returns>
	template<typename T>
	T GetValue(const std::string& groupName, const std::string& key) const;

	/// <summary>
	/// 表示順序を設定
	/// ImGuiでの項目の表示順を指定する
	/// </summary>
	void SetDisplayOrder(const std::string& groupName, const std::vector<std::string>& order);

	/// <summary>
	/// 表示順序を取得
	/// </summary>
	const std::vector<std::string>& GetDisplayOrder(const std::string& groupName) const;

	/// <summary>
	/// CollapsingHeaderグループ設定 (項目ごと)
	/// 特定の項目を指定したヘッダー内に表示するように設定する
	/// </summary>
	void SetHeaderGroup(const std::string& groupName, const std::string& itemKey, const std::string& headerName);

	/// <summary>
	/// CollapsingHeaderグループ取得
	/// </summary>
	std::string GetHeaderGroup(const std::string& groupName, const std::string& itemKey) const;

	/// <summary>
	/// CollapsingHeaderグループ設定 (開始)
	/// これ以降追加される項目を指定したヘッダー内に入れる
	/// </summary>
	void SetHeaderGroup(const std::string& groupName, const std::string& headerName);

	/// <summary>
	/// CollapsingHeaderグループ解除 (終了)
	/// </summary>
	void EndHeaderGroup(const std::string& groupName);

private:
	// 外部からは絶対にインスタンス化できない「鍵」となる構造体を定義
	struct PrivateToken {};
public:
	// シングルトン(make_uniqueする用)
	explicit GlobalVariables(PrivateToken) {}
	~GlobalVariables() = default;
	// コピー・ムーブの禁止
	GlobalVariables(const GlobalVariables&) = delete;
	GlobalVariables& operator=(const GlobalVariables&) = delete;

private:
		// --- シングルトン ---
	static std::unique_ptr<GlobalVariables> instance;

		// --- メンバ変数 ---
	using json = nlohmann::json;
	// 保持可能な型のバリアント
	using Item = std::variant<bool, int32_t, float, Vector2, Vector3, Vector4, std::string, Transform>;
	// 項目名と値のマップ
	using Group = std::map<std::string, Item>;

	// 全データ (グループ名 -> グループデータ)
	std::map<std::string, Group> data_;

	// 表示順序: groupName -> vector of item keys
	std::map<std::string, std::vector<std::string>> displayOrders_;

	// CollapsingHeaderグループ: groupName -> (itemKey -> headerName)
	std::map<std::string, std::map<std::string, std::string>> headerGroups_;
	// 現在のヘッダーグループ: groupName -> headerName (空文字列で無効)
	std::map<std::string, std::string> currentHeaderGroup_;

	// グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "resources/GlobalVariables/";
};

} // namespace Engine
