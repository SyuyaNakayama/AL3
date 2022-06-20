#include "EnemyBullet.h"
#include <assert.h>

using namespace MathUtility;

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("picture/missile.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_.x = 2.0f;
	velocity_ = velocity;
	deathTimer_ = LIFE_TIME;
	isDead_ = 0;

	worldTransform_.rotation_.y = std::atan2(velocity.x, velocity.z) + PI / 2;
	float length = velocity.x * velocity.x + velocity.z * velocity.z;
	worldTransform_.rotation_.z = std::atan2(-velocity.y, length);
}

void EnemyBullet::Update() {
	Vector3 toPlayer = *player_ - worldTransform_.translation_;

	toPlayer.normalize();
	velocity_.normalize();

	worldTransform_.translation_ += velocity_;

	if (--deathTimer_ <= 0) {
		isDead_ = 1;
	}

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
