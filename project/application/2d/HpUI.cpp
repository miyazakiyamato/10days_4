#include "HpUI.h"
#include "Player.h"
#include "TimeManager.h"
#include "Easing.h"

namespace Engine {

namespace {
	const Vector2 kHpBarPosition = { 30.0f, 30.0f };
}

void HpUI::Initialize(Player* player){
	player_ = player;
	maxHp_ = (float)player_->GetHp();
	preHp_ = maxHp_;
	//スプライトの初期化
	for (uint32_t i = 0; i < 3; ++i) {
		std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
		sprite->Initialize("HPBar.dds");
		sprite->SetPosition(kHpBarPosition);
		sprite->SetSize(hpSize_);
		sprites_.push_back(std::move(sprite));
	}
	sprites_[0]->SetColor({ 0.5f,0.5f,0.5f,1.0f });//失ったHP
	sprites_[1]->SetColor({ 1.0f,0.0f,0.0f,1.0f });//ダメージHP
	sprites_[2]->SetColor({ 0.0f,1.0f,0.0f,1.0f });//通常HP
}

void HpUI::Update(){
	damegeHpTimer_ += TimeManager::GetInstance()->deltaTime_;
	float nowHp = (float)player_->GetHp();
	if (nowHp != preHp_) {
		damageHp_ = easingDamageHp_;
		preHp_ = nowHp;
		damegeHpTimer_ = 0.0f;
	}
	if (damegeHpTimer_ >= damegeHpLimitTime_) {
		damageHp_ = nowHp;
	}
	float t = damegeHpTimer_ / damegeHpLimitTime_;
	easingDamageHp_ = Easing::EaseOutCubic(t, damageHp_, nowHp);

	float damegeHpRatio = easingDamageHp_ / maxHp_;
	float nowHpRatio = nowHp / maxHp_;
	sprites_[1]->SetSize({ hpSize_.x * damegeHpRatio,hpSize_.y });
	sprites_[1]->SetUVScale({ damegeHpRatio,1.0f });
	sprites_[2]->SetSize({ hpSize_.x * nowHpRatio,hpSize_.y });
	sprites_[2]->SetUVScale({ nowHpRatio,1.0f });

	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Update();
	}
}

void HpUI::Draw(){
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw();
	}
}

void HpUI::ImGuiUpdate(){
	uint32_t spriteIDIndex = 0;
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		std::string spriteName = ("HPUI" + std::to_string(spriteIDIndex)).c_str();
		sprite->ImGuiUpdate(spriteName);
		++spriteIDIndex;
	}
}

} // namespace Engine
