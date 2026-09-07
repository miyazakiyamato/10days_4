#include <MathFunctions.h>
#include <numbers>

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result;

	result.m[0][0] = 1;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[1][3] = 0;
	result.m[2][0] = 0;
	result.m[2][1] = std::sin(-radian);
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0;
	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result;

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = 0;
	result.m[0][2] = std::sin(-radian);
	result.m[0][3] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = 1;
	result.m[1][2] = 0;
	result.m[1][3] = 0;
	result.m[2][0] = std::sin(radian);
	result.m[2][1] = 0;
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0;
	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result;

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[0][2] = 0;
	result.m[0][3] = 0;
	result.m[1][0] = std::sin(-radian);
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = 0;
	result.m[1][3] = 0;
	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1;
	result.m[2][3] = 0;
	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) {
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rot.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rot.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rot.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
	Matrix4x4 result;

	result.m[0][0] = scale.x * rotateXYZMatrix.m[0][0];
	result.m[0][1] = scale.x * rotateXYZMatrix.m[0][1];
	result.m[0][2] = scale.x * rotateXYZMatrix.m[0][2];
	result.m[0][3] = 0;
	result.m[1][0] = scale.y * rotateXYZMatrix.m[1][0];
	result.m[1][1] = scale.y * rotateXYZMatrix.m[1][1];
	result.m[1][2] = scale.y * rotateXYZMatrix.m[1][2];
	result.m[1][3] = 0;
	result.m[2][0] = scale.z * rotateXYZMatrix.m[2][0];
	result.m[2][1] = scale.z * rotateXYZMatrix.m[2][1];
	result.m[2][2] = scale.z * rotateXYZMatrix.m[2][2];
	result.m[2][3] = 0;
	result.m[3][0] = translate.x; // rotateXYZMatrix.m[3][0];
	result.m[3][1] = translate.y; // rotateXYZMatrix.m[3][1];
	result.m[3][2] = translate.z; // rotateXYZMatrix.m[3][2];
	result.m[3][3] = 1;

	return result;
}

float EaseIN(float x) { return 1 - cosf((x * float(M_PI)) / 2); }

float EaseInSine(float t) { return 1.0f - cos((t * std::numbers::pi_v<float>) / 2.0f); }

float EaseInQuad(float t) { return t * t; }

float EaseInBack(float t) {
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return c3 * t * t * t - c1 * t * t;
}

float EaseInCirc(float t) { return 1.0f - sqrt(1.0f - (t * t)); }

float easeOutElastic(float t) {
	const float c4 = (2.0f * std::numbers::pi_v<float>) / 3.0f;
	if (t == 0)
		return 0;
	if (t == 1)
		return 1;
	return powf(2, -10 * t) * sinf((t * 10.0f - 0.75f) * c4) + 1.0f;
}

float easeInOut(float x) { return -static_cast<float>((std::cos(M_PI * x) - 1) / 2); }

float easeInOutElastic(float t) {
	const float c5 = (2.0f * std::numbers::pi_v<float>) / 30.0f; // バウンスの強さを少しゆるめるのは最期の値を大きくする基本は4.5f
	if (t == 0)
		return 0;
	if (t == 1)
		return 1;
	if (t < 0.5f) {
		return -(powf(2, 20 * t - 10) * sinf((20 * t - 11.125f) * c5)) / 2;
	}
	return (powf(2, -20 * t + 10) * sinf((20 * t - 11.125f) * c5)) / 2 + 1;
}

float easeOutBounce(float t) {
	float bounceFactor = 4.0f; // バウンドの強さ
	if (t < 1.0f / 2.75f) {
		return bounceFactor * t * t;
	} else if (t < 2.0f / 2.75f) {
		t -= 1.5f / 2.75f;
		return bounceFactor * t * t + 0.6f; // 高さを少し下げる
	} else if (t < 2.5f / 2.75f) {
		t -= 2.25f / 2.75f;
		return bounceFactor * t * t + 0.8f; // 高さを少し下げる
	} else {
		t -= 2.625f / 2.75f;
		return bounceFactor * t * t + 0.9f; // 最後のバウンドも緩やかに
	}
}

float easeInOutSine(float playerPosX, float startPosX, float endPosX, float t) {
	playerPosX = startPosX + (endPosX - startPosX) * easeInOut(t);
	return playerPosX;
}

float EaseInOutQuad(float t) {
	if (t < 0.5f) {
		return 2 * t * t;
	} else {
		return -1 + (4 - 2 * t) * t;
	}
}

float EaseInOutSine(float t) { return -0.5f * static_cast<float>(cos(M_PI * t) - 1); }

float EaseOutCubic(float t) { return 1 - static_cast<float>(pow(1 - t, 3)); }

float Lerp(float a, float b, float t) { return (1 - t) * a + t * b; /*a + (b - a) * t;*/ }

Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 AddResult{};

	AddResult.x = v1.x + v2.x;
	AddResult.y = v1.y + v2.y;
	AddResult.z = v1.z + v2.z;

	return AddResult;
}

float degreesToRadians(float degrees) {
	/* return degrees * (M_PI / 180.0f);*/
	const float PI_F = static_cast<float>(M_PI); // M_PI を float に変換
	return degrees * (PI_F / 180.0f);
}

bool AABBCollision(const AABB& a, const AABB& b) {
	if ((a.min.x <= b.max.x && a.max.x >= b.min.x) && (a.min.y <= b.max.y && a.max.y >= b.min.y) && (a.min.z <= b.max.z && a.max.z >= b.min.z)) {
		return true;
	}
	return false;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

// float degreesToRadians(float degrees) { return degrees * (PI_F / 180.0f); }

// Vector3 lerp(Vector3& start, Vector3& end, float t) {
//	return start + (end - start) * t
// }
