#pragma once
#include "d3dx12.h"
#include "Quaternion.h"
#include "BlendMode.h"
#include "Collision.h"
#include <memory>

namespace Engine {

class SrvUavManager;

/// <summary>
/// 3Dライン描画マネージャ（シングルトン）
/// デバッグ用などに、線分、グリッド、各種コライダー形状などを描画する機能を提供する
/// </summary>
class Line3dManager {
private:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
		// --- 構造体定義 ---
	/// <summary>
	/// 頂点データ構造体
	/// </summary>
	struct VertexData {
		Vector4 position;
		Vector4 color;
	};

	/// <summary>
	/// 変換行列用定数バッファ構造体
	/// </summary>
	struct TransformationMatrix {
		Matrix4x4 WVP;
	};

		// --- メンバ関数 ---
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	static Line3dManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// 頂点バッファの確保などを行う
	/// </summary>
	void Initialize(SrvUavManager* srvUavManager);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 更新処理
	/// バッファへの転送などを行う
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// 蓄積された頂点データをもとにドローコールを発行する
	/// </summary>
	void Draw();

		// --- プリミティブ描画関数群 ---
	/// <summary>
	/// 2点間に線を描画する
	/// </summary>
	void DrawLine(const Vector3& pos1, const Vector3& pos2, const Vector4& color);

	/// <summary>
	/// 球を描画する
	/// </summary>
	void DrawSphere(const Sphere& sphere, const Vector4& color, const uint32_t& subdivision);

	/// <summary>
	/// 行列指定で球を描画する
	/// </summary>
	void DrawSphere(const Matrix4x4& worldMatrix, const Vector4& color, const uint32_t& subdivision);

	/// <summary>
	/// グリッド床を描画する
	/// </summary>
	/// <param name="gridHalfLength">グリッドの半分のサイズ</param>
	void DrawGrid(const Vector2 gridHalfLength);

	/// <summary>
	/// 平面を描画する
	/// </summary>
	void DrawPlane(const Plane& plane, const Vector4& color);

	/// <summary>
	/// 三角形を描画する
	/// </summary>
	void DrawTriangle(const Triangle& triangle, const Vector4& color);

	/// <summary>
	/// AABB（軸平行境界ボックス）を描画する
	/// </summary>
	void DrawAABB(const AABB& aabb, const Vector4& color);

	/// <summary>
	/// OBB（有向境界ボックス）を描画する
	/// </summary>
	void DrawOBB(const OBB& obb, const Vector4& color);

	/// <summary>
	/// ベジェ曲線を描画する（3点制御）
	/// </summary>
	void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Vector4& color);

	/// <summary>
	/// Catmull-Romスプライン曲線を描画する（4点制御）
	/// </summary>
	void DrawCotmullRom(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Vector3& controlPoint3, const Vector4& color);

private:
	// 外部からは絶対にインスタンス化できない「鍵」となる構造体を定義
	struct PrivateToken {};
public:
	// シングルトン(make_uniqueする用)
	explicit Line3dManager(PrivateToken) {}
	~Line3dManager() = default;
	// コピー・ムーブの禁止
	Line3dManager(const Line3dManager&) = delete;
	Line3dManager& operator=(const Line3dManager&) = delete;

private:
		// --- シングルインスタンス ---
	static std::unique_ptr<Line3dManager> instance;

		// --- メンバ変数 ---
	// ポインタ
	SrvUavManager* srvUavManager_ = nullptr;

	// バッファリソース
	ComPtr<ID3D12Resource> vertexResource;
	ComPtr<ID3D12Resource> wvpResource;

	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	TransformationMatrix* wvpData = nullptr;

	// バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	// 描画用頂点リスト（毎フレームクリア・再構築される）
	std::list<VertexData> vertices;
	size_t preVerticesSize = 0; // 前フレームの頂点数（バッファサイズ調整用）

	// パイプラインステート名
	std::string pipelineStateName_ = "";
};

} // namespace Engine
