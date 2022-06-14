#include "Enemy.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "function.h"

void Enemy::Initialize(Model* model, Vector3* playerTranslation, ViewProjection* viewProjection)
{
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("picture/enemy.png");
	debugText_ = DebugText::GetInstance();
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 10.0f, 3.0f, 20.0f };
	worldTransform_.scale_ = { 2.5f,5.0f,2.5f };
	srand(time(NULL));
	phase_ = Phase::bomb;
	viewProjection_ = viewProjection;
	playerTranslation_ = playerTranslation;
	attackTimer_ = 20;
	isActionEnd = 0;
	tackleSpd = { 0,0,0 };
}

void (Enemy::* Enemy::pPhaseFuncTable[])() =
{
	&Enemy::Beam,&Enemy::Missile,&Enemy::Bomb,
	&Enemy::Press,&Enemy::Tackle,&Enemy::Summon
};

void Enemy::Beam()
{

}

void Enemy::Missile()
{
	missiles_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->isDead_; });

	static int missileCount = 0;
	if (!attackTimer_.CountDown()) { return; }

	const float spdConst = 2.0f;
	toPlayer_ *= spdConst;
	std::unique_ptr<EnemyBullet> newMissile = std::make_unique<EnemyBullet>();
	newMissile->Initialize(model_, worldTransform_.translation_, toPlayer_);
	missiles_.push_back(std::move(newMissile));
	if (++missileCount >= 10)
	{
		isActionEnd = 1;
		missileCount = 0;
		missiles_.remove_if([](std::unique_ptr< EnemyBullet >& bullet) { return 1; });
	}
}

void Enemy::Bomb()
{
	WorldTransform worldTransform;
	worldTransform.Initialize();
	static bool isStart = 0;
	uint32_t texture = TextureManager::Load("Picture/bomb.png");
	static float spd=1.0f;

	if (!isStart)
	{
		worldTransform.translation_ = worldTransform_.translation_;
		worldTransform.translation_.y = worldTransform_.scale_.y;
		isStart = 1;
	}
	else
	{
		worldTransform.translation_.y += spd;
		spd -= 0.1f;
	}

	model_->Draw(worldTransform, *viewProjection_, texture);
}

void Enemy::Press()
{

}

void Enemy::Tackle()
{
	static bool isStart = 0;
	if (!isStart)
	{
		tackleSpd = toPlayer_ * 1.5f;
		isStart = 1;
	}
	else
	{
		worldTransform_.translation_ += tackleSpd;
		if (isOver(worldTransform_.translation_.x) || isOver(worldTransform_.translation_.z))
		{
			worldTransform_.translation_ += tackleSpd;
			isStart = 0;
			isActionEnd = 1;
		}
	}
}

void Enemy::Summon()
{

}

void Enemy::Update()
{
	toPlayer_ = *playerTranslation_ - worldTransform_.translation_;
	toPlayer_.y = 0;
	toPlayer_.normalize();
	(this->*pPhaseFuncTable[phase_])();
	if (isActionEnd)
	{
		phase_ = 4;
		isActionEnd = 0;
	}

	for (std::unique_ptr<EnemyBullet>& bullet : missiles_) {
		bullet->Update();
	}

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (std::unique_ptr<EnemyBullet>& bullet : missiles_) {
		bullet->Draw(viewProjection);
	}
}