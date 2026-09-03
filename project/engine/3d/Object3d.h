#pragma once
#include "DirectXCommon.h"
#include "Model.h"
#include "Camera.h"
#include "LightManager.h"
#include "Animation.h"
#include "Skeleton.h"
#include "SkinCluster.h"

namespace Engine {

/// <summary>
/// 3Dオブジェクトクラス
/// ワールド空間上の1つのオブジェクト実体を表す
/// モデル、アニメーション、スケルトンなどのコンポーネントを所有・管理する
/// </summary>
class Object3d {
private:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
		// --- 構造体 ---
	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInverseTranspose; // 法線変換用
	};
	struct CameraForGpu {
		Vector3 worldPosition;
		float padding;
	};
	struct AnimationData {
		Animation* animation;
		float time = 0.0f;
		bool isLoop = false;
	};
	struct Material {
		Vector4 color{ 1,1,1,1 };
		Vector4 highLightColor{ 1,1,1,1 };
		Matrix4x4 uvTransform;
		int enableLighting;           // ライティング有効フラグ
		float shininess;              // 光沢度 (スペキュラ)
		int enableEnvironmentMap;     // 環境マップ有効フラグ
		float environmentCoefficient; // 環境マップ寄与度
	};
	struct MaterialData {
		std::string textureFilePath_ = "";
		Transform uvTransform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
		Material* material = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	};

		// --- メンバ関数 ---
	Object3d() = default;
	~Object3d();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// 行列計算、アニメーション進行、スケルトン更新を行う
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ImGui更新処理
	/// </summary>
	void ImGuiUpdate(const std::string& name);

private:
		// --- メンバ変数 ---
	DirectXCommon* dxCommon_ = nullptr;

	Model* model_ = nullptr;
	std::vector<MaterialData> materialData_;
	BlendMode blendMode_ = BlendMode::kNormal;
	std::string pipelineStateName_ = "";
	std::string computeShaderPipelineName_ = "";

	// バッファリソース
	ComPtr<ID3D12Resource> wvpResource;
	ComPtr<ID3D12Resource> cameraResource;

	// マッピングポインタ
	TransformationMatrix* wvpData = nullptr;
	CameraForGpu* cameraData = nullptr;
	LightManager* lightManager_ = nullptr;

	// トランスフォーム
	Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Matrix4x4 worldMatrix_ = Matrix4x4::MakeIdentity4x4();

	// アニメーション・スキニング関連
	std::unique_ptr<AnimationData> animationData_ = nullptr;
	std::unique_ptr<AnimationData> nextAnimationData_ = nullptr; // ブレンド用次アニメーション
	std::unique_ptr<Skeleton> skeletonData_ = nullptr;
	std::unique_ptr<SkinCluster> skinClusterData_ = nullptr;
	float lerpTime_ = 0.1f; //!< アニメーションブレンド時間

	Object3d* parent_ = nullptr; // 親オブジェクト

	bool isSetWorldMatrix_ = false;       //!< WorldMatrixを外部からセットするか
	bool isDrawSkeleton_ = false;         //!< Skeleton(骨)を描画するか
	bool isSkybox_ = false;               //!< SkyBoxとして描画するか
	std::string environmentTextureFilePath_ = ""; // 環境マップテクスチャパス

public:
		// --- ゲッター ---
	Object3d* GetParent() const { return parent_; }
	const BlendMode& GetBlendMode() { return blendMode_; }
	const Vector3& GetScale() const { return transform.scale; }
	const Vector3& GetRotate() const { return transform.rotate; }
	const Vector3& GetTranslate() const { return transform.translate; }
	const Vector3& GetUvScale(uint32_t num) const { return materialData_[num].uvTransform.scale; }
	const Vector3& GetUvRotate(uint32_t num) const { return materialData_[num].uvTransform.rotate; }
	const Vector3& GetUvTranslate(uint32_t num) const { return materialData_[num].uvTransform.translate; }
	const Vector4& GetColor(uint32_t num) const { return materialData_[num].material->color; }
	const Vector4& GetHighLightColor(uint32_t num) const { return materialData_[num].material->highLightColor; }
	Vector3 GetCenterPosition() const;
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }
	bool GetEnableLighting(uint32_t num) const { return materialData_[num].material->enableLighting; }
	float GetShininess(uint32_t num) const { return materialData_[num].material->shininess; }
	bool GetEnableEnvironmentMap(uint32_t num) const { return materialData_[num].material->enableEnvironmentMap; }
	float GetEnvironmentCoefficient(uint32_t num) const { return materialData_[num].material->environmentCoefficient; }
	bool GetIsDrawSkeleton() const { return isDrawSkeleton_; }
	Matrix4x4 GetJointMatrix(std::string jointName) const; 
	Vector3 GetJointsPosition(std::string jointName);
		// --- セッター ---
	void SetParent(Object3d* parent) { parent_ = parent; }
	void SetWorldMatrix(const Matrix4x4& worldMatrix);
	void SetTexture(const std::string& textureFilePath) { materialData_[0].textureFilePath_ = textureFilePath; }
	void SetTexture(const std::string& textureFilePath, uint32_t num) { materialData_[num].textureFilePath_ = textureFilePath; }
	void SetEnvironmentTexture(const std::string& cubeTextureFilePath);
	void SetModel(const std::string& filePath);
	void SetAnimation(const std::string& filePath,bool isLoop);
	void SetBlendMode(const BlendMode& blendMode);
	void SetScale(const Vector3& scale) { transform.scale = scale; }
	void SetRotate(const Vector3& rotate) { transform.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { transform.translate = translate; }
	void SetUvScale(const Vector3& scale, uint32_t num) { materialData_[num].uvTransform.scale = scale; }
	void SetUvRotate(const Vector3& rotate, uint32_t num) { materialData_[num].uvTransform.rotate = rotate; }
	void SetUvTranslate(const Vector3& translate, uint32_t num) { materialData_[num].uvTransform.translate = translate; }
	void SetColor(const Vector4& color, uint32_t num = 0) { materialData_[num].material->color = color; }
	void SetHighLightColor(const Vector4& color, uint32_t num) { materialData_[num].material->highLightColor = color; }
	void SetEnableLighting(bool enable, uint32_t num) { materialData_[num].material->enableLighting = enable; }
	void SetShininess(float shininess, uint32_t num) { materialData_[num].material->shininess = shininess; }
	void SetEnableEnvironmentMap(bool enable, uint32_t num) { materialData_[num].material->enableEnvironmentMap = enable; }
	void SetEnvironmentCoefficient(float coefficient, uint32_t num) {materialData_[num].material->environmentCoefficient = coefficient;}
	void SetIsDrawSkeleton(bool isDraw) { isDrawSkeleton_ = isDraw; }
};


} // namespace Engine
