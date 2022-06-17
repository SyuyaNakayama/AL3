#include "Bomb.h"
#include <assert.h>

void Bomb::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("picture/bomb.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
	isDead_ = 0;
}

void Bomb::Update()
{
	if (isDead_) { return; }

	worldTransform_.translation_ += velocity_;
	velocity_.y -= 0.03f;
	if (worldTransform_.translation_.y <= 0) { isDead_ = 1; }

	worldTransform_.translation_.ShowVector({ 0, 0 });
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void Bomb::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}