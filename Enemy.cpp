#include "Enemy.h"
#include <assert.h>
#include "Player.h"
#include "GameScene.h"

void Enemy::Initialize(Model* model, const Vector3& position,Player* player) {
	model_ = model;
	textureHandle_ = TextureManager::Load("picture/enemy.png");
	debugText_ = DebugText::GetInstance();
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_.y = 2.0f;
	fireTimer = 0;
	player_ = player;
	ApproachInit();
}

void Enemy::Approach() {
	const Vector3 APPROACH_SPD = { 0, 0, -0.2f };
	worldTransform_.translation_ += APPROACH_SPD;

	if (--fireTimer > 0) { return; }

	Fire();
	fireTimer = FIRE_INTERVAL;

	if (worldTransform_.translation_.z < 0.0f) { phase_ = Phase::Leave; }
}

void Enemy::ApproachInit() { fireTimer = FIRE_INTERVAL; }

void Enemy::Leave() {
	const Vector3 LEAVE_SPD = { -0.2f, 0.2f, -0.2f };
	worldTransform_.translation_ += LEAVE_SPD;
}

void Enemy::Fire() 
{
	const float BULLET_SPD = 1.0f;
	Vector3 playerPos = player_->GetPosition();
	Vector3 enemyPos = GetPosition();
	Vector3 velocity = playerPos - enemyPos;
	velocity.normalize();
	velocity *= BULLET_SPD;

	unique_ptr<EnemyBullet> newBullet = make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	gameScene_->AddEnemyBullet(move(newBullet));
}

void Enemy::Update() 
{
	switch (phase_) 
	{
	case Enemy::Phase::Approach:
	default:
		Approach();
		break;
	case Enemy::Phase::Leave:
		Leave();
		break;
	}

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 70);
	debugText_->Printf(
		"enemyPos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

void Enemy::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}