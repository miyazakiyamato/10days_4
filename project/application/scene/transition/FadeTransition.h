#pragma once
#include "BaseTransition.h"
#include "Sprite.h"
#include <vector>
#include <memory>

namespace Engine {

/// <summary>
/// フェード遷移シーン
/// </summary>
class FadeTransition : public BaseTransition {
public: // メンバ関数
    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize() override;
    /// <summary>
    /// 終了
    /// </summary>
    void Finalize() override;
    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void Update() override;
    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;

private: // メンバ変数
    std::vector<std::unique_ptr<Sprite>> sprites_;
    // フェードの持続時間
    float duration_ = 0.5f;
    // 経過時間カウンター
    float counter_ = 0.0f;
};

} // namespace Engine
