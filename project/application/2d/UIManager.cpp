#include "UIManager.h"
#include <algorithm>

namespace Engine {

void UIManager::Update() {
    // UIの更新
    for (auto& ui : uiList_) {
        ui->Update();
    }

    // UIの死活監視
    uiList_.erase(std::remove_if(uiList_.begin(), uiList_.end(), [](const std::unique_ptr<BaseUI>& ui) {
        return ui->GetIsDead();
        }), uiList_.end());
}

void UIManager::Draw() {
    for (auto& ui : uiList_) {
        ui->Draw();
    }
}

void UIManager::ImGuiUpdate() {
    for (auto& ui : uiList_) {
        ui->ImGuiUpdate();
    }
}

void UIManager::AddUI(std::unique_ptr<BaseUI> ui) {
    uiList_.push_back(std::move(ui));
}

void UIManager::Clear() {
    uiList_.clear();
}

} // namespace Engine
