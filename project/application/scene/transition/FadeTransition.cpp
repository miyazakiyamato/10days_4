#include "FadeTransition.h"
#include "SceneManager.h"
#include "TimeManager.h"

namespace Engine {

void FadeTransition::Initialize() {
    // スプライトの初期化
    for (uint32_t i = 0; i < 1; ++i) {
        std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
        sprite->Initialize("white.dds");
        sprite->SetPosition({0, 0});
        sprite->SetSize({1280.0f, 720.0f});
        sprite->SetColor({0.0f, 0.0f, 0.0f, 1.0f});
        sprites_.push_back(std::move(sprite));
    }
    counter_ = 0.0f;
    isFinished_ = false;
}

void FadeTransition::Finalize() {
    // 解放
    sprites_.clear();
}

void FadeTransition::Update() {
    counter_ += TimeManager::GetInstance()->kFlamTime_;

    if (counter_ < duration_) {
        float alpha = 0.0f;
        if (type_ == Type::kIn) {
            // 1.0 -> 0.0
            alpha = 1.0f - (counter_ / duration_);
        } else {
            // 0.0 -> 1.0
            alpha = (counter_ / duration_);
        }
        sprites_[0]->SetColor({0.0f, 0.0f, 0.0f, alpha});
    } else {
        if (type_ == Type::kIn) {
            sprites_[0]->SetColor({0.0f, 0.0f, 0.0f, 0.0f});
        } else {
            sprites_[0]->SetColor({0.0f, 0.0f, 0.0f, 1.0f});
        }
		counter_ = 0.0f;
        isFinished_ = true;
    }

    for (std::unique_ptr<Sprite>& sprite : sprites_) {
        sprite->Update();
    }
}

void FadeTransition::Draw() {
    for (std::unique_ptr<Sprite>& sprite : sprites_) {
        sprite->Draw();
    }
}

} // namespace Engine
