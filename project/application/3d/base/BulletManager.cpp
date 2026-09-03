#include "BulletManager.h"
#include "CollisionManager.h"

namespace Engine {



void BulletManager::Update(){
	bullets_.remove_if([](std::unique_ptr<BaseBullet>& bullet) {
		if (!bullet->isAlive()) {
			bullet.reset();
			return true;
		}
		return false;
	});
	for (std::unique_ptr<BaseBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

void BulletManager::Draw(){
	for (std::unique_ptr<BaseBullet>& bullet : bullets_) {
		bullet->Draw();
	}
}

void BulletManager::AddBullet(std::unique_ptr<BaseBullet> bullet){
	if (bullet) {
		bullets_.push_back(std::move(bullet));
	}
}

void BulletManager::AddCollider(CollisionManager* collisionManager){
	for (std::unique_ptr<BaseBullet>& bullet : bullets_) {
		collisionManager->AddCollider(bullet.get());
	}
}

} // namespace Engine
