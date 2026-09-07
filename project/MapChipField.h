#pragma once
#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <Vector3.h>
enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
	kPlayer,//プレイヤー
	kGoal, //ゴール
	kButton,      // ボタン
	kMovingBlock, // 移動ブロック
	kBrokenBlock, // 壊れるブロック
};
// 種類とIDをセットで保持する構造体
struct MapChipCell {
	MapChipType type;
	int id = -1; // IDがない場合は-1
};

// MapChipTypeからMapChipCellに変更
struct MapChipData {
	std::vector<std::vector<MapChipCell>> data;
};

namespace {

std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
    {"2", MapChipType::kPlayer},
    {"3", MapChipType::kGoal},
    {"4", MapChipType::kButton     },
    {"5", MapChipType::kMovingBlock},
    {"6", MapChipType::kBrokenBlock},
};
// 列挙型から文字列を取得する逆引き関数
std::string GetStringFromMapChipType(MapChipType type) {
	for (const auto& pair : mapChipTable) {
		if (pair.second == type) {
			return pair.first;
		}
	}
	return "0";
}

}
/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField {
public:
	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};
	// 矩形
	struct Rect {
		float left = 0.0f;   // 左端
		float right = 1.0f;  // 右端
		float bottom = 0.0f; // 下端
		float top = 1.0f;    // 上端
	};
	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);
	void SaveMapChipCsv(const std::string& filePath);
	// ImGuiでのマップエディタ更新関数
	void ImGuiUpdate();

private:
	//1ブロックのサイズ
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;
	//ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 30;

	MapChipData mapChipData_;

	IndexSet playerIndex_{};
	IndexSet goalIndex_{};

	std::string previousFilePath_ = ""; // 前回読み込んだファイルパスを保持する変数
public:
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	MapChipCell GetMapChipCellByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	uint32_t GetNumBlockVirtical() { return kNumBlockVirtical; }
	uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; }
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);
	Rect GatRectByIndex(uint32_t xIndex, uint32_t yIndex);
	void SetMapChipData(uint32_t xIndex, uint32_t yIndex, MapChipType mapChipTipe);
	MapChipType GetMapChipData(uint32_t xIndex, uint32_t yIndex) { return mapChipData_.data[yIndex][xIndex].type; }
	IndexSet GetPlayerIndex() { return playerIndex_; }
	IndexSet GetGoalIndex() { return goalIndex_; }
};
