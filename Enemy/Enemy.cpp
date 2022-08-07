#include "Enemy.h"
#include <assert.h>
#include <stdlib.h>
#include "function.h"
#include "Collider/CollisionConfig.h"
#include <DirectXMath.h>
#include <random>

void Enemy::Initialize(Model* model, Vector3* playerTranslation, ViewProjection* viewProjection)
{
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("picture/enemy.png");
	debugText_ = DebugText::GetInstance();
	worldTransform_.translation_ = { 10.0f, 3.0f, 20.0f };
	worldTransform_.scale_ = { 2.5f,5.0f,2.5f };
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	playerTranslation_ = playerTranslation;
	SetCollisionAttribute(CollisionAttribute::CEnemy);
	SetRadius(4.0f);
	SetCollisionMask(CollisionMask::CEnemyMask);
	hp_ = 500;
}

void (Enemy::* Enemy::pPhaseFuncTable[])() =
{
	&Enemy::Beam,&Enemy::Missile,&Enemy::BombAction,
	&Enemy::Press,&Enemy::Tackle
};

void Enemy::Beam()
{
	if (!isStart)
	{
		if (beamTimer_.CountDown())
		{
			beam_.Initialize(model_, state);
			isStart = 1;
		}
	}
	if (isStart)
	{
		beam_.Update();
		if (beamTimer_.CountDown())
		{
			isActionEnd = 1;
			beam_.Clear();
		}
	}
}

void Enemy::Missile()
{
	static int missileCount = 0;

	switch (state)
	{
	case Enemy::Easy:
		if (missileInterval_.CountDown())
		{
			toPlayer_ *= 2.0f;
			std::unique_ptr<EnemyBullet> newMissile = std::make_unique<EnemyBullet>();
			newMissile->Initialize(model_, worldTransform_.translation_, toPlayer_);
			missiles_.push_back(std::move(newMissile));
			missileCount++;
		}
		if (missileCount >= 8)
		{
			isActionEnd = 1;
			missileCount = 0;
		}
		break;
	case Enemy::Normal:
		if (missileInterval_.CountDown())
		{
			Vector3 velocity{};
			int offset = rand() % 360;
			for (size_t i = 0; i < 8; i++)
			{
				std::unique_ptr<EnemyBullet> newMissile = std::make_unique<EnemyBullet>();
				velocity.x = sinf(DirectX::XM_2PI / 8.0f * (float)i + DirectX::XMConvertToRadians(offset));
				velocity.z = cosf(DirectX::XM_2PI / 8.0f * (float)i + DirectX::XMConvertToRadians(offset));
				velocity *= 2.0f;
				newMissile->Initialize(model_, worldTransform_.translation_, velocity);
				missiles_.push_back(std::move(newMissile));
			}
			missileCount++;
		}
		if (missileCount >= 5)
		{
			isActionEnd = 1;
			missileCount = 0;
		}
		break;
	case Enemy::Hard:
		if (!isStart)
		{
			missileInterval_ = 2;
			isStart = 1;
		}
		if (isStart)
		{
			if (missileInterval_.CountDown())
			{
				Vector3 velocity{};

				std::unique_ptr<EnemyBullet> newMissile = std::make_unique<EnemyBullet>();
				velocity.x = sinf(DirectX::XMConvertToRadians(missileCount * 3));
				velocity.z = cosf(DirectX::XMConvertToRadians(missileCount * 3));
				velocity *= 2.0f;
				newMissile->Initialize(model_, worldTransform_.translation_, velocity);
				missiles_.push_back(std::move(newMissile));
				missileCount++;
			}
		}
		if (missileCount >= 200)
		{
			isActionEnd = 1;
			missileCount = 0;
		}
		break;
	}
}

