#pragma once
#include <string>
#include <unordered_map>
#include "BaseParticleEmitter.h"

namespace Engine {

class GlobalVariables;

/// <summary>
/// パーティクルシステムクラス
/// 複数のエミッターを統合管理し、ゲームループ内でのパーティクル処理を統括する
/// </summary>
class ParticleSystem {
public:
		// --- メンバ関数 ---
	ParticleSystem() {};
	~ParticleSystem();

	/// <summary>
	/// 初期化
	/// GlobalVariablesの読み込み等を行う
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// 管理下の全エミッターの更新を行う
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// ParticleManagerの描画処理を呼び出す
	/// </summary>
	void Draw();

	/// <summary>
	/// エミッターの登録
	/// </summary>
	/// <param name="emitter">所有権を移譲するエミッターのポインタ</param>
	void SetParticleEmitter(std::unique_ptr<BaseParticleEmitter> emitter);

	/// <summary>
	/// エミッターの取得
	/// </summary>
	BaseParticleEmitter* GetParticleEmitter(const std::string& emitterName);

	/// <summary>
	/// 特定のエミッターからパーティクルを発生させる
	/// </summary>
	void Emit(const std::string& emitterName);

	/// <summary>
	/// ImGui更新処理
	/// エミッターのパラメータ調整UIを表示する
	/// </summary>
	void ImGuiUpdate();

	/// <summary>
	/// エミッターの検索
	/// </summary>
	BaseParticleEmitter* FindEmitter(const std::string& emitterName) { return emitters_[emitterName].get(); }

private:
		// --- ローカル関数 ---
	/// <summary>
	/// タイプ名からエミッターインスタンスを作成するファクトリ関数
	/// </summary>
	std::unique_ptr<BaseParticleEmitter> CreateEmitterByType(const std::string& typeName);

	// 調整項目の初期化
	void InitializeGlobalVariables();
	// 調整項目の適用
	void ApplyGlobalVariables();

		// --- メンバ変数 ---
	GlobalVariables* globalVariables_ = nullptr;

	// パーティクルエミッターの管理マップ (名前 -> インスタンス)
	std::unordered_map<std::string, std::unique_ptr<BaseParticleEmitter>> emitters_;

	// ImGui用: エミッター名入力バッファ
	std::string emitterNameText_ = "";
	char textBuffer_[128] = "";

	// パーティクル最大数設定
	int maxParticles_ = 1000;

	// 作成するエミッターのタイプ名
	std::string nowEmitterTypeName_ = "Sphere";
};

} // namespace Engine
