#pragma once
#include "d3dx12.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "BlendMode.h"

namespace Engine {

/// <summary>
/// 2Dスプライトクラス
/// テクスチャ付きの矩形ポリゴンを描画・管理する
/// </summary>
class Sprite {
private:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
		// --- 構造体定義 ---
	/// <summary>
	/// 頂点データ構造体
	/// </summary>
	struct VertexData {
		Vector4 position; // 座標
		Vector2 texcoord; // テクスチャ座標(UV)
		Vector3 normal;   // 法線ベクトル
	};

	/// <summary>
	/// マテリアル定数バッファ用構造体
	/// </summary>
	struct Material {
		Vector4 color;          // 色 (RGBA)
		int enableLighting;     // ライティング有効フラグ
		float padding[3];       // アライメント調整
		Matrix4x4 uvTransform;  // UV変換行列
	};

	/// <summary>
	/// 座標変換行列定数バッファ用構造体
	/// </summary>
	struct TransformationMatrix {
		Matrix4x4 WVP;   // World * View * Projection
		Matrix4x4 World; // World行列
	};
		
		// --- メンバ関数 ---
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sprite();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite() = default;
	Sprite(const Sprite&) = delete;
	Sprite& operator=(const Sprite&) = delete;
	Sprite(Sprite&&) = default;
	Sprite& operator=(Sprite&&) = default;

	/// <summary>
	/// 初期化処理
	/// 頂点バッファ、インデックスバッファ、マテリアル等のリソースを作成する
	/// </summary>
	/// <param name="textureFilePath">使用するテクスチャのファイルパス</param>
	void Initialize(std::string textureFilePath);

	/// <summary>
	/// 更新処理
	/// 行列の計算やリソースへのデータ転送を行う
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// コマンドリストに描画コマンドを積む
	/// </summary>
	void Draw();

	/// <summary>
	/// ImGuiでのパラメータ編集用更新処理
	/// </summary>
	/// <param name="name">ImGuiのウィンドウ識別用名称</param>
	void ImGuiUpdate(const std::string& name);

private:
		// --- ローカル関数 ---
	/// <summary>
	/// テクスチャサイズに合わせてスプライトのサイズを自動調整する
	/// </summary>
	void AdjustTextureSize();

		// --- メンバ変数 ---
	// バッファリソース
	ComPtr<ID3D12Resource> vertexResource;   // 頂点リソース
	ComPtr<ID3D12Resource> indexResource;    // インデックスリソース
	ComPtr<ID3D12Resource> materialResource; // マテリアルリソース
	ComPtr<ID3D12Resource> wvpResource;      // 変換行列リソース

	// バッファリソース内のデータを指すポインタ (Mapped Data)
	VertexData* vertexData = nullptr;
	uint32_t* indexData = nullptr;
	Material* materialData = nullptr;
	TransformationMatrix* wvpData = nullptr;

	// バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

	// パラメータ
	std::string textureFilePath_ = ""; // テクスチャパス
	Vector2 anchorPoint_ = { 0.0f, 0.0f }; // アンカーポイント (0.0~1.0, 左上が0,0)
	bool isFlipX_ = false; // 左右反転フラグ
	bool isFlipY_ = false; // 上下反転フラグ

	Vector2 position_ = { 0.0f, 0.0f }; // 描画位置
	float rotation_ = 0.0f;             // 回転角 (ラジアン)
	Vector2 size_ = { 640.0f, 360.0f }; // スプライトサイズ

	// テクスチャ切り出し範囲
	Vector2 textureLeftTop_ = { 0.0f, 0.0f }; // 切り出し左上座標
	Vector2 textureSize_ = { 100.0f, 100.0f }; // 切り出しサイズ

	// UVトランスフォーム
	Transform uvTransform_{ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

	BlendMode blendMode_ = BlendMode::kNormal; // ブレンドモード
	std::string pipelineStateName_ = "";       // 使用するパイプラインステート名

public:
		// --- ゲッター ---
	const BlendMode& GetBlendMode() { return blendMode_; }
	const Vector2& GetPosition() { return position_; }
	float GetRotation() { return rotation_; }
	const Vector2& GetSize() { return size_; }
	const Vector4& GetColor() { return materialData->color; }
	const Vector2& GetAnchorPoint() const { return anchorPoint_; }
	bool GetIsFlipX() { return isFlipX_; }
	bool GetIsFlipY() { return isFlipY_; }

	// UV関連ゲッター
	const Transform& GetUVTransform() { return uvTransform_; }
	Vector2 GetUVTranslate() { return { uvTransform_.translate.x, uvTransform_.translate.y }; }
	float GetUVRotate() { return uvTransform_.rotate.z; }
	Vector2 GetUVScale() { return { uvTransform_.scale.x, uvTransform_.scale.y }; }

		// --- セッター ---
	/// <summary>
	/// ブレンドモードを設定する
	/// これにより使用するパイプラインステートが切り替わる
	/// </summary>
	void SetBlendMode(BlendMode blendMode);

	void SetPosition(const Vector2& position) { position_ = position; }
	void SetRotation(float rotation) { rotation_ = rotation; }
	void SetSize(const Vector2& size) { size_ = size; }
	void SetColor(const Vector4& color) { materialData->color = color; }

	/// <summary>
	/// テクスチャを変更し、サイズをリセットする
	/// </summary>
	void SetTexture(std::string textureFilePath);

	/// <summary>
	/// アンカーポイントを設定する (0.0f ～ 1.0f)
	/// 例: {0.5f, 0.5f} で中心基準
	/// </summary>
	void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }

	void SetIsFlipX(bool isFlipX) { isFlipX_ = isFlipX; }
	void SetIsFlipY(bool isFlipY) { isFlipY_ = isFlipY; }

	// UV関連セッター
	void SetUVTransform(const Transform& uvTransform) { uvTransform_ = uvTransform; }
	void SetUVTranslate(const Vector2& translate) { uvTransform_.translate = { translate.x, translate.y, 0.0f }; }
	void SetUVRotate(float rotate) { uvTransform_.rotate = { 0.0f, 0.0f, rotate }; }
	void SetUVScale(const Vector2& scale) { uvTransform_.scale = { scale.x, scale.y, 1.0f }; }
};

} // namespace Engine
