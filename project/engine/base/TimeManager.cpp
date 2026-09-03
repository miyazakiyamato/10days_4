#include "TimeManager.h"
#include <cassert>

namespace Engine {

std::unique_ptr<TimeManager> TimeManager::instance = nullptr;

TimeManager* TimeManager::GetInstance(){
	if (instance == nullptr) {
		instance = std::make_unique<TimeManager>(PrivateToken{});
	}
	return instance.get();
}

void TimeManager::Finalize(){
	instance.reset();
}

void TimeManager::Initialize(DirectXCommon* dxCommon){
	dxCommon_ = dxCommon;
	deltaTime_ = kFlamTime_;
	// フレームごとの時間情報のリソースを作成
	perFrameResource_ = dxCommon_->CreateBufferResource(sizeof(PerFrame));
	perFrameResource_->Map(0, nullptr, reinterpret_cast<void**>(&perFrameData_));
	// フレームごとの時間情報を初期化
	perFrameData_->time = 0.0f; // 初期時間
	perFrameData_->deltaTime = deltaTime_; // 初期1フレームの経過時間
}

void TimeManager::Update(){
	if (timeSpeed_ != deltaTime_) {
		deltaTime_ = timeSpeed_;
	}

	if (timeCount_ != 0.0f) {
		timeCount_ += kFlamTime_;
		if (timeCount_ > kLimitTime_) {
			timeCount_ = 0.0f;
			deltaTime_ = kFlamTime_;
		}
	}
	for (auto& [name, timer] : timers_) {
		if (timer.isStart == true) {
			timer.timeCount += deltaTime_;
			if (timer.timeCount > timer.kLimitTime) {
				timer.timeCount = 0.0f;
				timer.isStart = false;
			}
		}
	}
	timeSpeed_ = deltaTime_;
	perFrameData_->time += deltaTime_;
	perFrameData_->deltaTime = deltaTime_;
}

void TimeManager::TimeSpeedReset(){
	deltaTime_ = kFlamTime_;
}

void TimeManager::SetDeltaTimeSpeedStart(float timeSpeed, float kLimitTime){
	kLimitTime_ = kLimitTime;
	timeSpeed_ = timeSpeed;
	timeCount_ = kFlamTime_;
}

void TimeManager::SetTimer(std::string name, float kLimitTime){
	Timer& timer = timers_[name];
	timer.isStart = true;
	timer.kLimitTime = kLimitTime;
	timer.timeCount = 0.0f;
}

} // namespace Engine
