#include "Enemy.h"
#include <assert.h>
#include <stdlib.h>
#include <DirectXMath.h>
#include <random>
#include "function.h"
#include "Collider/CollisionConfig.h"
#include "Player/Player.h"

void Enemy::Initialize(Model* model, Vector3* playerTranslation, ViewProjection* viewProjection, bool* isPlayerMove)
{
	model_ = model;
	pressRippleModel_ = Model::CreateFromOBJ("Ripple", true);
	textureHandle_ = TextureManager::Load("picture/enemy.png");
	debugText_ = DebugText::GetInstance();
	worldTransform_.translation_ = { 0, 3.0f, 0 };
	worldTransform_.scale_ = { 2.5f,5.0f,2.5f };
	worldTransform_.Initialize();
	rippleTransform_.scale_ = { 1.0f,2.0f,1.0f };
	rippleTransform_.Initialize();
	viewProjection_ = viewProjection;
	playerTranslation_ = playerTranslation;
	isPlayerMove_ = isPlayerMove;
	SetCollisionAttribute(CollisionAttribute::CEnemy);
	SetCollisionMask(CollisionMask::CEnemyMask);
	hp_ = 500;
	state = State::Normal;
	phase_ = Phase::beam;
}

void Enemy::BeamAction()
{
	static bool isBeamObjectCreate = 0;
	if (state == State::Hard && isStart)
	{
		beam_[0].Update(-0.5f);
		beam_[1].Update(0.5f);
	}
	if (!isBeamObjectCreate)
	{
		Beam newBeam;
		switch (state)
		{
		case Enemy::Easy:
			for (size_t i = 0; i < 5; i++)
			{
				newBeam.SetScaleX();
				newBeam.SetPositionZ(75.0f - 150.0f / 6.0f * (float)(i + 1));
				beam_.push_back(newBeam);
			}
			break;
		case Enemy::Normal:
			for (size_t i = 0; i < 4; i++)
			{
				newBeam.SetScaleX();
				newBeam.SetPositionZ(75.0f - 150.0f / 5.0f * (float)(i + 1));
				beam_.push_back(newBeam);
			}
			for (size_t i = 0; i < 4; i++)
			{
				newBeam.SetScaleZ();
				newBeam.SetPositionX(75.0f - 150.0f / 5.0f * (float)(i + 1));
				beam_.push_back(newBeam);
			}
			break;
		case Enemy::Hard:
			for (size_t i = 0; i < 2; i++)
			{
				newBeam.SetScaleZ();
				newBeam.SetPositionX(75.0f * NewFlag(i == 0));
				beam_.push_back(newBeam);
			}
			break;
		}

		for (size_t i = 0; i < beam_.size(); i++) { beam_[i].Initialize(model_); }
		isBeamObjectCreate = 1;
	}
	if (beamTimer_.CountDown())
	{
		if (!isStart) { isStart = 1; }
		else
		{
			isActionEnd = 1;
			isBeamObjectCreate = 0;
			beam_.clear();
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
	Vector3 bombSpd;

	switch (state)
	{
	case Enemy::Easy:
	case Enemy::Normal:
		if (bombTimer_.CountDown())
		{
			bombSpd = (*playerTranslation_ - worldTransform_.translation_) / 102.0f;
			bombSpd.y = 1.5f;
			std::unique_ptr<Bomb> newBomb_ = std::make_unique<Bomb>();
			newBomb_->Initialize(model_, worldTransform_.translation_, bombSpd);
			if (state == State::Normal) { newBomb_->SetScale({ 5.0f,5.0f,5.0f }); }
			bomb_.push_back(std::move(newBomb_));
			bombCount++;
		}
		if (bombCount >= 3)
		{
			isActionEnd = 1;
			bombCount = 0;
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
				static std::random_device seedGen;
				static std::mt19937_64 engine(seedGen());
				static std::uniform_real_distribution<float> bombFallPos(-74.0f, 74.0f);
				bombSpd = { bombFallPos(engine),0, bombFallPos(engine) };
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
	static Vector3 pressSpd;
	if (state == State::Hard)
	{
		if (!isStart)
		{
			pressSpd = (*playerTranslation_ - worldTransform_.translation_) / 80.0f;
			pressSpd.y = 0;
			isStart = 1;
		}
	}
	if (worldTransform_.translation_.y < 3.0f)
	{
		worldTransform_.translation_.y = 3.0f;
		jumpSpd = JUMP_SPD_INIT;
		isRippleExist = 1;
		rippleTransform_.translation_ = worldTransform_.translation_;
		rippleTransform_.translation_.y = -2.0f;
		if (playerTranslation_->y == 0 && state != State::Easy) { *isPlayerMove_ = 0; }
	}
	if (!isRippleExist)
	{
		worldTransform_.translation_.y += jumpSpd;
		jumpSpd -= 0.05f;
		if (state == State::Hard) { worldTransform_.translation_ += pressSpd; }
	}
	else
	{
		rippleTransform_.scale_ += { 1.0f, 0, 1.0f };
		rippleTransform_.UpdateMatrix();
		rippleTransform_.TransferMatrix();
		if (rippleLifeTimer.CountDown())
		{
			rippleTransform_.scale_ = { 1.0f,2.0f,1.0f };
			isRippleExist = 0;
			isActionEnd = 1;
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

	if (!*isPlayerMove_)
	{
		static Timer bindTimer = 60;
		if (bindTimer.CountDown()) { *isPlayerMove_ = 1; }
	}

	//StateChange();

	toPlayer_ = *playerTranslation_ - worldTransform_.translation_;
	toPlayer_.y = 0;
	toPlayer_.normalize();

	(this->*pPhaseFuncTable[phase_])();
	if (isActionEnd)
	{
		phase_ = Phase::beam;
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
	if (phase_ == Phase::beam) { for (size_t i = 0; i < beam_.size(); i++) { beam_[i].Draw(*viewProjection_, isStart); } }
	if (isRippleExist) { pressRippleModel_->Draw(rippleTransform_, *viewProjection_); }
}

void (Enemy::* Enemy::pPhaseFuncTable[])() =
{
	&Enemy::BeamAction,&Enemy::Missile,&Enemy::BombAction,
	&Enemy::Press,&Enemy::Tackle
};

void Enemy::StateChange()
{
	if (hp_ < 300) { state = State::Normal; }
	if (hp_ < 100) { state = State::Hard; }
}

void Enemy::Clear()
{
	missiles_.clear();
}