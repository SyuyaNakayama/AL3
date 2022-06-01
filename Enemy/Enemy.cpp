#include "Enemy.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>

void Enemy::Initialize(Model* model, Vector3* playerTranslation)
{
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("picture/enemy.png");
	debugText_ = DebugText::GetInstance();
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 10.0f, 3.0f, 20.0f };
	worldTransform_.scale_ = { 2.5f,5.0f,2.5f };
	srand(time(NULL));
	phase_ = 1;
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
	static int missileCount = 0;
	if (!attackTimer_.CountDown()) { return; }
	
	const float spdConst = 2.0f;
	toPlayer_ *= spdConst;
	std::unique_ptr<EnemyBullet> newMissile = std::make_unique<EnemyBullet>();
	newMissile->Initialize(model_, worldTransform_.translation_, toPlayer_);
	missiles_.push_back(std::move(newMissile));
	if(++missileCount>=10)
	{
		isActionEnd = 1;
		missileCount = 0;
	}
}

void Enemy::Bomb()
{

}

void Enemy::Press()
{

}

void Enemy::Tackle()
{
	worldTransform_.translation_ += tackleSpd;
}

void Enemy::Summon()
{

}

void Enemy::Update()
{
	missiles_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->isDead_; });

	toPlayer_= *playerTranslation_ - worldTransform_.translation_;
	toPlayer_.y = 0;
	toPlayer_.normalize();
	(this->*pPhaseFuncTable[phase_])();
	if(isActionEnd)
	{
		phase_ = 4;
		tackleSpd = toPlayer_ * 1.5f;
		isActionEnd=0;
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