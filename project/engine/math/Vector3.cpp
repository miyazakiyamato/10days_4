#include "Vector3.h"
#include <cmath>
#include <algorithm>
#include <cassert>
#include "Quaternion.h"
#include "Collision.h"

namespace Engine {

float Vector3::Length() const {
    return Length(*this);
}

Vector3 Vector3::Normalize() const {
    return Normalize(*this);
}

Vector3 Vector3::Abs() const {
    return Abs(*this);
}

Vector3 Vector3::Clamp01() const {
    return Clamp(*this, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
}

Vector3 Vector3::Clamp_11() const {
    return Clamp(*this, { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f });
}

Quaternion Vector3::ToQuaternion() const{
    float pitch = x * 0.5f;
    float yaw = y * 0.5f;
    float roll = z * 0.5f;

    float sinPitch = sinf(pitch);
    float cosPitch = cosf(pitch);
    float sinYaw = sinf(yaw);
    float cosYaw = cosf(yaw);
    float sinRoll = sinf(roll);
    float cosRoll = cosf(roll);

    // クォータニオンの各成分を計算（Z-Y-X順）
    Quaternion q;
    q.x = sinPitch * cosYaw * cosRoll - cosPitch * sinYaw * sinRoll;
    q.y = cosPitch * sinYaw * cosRoll + sinPitch * cosYaw * sinRoll;
    q.z = cosPitch * cosYaw * sinRoll - sinPitch * sinYaw * cosRoll;
    q.w = cosPitch * cosYaw * cosRoll + sinPitch * sinYaw * sinRoll;
    return q;
}

Vector3 Vector3::Add(const Vector3& v1, const Vector3& v2) {
    Vector3 v3;
    v3.x = v1.x + v2.x;
    v3.y = v1.y + v2.y;
    v3.z = v1.z + v2.z;
    return v3;
}

Vector3 Vector3::Subtract(const Vector3& v1, const Vector3& v2) {
    Vector3 v3;
    v3.x = v1.x - v2.x;
    v3.y = v1.y - v2.y;
    v3.z = v1.z - v2.z;
    return v3;
}

Vector3 Vector3::Multiply(float scalar, const Vector3& v) {
    Vector3 v2;
    v2.x = v.x * scalar;
    v2.y = v.y * scalar;
    v2.z = v.z * scalar;
    return v2;
}
Vector3 Vector3::Divide(float scalar, const Vector3& v){
    Vector3 v2;
    v2.x = v.x / scalar;
    v2.y = v.y / scalar;
    v2.z = v.z / scalar;
    return v2;
}
float Vector3::Dot(const Vector3& v1, const Vector3& v2){
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Vector3::Length(const Vector3& v){
    return std::sqrt(Dot(v, v));
}

Vector3 Vector3::Normalize(const Vector3& v){
    return (Vector3)v / Length(v);
}

Vector3 Vector3::Abs(const Vector3& v){
    return { std::fabs(v.x), std::fabs(v.y), std::fabs(v.z) };
}

Vector3 Vector3::Min(const Vector3& v, const Vector3& v2){
    return { std::min(v.x, v2.x), std::min(v.y, v2.y), std::min(v.z, v2.z) };
}

Vector3 Vector3::Max(const Vector3& v, const Vector3& v2){
    return { std::max(v.x, v2.x), std::max(v.y, v2.y), std::max(v.z, v2.z) };
}

Vector3 Vector3::Clamp(const Vector3& v, const Vector3& min, const Vector3& max){
    return { std::clamp(v.x, min.x, max.x), std::clamp(v.y, min.y, max.y), std::clamp(v.z, min.z, max.z) };
}

Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2){
    return { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
}

Vector3 Vector3::Project(const Vector3& v1, const Vector3& v2){
    Vector3 norm_v2 = Normalize(v2);
    return norm_v2 * Dot(v1, norm_v2);
}
Vector3 Vector3::Perpendicular(const Vector3& vector) {
    if (vector.x != 0.0f || vector.y != 0.0f) {
        return { -vector.y,vector.x,0.0f };
    }
    return { 0.0f,-vector.z,vector.y };
}

Vector3 Vector3::Reflect(const Vector3& input, const Vector3& normal) {
    return Vector3(input) - Vector3(normal) * (2.0f * Vector3::Dot(input, normal));
}

Vector3 Vector3::ClosestPoint(const Vector3& point, const Line& line)
{
    return line.origin + Project(point - line.origin, line.diff);
}

Vector3 Vector3::ClosestPoint(const Vector3& point, const Ray& ray)
{
    Vector3 v3 = ray.origin + Project(point - ray.origin, ray.diff);
    return Max(v3, ray.origin);
}

Vector3 Vector3::ClosestPoint(const Vector3& point, const Segment& segment)
{
    Vector3 v3 = segment.origin + Project(point - segment.origin, segment.diff);
    return Clamp(v3, segment.origin, segment.origin + segment.diff);
}

Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t){
    return v1 + (v2 - v1) * t;
}
Vector3 Vector3::Slerp(const Vector3& v1, const Vector3& v2, float t) {
    Vector3 v3;
    Vector3 nV1 = Normalize(v1);
    Vector3 nV2 = Normalize(v2);
    float dot = Dot(nV1, nV2);
    //
    if (dot > 1.0f) {
        dot = 1.0f;
    }
    float theta = std::acos(dot);
    float sinTheta = std::sin(theta);
    float sinThetaFrom = std::sin((1 - t) * theta);
    float sinThetaTo = std::sin(t * theta);
    //
    Vector3 nCompVector;
    if (sinTheta < 1.0e-5) {
        nCompVector = nV1;
    } else {
        nCompVector = Add(Multiply(sinThetaFrom / sinTheta, nV1), Multiply(sinThetaTo / sinTheta, nV2));
    }
    float length1 = Length(v1);
    float length2 = Length(v2);
    float length = Lerp({ length1 }, { length2 }, t).x;

    v3 = Multiply(length, nCompVector);
    return v3;
}
Vector3 Vector3::CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t){
    const float s = 0.5f; // 1/2
    float t2 = t * t;  // t の2乗
    float t3 = t2 * t; // t の3乗

    Vector3 e3 = (Vector3)p0 * -1.0f + (Vector3)p1 * 3.0f - (Vector3)p2 * 3.0f + p3;
    Vector3 e2 = (Vector3)p0 * 2.0f - (Vector3)p1 * 5.0f + (Vector3)p2 * 4.0f - p3;
    Vector3 e1 = (Vector3)p0 * -1.0f + p2;
    Vector3 e0 = (Vector3)p1 * 2.0f;

    return (e3 * t3 + e2 * t2 + e1 * t + e0) * s;
}
/// CatmullRomスプライン曲線上の座標を得る
Vector3 Vector3::CatmullRomPosition(const std::vector<Vector3>& points, float t) {
    // 制御点が2点未満の場合は補間できない
    assert(points.size() >= 2 && "制御点は2点以上必要です");

    // 制御点が2点なら線形補間
    if (points.size() == 2) {
        return Lerp(points[0], points[1], t);
    }

    // 制御点が3点ならp0, p1, p2, p3のうちp0とp3を端点で複製
    // それ以外は通常通り
    size_t n = points.size();
    size_t division = n - 1;
    float areaWidth = 1.0f / division;

    // 区間番号
    size_t index = static_cast<size_t>(t / areaWidth);
    if (index >= division) index = division - 1;

    // 区間内のt
    float t0 = areaWidth * index;
    float t_2 = (t - t0) / areaWidth;
    t_2 = Clamp({ t_2,0,0 }, { 0.0f,0,0 }, { 1.0f,0,0 }).x;

    // 4点分のインデックス
    int index0 = static_cast<int>(index) - 1;
    int index1 = static_cast<int>(index);
    int index2 = static_cast<int>(index) + 1;
    int index3 = static_cast<int>(index) + 2;

    // 端点の複製
    if (index0 < 0) index0 = 0;
    if (index3 >= static_cast<int>(n)) index3 = static_cast<int>(n) - 1;

    const Vector3& p0 = points.at(index0);
    const Vector3& p1 = points.at(index1);
    const Vector3& p2 = points.at(index2);
    const Vector3& p3 = points.at(index3);

    // 4点を指定してCatmul-Rom補間
    return CatmullRomInterpolation(p0, p1, p2, p3, t_2);
}

Vector3 Vector3::Random(std::mt19937& randomEngine,const Vector3& vMin, const Vector3& vMax){
    std::uniform_real_distribution<float> distX(vMin.x, vMax.x);
    std::uniform_real_distribution<float> distY(vMin.y, vMax.y);
    std::uniform_real_distribution<float> distZ(vMin.z, vMax.z);
    return { distX(randomEngine), distY(randomEngine), distZ(randomEngine) };
}

Vector3 Vector3::operator-() const {
    return { -x, -y, -z };
}

Vector3 Vector3::operator+(const Vector3& v2) const {
    return { x + v2.x, y + v2.y, z + v2.z };
}

Vector3 Vector3::operator-(const Vector3& v2) const {
    return { x - v2.x, y - v2.y, z - v2.z };
}

Vector3 Vector3::operator*(float v2) const {
    return { x * v2, y * v2, z * v2 };
}

Vector3 Vector3::operator/(float v2) const {
    return { x / v2, y / v2, z / v2 };
}

Vector3& Vector3::operator+=(const Vector3& v2){
    x += v2.x;
    y += v2.y;
    z += v2.z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& v2){
    x -= v2.x;
    y -= v2.y;
    z -= v2.z;
    return *this;
}

Vector3& Vector3::operator*=(float v2)
{
    x *= v2;
    y *= v2;
    z *= v2;
    return *this;
}

Vector3& Vector3::operator/=(float v2){
    x /= v2;
    y /= v2;
    z /= v2;
    return *this;
}

} // namespace Engine
