#include "Enemy.h"
#include <assert.h>

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("picture/enemy.png");
	debugText_ = DebugText::GetInstance();
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_.y = 2.0f;
	velocity_ = velocity;
}

void Enemy::Approach() {
	const Vector3 APPROACH_SPD = {0, 0, -0.2f};
	worldTransform_.translation_ += APPROACH_SPD;

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {
	const Vector3 LEAVE_SPD = {-0.2f, 0.2f, -0.2f};
	worldTransform_.translation_ += LEAVE_SPD;
}

void (Enemy::*Enemy::pPhaseFuncTable[])() = {&Enemy::Approach, &Enemy::Leave};

void Enemy::Update() {
	(this->*pPhaseFuncTable[static_cast<size_t>(phase_)])();

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "enemyPos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}