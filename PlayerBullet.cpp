#include "PlayerBullet.h"
#include <assert.h>
#include "CollisionConfig.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("picture/playerbullet.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
	deathTimer_ = LIFE_TIME;
	isDead_ = 0;
	SetCollisionAttribute(CollisionAttribute::Player);
	SetCollisionMask(~CollisionAttribute::Player);
}

void PlayerBullet::Update() {
	worldTransform_.translation_ += velocity_;

	if (--deathTimer_ <= 0) { isDead_ = 1; }

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}