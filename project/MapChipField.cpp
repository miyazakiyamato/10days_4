#include "MapChipField.h"
#include <cassert>
#include <fstream>
#include <sstream>
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


void MapChipField::ResetMapChipData() {
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	// MapChipTypeからMapChipCellへ変更
	for (std::vector<MapChipCell>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {
	ResetMapChipData();
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	std::stringstream mapChipCsv;
	mapChipCsv << file.rdbuf();
	file.close();

	previousFilePath_ = filePath;
	
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		
		std::string line;
		getline(mapChipCsv, line);
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			std::string word;
			getline(line_stream, word, ',');

			// アンダーバーで分割して種類とIDを取得する処理
			std::string typeStr = word;
			int id = -1;
			size_t pos = word.find('_');

			if (pos != std::string::npos) {
				typeStr = word.substr(0, pos);
				id = std::stoi(word.substr(pos + 1));
			}

			if (mapChipTable.contains(typeStr)) {
				mapChipData_.data[i][j].type = mapChipTable[typeStr];
				mapChipData_.data[i][j].id = id;
			}
		}
	}
}

void MapChipField::SaveMapChipCsv(const std::string& filePath) {
	std::ofstream file;
	file.open(filePath);
	assert(file.is_open());

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			MapChipCell cell = mapChipData_.data[i][j];
			std::string typeStr = GetStringFromMapChipType(cell.type);

			// IDが存在する場合はアンダーバーで連結して書き込む
			if (cell.id != -1) {
				file << typeStr << "_" << cell.id;
			} else {
				file << typeStr;
			}

			// 最後の列以外はカンマを追加
			if (j < kNumBlockHorizontal - 1) {
				file << ",";
			}
		}
		file << "\n"; // 1行終わったら改行
	}

	file.close();
}

void MapChipField::ImGuiUpdate() {
#ifdef _DEBUG
	ImGui::Begin("Map Editor");

	// --- 1. ペン（ブラシ）の設定 ---
	// 選択中のブロックの種類とIDを保持（staticで状態を維持）
	static int selectedTypeIndex = 1; // デフォルトはブロック
	static int selectedId = -1;

	// UI表示用の名前と、対応するMapChipType
	const char* typeNames[] = {"0: Blank", "1: Block", "2: Player", "3: Goal", "4: Button", "5: MovingBlock", "6:BrokenBlock"};
	MapChipType types[] = {MapChipType::kBlank, MapChipType::kBlock, MapChipType::kPlayer, MapChipType::kGoal, MapChipType::kButton, MapChipType::kMovingBlock,MapChipType::kBrokenBlock};

	ImGui::Combo("Paint Type", &selectedTypeIndex, typeNames, IM_ARRAYSIZE(typeNames));
	ImGui::InputInt("Paint ID (-1 for none)", &selectedId);

	// --- 2. セーブ＆ロードボタン ---
	if (ImGui::Button("Save Map")) {
		// 上書き保存（パスは環境に合わせて変更してください）
		SaveMapChipCsv(previousFilePath_);
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Map")) {
		LoadMapChipCsv(previousFilePath_);
	}

	ImGui::Separator();
	ImGui::Text("Map Grid (Click or Drag to paint)");

	// --- 3. マップグリッドの描画と編集（クリック＆ドラッグ判定） ---
	for (uint32_t y = 0; y < kNumBlockVirtical; ++y) {
		for (uint32_t x = 0; x < kNumBlockHorizontal; ++x) {
			// ImGuiがボタンを区別できるように固有のIDを発行
			ImGui::PushID(y * kNumBlockHorizontal + x);

			// 現在のセルの情報を取得
			MapChipCell& cell = mapChipData_.data[y][x];

			// ボタンに表示する文字（例: "1", "5_1"）を作成
			std::string cellLabel = GetStringFromMapChipType(cell.type);
			if (cell.id != -1) {
				cellLabel += "_" + std::to_string(cell.id);
			}

			// マス目のボタンを描画（幅30、高さ30）
			ImVec2 buttonSize(30, 30);
			ImGui::Button(cellLabel.c_str(), buttonSize);

			// ★重要：クリックまたは長押し（ドラッグ）で塗る処理★
			// マウスカーソルが乗っていて、かつ左クリックが押されているか判定
			if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
				// 選択しているペン（ブラシ）の情報で上書き
				cell.type = types[selectedTypeIndex];
				cell.id = selectedId;
			}

			ImGui::PopID();

			// 最後の列以外は SameLine で横に並べる
			if (x < kNumBlockHorizontal - 1) {
				ImGui::SameLine();
			}
		}
	}

	ImGui::End();
#endif // _DEBUG
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}
	// 変更：.typeを返す
	return mapChipData_.data[yIndex][xIndex].type;
}

// GameScene等でIDを取得するための関数
MapChipCell MapChipField::GetMapChipCellByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return {MapChipType::kBlank, -1};
	}
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return {MapChipType::kBlank, -1};
	}
	return mapChipData_.data[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0); }

MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {
	IndexSet indexSet = {};
	indexSet.xIndex = uint32_t((position.x + kBlockWidth / 2.0f) / kBlockWidth);
	indexSet.yIndex = kNumBlockVirtical - 1 - uint32_t((position.y + kBlockHeight / 2.0f) / kBlockHeight);
	return indexSet;
}

MapChipField::Rect MapChipField::GatRectByIndex(uint32_t xIndex, uint32_t yIndex) {
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;
	return rect;
}

void MapChipField::SetMapChipData(uint32_t xIndex, uint32_t yIndex, MapChipType mapChipTipe) {
	mapChipData_.data[yIndex][xIndex].type = mapChipTipe;
}