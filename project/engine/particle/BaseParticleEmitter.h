#pragma once
#include "Matrix4x4.h"
#include <map>
#include "ParticleManager.h"
#include "DirectXCommon.h"

namespace Engine {

class ParticleManager;
class SrvUavManager;
class GlobalVariables;

/// <summary>
/// パーティクルエミッター基底クラス
/// 各種形状のエミッターの共通インターフェースと基本機能を定義する
/// </summary>
class BaseParticleEmitter {
public:
		// --- コンストラクタ/デストラクタ ---
	BaseParticleEmitter() = default;
	virtual ~BaseParticleEmitter() = default;

		// --- 仮想関数 ---
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="emitterName">エミッター識別名</param>
	/// <param name="kMaxParticles">最大パーティクル数</param>
	virtual void Initialize(const std::string& emitterName, uint32_t kMaxParticles);

	/// <summary>
	/// 更新処理 (純粋仮想関数)
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// パーティクルの発生 (純粋仮想関数)
	/// </summary>
	virtual void Emit() = 0;

	/// <summary>
	/// 調整項目の初期化
	/// GlobalVariablesへのパラメータ登録を行う
	/// </summary>
	virtual void InitializeGlobalVariables();

	/// <summary>
	/// 調整項目の適用
	/// GlobalVariablesの値をエミッターに反映する
	/// </summary>
	virtual void ApplyGlobalVariables();

protected:
		// --- メンバ変数 ---
	DirectXCommon* dxCommon_ = nullptr;        // DirectXの共通インスタンス
	ParticleManager* particleManager_ = nullptr; // パーティクルマネージャーのインスタンス
	SrvUavManager* srvUavManager_ = nullptr;   // SRV/UAVマネージャーのインスタンス
	GlobalVariables* globalVariables_ = nullptr; // グローバル変数のインスタンス

	std::string name_ = ""; // エミッターの名前

	Transform transform_;   // エミッターの変換情報 (位置・回転・スケール)

public:
		// --- ゲッター ---
	const std::string& GetName() const { return name_; }
	const Vector3& GetPosition() { return transform_.translate; }
	const Vector3& GetRotation() { return transform_.rotate; }
	const Vector3& GetSize() { return transform_.scale; }

		// --- セッター ---
	void SetPosition(const Vector3& position) { transform_.translate = position; }
	void SetRotation(const Vector3& rotation) { transform_.rotate = rotation; }
	void SetSize(const Vector3& size) { transform_.scale = size; }

	/// <summary>
	/// 使用するテクスチャを設定する
	/// </summary>
	void SetTexture(const std::string& textureName);

	/// <summary>
	/// リング形状のメッシュ設定 (パーティクル表示用)
	/// </summary>
	void SetRing(const uint32_t& kDivide, float kOuterRadius, float kInnerRadius);

	/// <summary>
	/// 円柱形状のメッシュ設定 (パーティクル表示用)
	/// </summary>
	void SetCylinder(const uint32_t& kDivide, float kTopRadius, float kBottomRadius, float kHeight);
};

} // namespace Engine
