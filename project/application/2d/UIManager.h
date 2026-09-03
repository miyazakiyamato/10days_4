#pragma once
#include "BaseUI.h"
#include <vector>
#include <memory>
#include <string>

namespace Engine {

/// <summary>
/// UIマネージャクラス
/// </summary>
class UIManager {
public:
    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

    /// <summary>
    /// ImGui更新
    /// </summary>
    void ImGuiUpdate();

    /// <summary>
    /// UIの追加
    /// </summary>
    /// <param name="ui">追加するUI</param>
    void AddUI(std::unique_ptr<BaseUI> ui);

    /// <summary>
    /// 全削除
    /// </summary>
    void Clear();

private:
    // UIリスト
    std::vector<std::unique_ptr<BaseUI>> uiList_;
};

} // namespace Engine
