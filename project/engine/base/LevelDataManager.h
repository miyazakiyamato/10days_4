#pragma once
#include <unordered_map>
#include <json.hpp>
#include "Matrix4x4.h"

namespace Engine {

/// <summary>
/// オブジェクトデータ構造体
/// レベルデータに含まれる個々のオブジェクト情報
/// </summary>
struct ObjectData {
	Vector3 translation{}; // 平行移動
	Vector3 rotation{};    // 回転
	Vector3 scaling{ 1.0f,1.0f,1.0f }; // スケーリング
	std::string fileName = ""; // ファイル名
	std::string typeName = ""; // オブジェクト種別名
	std::string name = ""; // オブジェクト名
	std::vector<std::unique_ptr<ObjectData>> children; // 子オブジェクト
};

/// <summary>
/// レベルデータマネージャクラス
/// JSONファイルからレベルデータを読み込み、管理する
/// </summary>
class LevelDataManager {
public:
		// --- namespace省略 ---
	using LevelData = std::vector<std::unique_ptr<ObjectData>>;

		// --- メンバ関数 ---
	LevelDataManager() = default;
	~LevelDataManager() = default;

	/// <summary>
	/// JSONファイルの読み込み
	/// </summary>
	/// <param name="filePath">読み込むファイルのパス (拡張子なし)</param>
	void LoadJsonFile(const std::string& filePath);

	/// <summary>
	/// オブジェクトデータの読み込み（再帰的）
	/// </summary>
	/// <param name="object">JSONオブジェクト</param>
	/// <returns>解析されたオブジェクトデータ</returns>
	std::unique_ptr<ObjectData> LoadObjectData(const nlohmann::json& object);

	/// <summary>
	/// データのクリア
	/// </summary>
	void Clear() { levelData_.clear(); }

	/// <summary>
	/// レベルデータの取得
	/// </summary>
	/// <param name="filePath">取得したいレベルデータのキー</param>
	/// <returns>レベルデータへのポインタ</returns>
	LevelData* GetObjectData(const std::string& filePath) { return levelData_[filePath].get(); }

private:
		// --- メンバ変数 ---
	const std::string kDirectoryFilePath = "./resources/level_data/";
	const std::string kExtension = ".json";
	std::unordered_map<std::string, std::unique_ptr<LevelData>> levelData_;
};

} // namespace Engine
