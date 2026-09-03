#pragma once
#include <map>
#include <memory>
#include "Model.h"
#include "Animation.h"

namespace Engine {

class SrvUavManager;

/// <summary>
/// モデル・アニメーションリソース管理クラス（シングルトン）
/// 同じファイルパスのモデルやアニメーションを重複して読み込まないように管理する
/// </summary>
class ModelManager {
private:
	// 外部からは絶対にインスタンス化できない「鍵」となる構造体を定義
	struct PrivateToken {};
public:
	// シングルトン(make_uniqueする用)
	explicit ModelManager(PrivateToken) {}
	~ModelManager() = default;
	// コピー・ムーブの禁止
	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;
		// --- メンバ関数 ---
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	static ModelManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, SrvUavManager* srvUavManager);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// モデルファイルを読み込む
	/// 既に読み込み済みの場合はスキップする
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadModel(const std::string& filePath);

	/// <summary>
	/// 読み込み済みのモデルを検索して取得する
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>Modelへのポインタ、なければnullptr</returns>
	Model* FindModel(const std::string& filePath);

	/// <summary>
	/// アニメーションファイルを読み込む
	/// </summary>
	void LoadAnimation(const std::string& filePath);

	/// <summary>
	/// 読み込み済みのアニメーションを検索して取得する
	/// </summary>
	Animation* FindAnimation(const std::string& filePath);

private:
		// --- シングルトン ---
	static std::unique_ptr<ModelManager> instance;
		// --- メンバ変数 ---
	// ポインタ
	DirectXCommon* dxCommon_ = nullptr;
	SrvUavManager* srvUavManager_ = nullptr;

	// モデルデータ (キー: ファイルパス)
	std::map<std::string, std::unique_ptr<Model>> models;

	// アニメーションデータ (キー: ファイルパス)
	std::map<std::string, std::unique_ptr<Animation>> animations;

public:
		// --- ゲッター ---
	DirectXCommon* GetDirectXCommon() { return dxCommon_; }
	SrvUavManager* GetSrvManager() { return srvUavManager_; }
};

} // namespace Engine