void Enemy::BombAction()
{
	static int bombCount = 0;

	switch (state)
	{
	case Enemy::Easy:
		if (bombTimer_.CountDown())
		{
			Vector3 bombSpd = (*playerTranslation_ - worldTransform_.translation_) / 102.0f;
			bombSpd.y = 1.5f;
			std::unique_ptr<Bomb> newBomb_ = std::make_unique<Bomb>();
			newBomb_->Initialize(model_, worldTransform_.translation_, bombSpd);
			bomb_.push_back(std::move(newBomb_));
			bombCount++;
		}
		if (bombCount >= 3)
		{
			isActionEnd = 1;
			bombCount = 0;
		}
		break;
	case Enemy::Normal:
		if (bombTimer_.CountDown())
		{
			Vector3 bombSpd = (*playerTranslation_ - worldTransform_.translation_) / 102.0f;
			bombSpd.y = 1.5f;
			std::unique_ptr<Bomb> newBomb_ = std::make_unique<Bomb>();
			newBomb_->Initialize(model_, worldTransform_.translation_, bombSpd);
			newBomb_->SetScale({ 5.0f,5.0f,5.0f });
			bomb_.push_back(std::move(newBomb_));
			isActionEnd = 1;
		}
		break;
	case Enemy::Hard:
		if (!isStart)
		{
			bombTimer_ = 5;
			isStart = 1;
		}
		if (isStart)
		{
			if (bombTimer_.CountDown())
			{
				std::random_device seedGen;
				std::mt19937_64 engine(seedGen());
				std::uniform_real_distribution<float> bombFallPos(-74.0f, 74.0f);
				Vector3 bombSpd = { bombFallPos(engine),0, bombFallPos(engine) };
				bombSpd -= worldTransform_.translation_;
				bombSpd /= 102.0f;
				bombSpd.y = 1.5f;
				std::unique_ptr<Bomb> newBomb_ = std::make_unique<Bomb>();
				newBomb_->Initialize(model_, worldTransform_.translation_, bombSpd);
				bomb_.push_back(std::move(newBomb_));
				bombCount++;
			}
		}
		if (bombCount >= 30)
		{
			isActionEnd = 1;
			bombCount = 0;
		}
		break;
	}
}

void Enemy::Press()
{
	if (!isStart)
	{
		isStart = 1;
	}
	if (isStart)
	{
		worldTransform_.translation_.y += jumpSpd;
		jumpSpd -= 0.05f;

		if (worldTransform_.translation_.y <= 3.0f)
		{
			worldTransform_.translation_.y = 3.0f;
			isActionEnd = 1;
			jumpSpd = JUMP_SPD_INIT;
		}
	}
}

void Enemy::Tackle()
{
	if (!isStart)
	{
		tackleSpd = toPlayer_ * 1.5f;
		isStart = 1;
	}
	if (isStart)
	{
		worldTransform_.translation_ += tackleSpd;

		if (isOver(worldTransform_.translation_.x) || isOver(worldTransform_.translation_.z))
		{
			worldTransform_.translation_ -= tackleSpd;
			isActionEnd = 1;
		}
	}
}

void Enemy::Update()
{
	missiles_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->isDead_; });
	bomb_.remove_if([](std::unique_ptr<Bomb>& bomb) { return (bomb->isDead_ && !bomb->isExplosion); });

	toPlayer_ = *playerTranslation_ - worldTransform_.translation_;
	toPlayer_.y = 0;
	toPlayer_.normalize();

	(this->*pPhaseFuncTable[phase_])();
	if (isActionEnd)
	{
		phase_ = rand()%5;
		isStart = 0;
		isActionEnd = 0;
	}

	for (std::unique_ptr<EnemyBullet>& bullet : missiles_) { bullet->Update(); }
	for (std::unique_ptr<Bomb>& bomb : bomb_) { bomb->Update(); }

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 70);
	debugText_->Printf("EnemyHp:%d", hp_);
}

void Enemy::Draw()
{
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

	for (std::unique_ptr<EnemyBullet>& bullet : missiles_) { bullet->Draw(*viewProjection_); }
	for (std::unique_ptr<Bomb>& bomb : bomb_) { bomb->Draw(*viewProjection_); }
	beam_.Draw(*viewProjection_);
}

void Enemy::Clear()
{
	missiles_.clear();
}