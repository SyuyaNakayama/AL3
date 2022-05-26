#include "PlayerBullet.h"
#include <assert.h>

void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("picture/missile1.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_.x = 2.0f;
}

void PlayerBullet::Update() {
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewprojection) {
	model_->Draw(worldTransform_, viewprojection, textureHandle_);
}