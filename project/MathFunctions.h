#pragma once
#pragma once
#define _USE_MATH_DEFINES
#include "Vector3.h"
#include <Matrix4x4.h>
#include <assert.h>
#include <cmath>
#include <math.h>
#include <time.h>
#include "MyMtMatrix.h"

// 1. X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);

// 2. Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);

// 3. Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// アフィン変換行列計算関数
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

float Lerp(float a, float b, float t);

// Vector3 lerp(Vector3& start, Vector3& end, float t);

Vector3 Add(const Vector3& v1, const Vector3& v2);

float degreesToRadians(float degrees);

bool AABBCollision(const AABB& a, const AABB& b);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

// イージング
float EaseIN(float x);

float EaseInSine(float t);

float EaseInQuad(float t);

float EaseInBack(float t);

float EaseInCirc(float t);

float easeOutElastic(float t);

float easeInOut(float x);

float easeInOutElastic(float t);

float easeOutBounce(float t);

float easeInOutSine(float playerPosX, float startPosX, float endPosX, float t);

float EaseInOutQuad(float t);

float EaseInOutSine(float t);

float EaseOutCubic(float t);