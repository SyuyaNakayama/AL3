#include "Enemy.h"
#include <assert.h>
#include "Player.h"

void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("picture/enemy.png");
	debugText_ = DebugText::GetInstance();
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_.y = 2.0f;
	fireTimer = 0;
	ApproachInit();
}

void Enemy::Approach() {
	const Vector3 APPROACH_SPD = { 0, 0, -0.2f };
	worldTransform_.translation_ += APPROACH_SPD;

	if (--fireTimer > 0) {
		return;
	}

	Fire();
	fireTimer = FIRE_INTERVAL;

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::ApproachInit() { fireTimer = FIRE_INTERVAL; }

void Enemy::Leave() {
	const Vector3 LEAVE_SPD = { -0.2f, 0.2f, -0.2f };
	worldTransform_.translation_ += LEAVE_SPD;
}

void Enemy::Fire() {
	assert(player_);
	
	const float BULLET_SPD = 1.0f;
	Vector3 playerPos=player_->GetWorldPosition();
	Vector3 enemyPos=GetWorldPosition();
	Vector3 velocity = playerPos - enemyPos;
	velocity.normalize();
	velocity *= BULLET_SPD;

	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	bullets_.push_back(std::move(newBullet));
}

void Enemy::Update() {
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });
	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
		Approach();
		break;
	case Enemy::Phase::Leave:
		Leave();
		break;
	}

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 70);
	debugText_->Printf(
		"enemyPos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

Vector3 Enemy::GetWorldPosition()
{
	return Vector3(worldTransform_.translation_);
}