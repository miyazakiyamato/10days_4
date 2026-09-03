#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

namespace Engine {

/// <summary>
/// 3Dカメラクラス
/// ビュー行列とプロジェクション行列を管理・計算する
/// </summary>
class Camera {
public:
		// --- 構造体 ---
	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};
		// --- メンバ変数 ---
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera();

	/// <summary>
	/// 更新処理
	/// カメラのパラメータからView行列とProjection行列を再計算する
	/// </summary>
	void Update();

private:
		// --- メンバ変数 ---
	Transform transform;
	Matrix4x4 worldMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 viewProjectionMatrix;

	// 垂直方向視野角
	float fovAngleY_;
	// ビューポートのアスペクト比
	float aspectRatio_;
	// 深度限界前 (Near Clip)
	float nearClip_;
	// 深度限界奥 (Far Clip)
	float farClip_;

public:
		// --- セッター ---
	void SetRotate(const Vector3& rotate) { transform.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { transform.translate = translate; }
	void SetFovAngleY(float fovAngleY) { fovAngleY_ = fovAngleY; }
	void SetAspectRatio(float aspectRatio) { aspectRatio_ = aspectRatio; }
	void SetNearClip(float nearClip) { nearClip_ = nearClip; }
	void SetFarClip(float farClip) { farClip_ = farClip; }

		// --- ゲッター ---
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix; }
	const Matrix4x4& GetViewMatrix() const { return viewMatrix; }
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix; }

	/// <summary>
	/// ビュープロジェクション行列を取得する (View * Projection)
	/// </summary>
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix; }

	const Vector3& GetRotate() const { return transform.rotate; }
	const Vector3& GetTranslate() const { return transform.translate; }
};

} // namespace Engine
