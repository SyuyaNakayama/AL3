#include "Enemy.h"
#include <stdlib.h>
#include <DirectXMath.h>
#include <random>
#include "function.h"
#include "Collider/CollisionConfig.h"
#include "Player/Player.h"
#include <cassert>

using namespace std;

void Enemy::Initialize(Model* model, Vector3* playerTranslation,
	ViewProjection* viewProjection, bool* isPlayerMove, bool isHardMode)
{
	model_ = model;
	pressRippleModel_ = Model::CreateFromOBJ("Ripple", true);
	if (!isHardMode) { textureHandle_[0] = TextureManager::Load("picture/enemy.png"); }
	else { textureHandle_[0] = TextureManager::Load("picture/enemyhard.png"); }
	textureHandle_[1] = TextureManager::Load("picture/beam.png");
	debugText_ = DebugText::GetInstance();
	worldTransform_.translation_ = { 0, 3.0f, 0 };
	worldTransform_.scale_ = { 2.5f,5.0f,2.5f };
	worldTransform_.Initialize();
	rippleTransform_.scale_ = { 1.0f,2.0f,1.0f };
	rippleTransform_.Initialize();
	viewProjection_ = viewProjection;
	playerTranslation_ = playerTranslation;
	isPlayerMove_ = isPlayerMove;
	SetCollisionAttribute(CollisionAttribute::Enemy);
	hp_ = 200;
	isActionEnd = 1;
	isRippleExist = 0;
	counter_ = 0;
	idleTimer_ = 80;
	bindTimer = 60;
	state = State::Easy;
	preHp_ = hp_;
	audio_ = Audio::GetInstance();
	isHardMode_ = isHardMode;
	isTackle_ = 0;
	seHandle_.push_back(audio_->LoadWave("sound/SE/EnemyDamage.mp3"));
	seHandle_.push_back(audio_->LoadWave("sound/SE/Missile.mp3"));
	seHandle_.push_back(audio_->LoadWave("sound/SE/Press.mp3"));
	seHandle_.push_back(audio_->LoadWave("sound/SE/Tackle.mp3"));
	seHandle_.push_back(audio_->LoadWave("sound/SE/LaserIdle.mp3"));
	seHandle_.push_back(audio_->LoadWave("sound/SE/LaserShot.mp3"));
	seHandle_.push_back(audio_->LoadWave("sound/SE/Warp.mp3"));
	seHandle_.push_back(0);
	bgm_.push_back(audio_->LoadWave("sound/bgm/battlemusic1.mp3"));
	bgm_.push_back(audio_->LoadWave("sound/bgm/battlemusic2.mp3"));
	bgm_.push_back(audio_->LoadWave("sound/bgm/battlemusic3.mp3"));
}

void Enemy::BeamAction()
{
	static int phase = 0;
	Beam newBeam;
	switch (phase)
	{
	case 0:
		int	pattern;
		switch (state)
		{
		case Enemy::Easy:
			pattern = rand() % 2;
			for (size_t i = 0; i < 5; i++)
			{
				float position = 75.0f - 150.0f / 6.0f * (float)(i + 1);
				if (pattern == 0)
				{
					newBeam.SetScaleX();
					newBeam.SetPositionZ(position);
				}
				else
				{
					newBeam.SetScaleZ();
					newBeam.SetPositionX(position);
				}
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
		for (size_t i = 0; i < beam_.size(); i++) { beam_[i].Initialize(model_, &phase); }
		phase = 1;
		seHandle_[7] = audio_->PlayWave(seHandle_[4], true);
		actionTimer_ = 150;
	case 1:
		if (actionTimer_.CountDown())
		{
			phase = 2;
			audio_->StopWave(seHandle_[7]);
			seHandle_[7] = audio_->PlayWave(seHandle_[5], true);
		}
		break;
	case 2:
		if (state == State::Hard)
		{
			beam_[0].Update(-1.0f);
			beam_[1].Update(1.0f);
		}
		if (actionTimer_.CountDown())
		{
			isActionEnd = 1;
			phase = 0;
			beam_.clear();
			audio_->StopWave(seHandle_[7]);
		}
		break;
	}
}

void Enemy::Missile()
{
	bool isEnd = 0;
	if (!isStart)
	{
		switch (state)
		{
		case Enemy::Easy:
			actionTimer_ = 30;
			break;
		case Enemy::Normal:
			actionTimer_ = 40;
			break;
		case Enemy::Hard:
			actionTimer_ = 2;
			break;
		}
		isStart = 1;
	}

	if (actionTimer_.CountDown())
	{
		unique_ptr<EnemyBullet> newMissile = make_unique<EnemyBullet>();
		Vector3 velocity{};
		int offset;
		const int MISSILE_NUM = 12;
		switch (state)
		{
		case Enemy::Easy:
			toPlayer_ *= 2.0f;
			newMissile->Initialize(model_, worldTransform_.translation_, toPlayer_);
			missiles_.push_back(move(newMissile));
			break;
		case Enemy::Normal:
			offset = rand() % 360;
			for (size_t i = 0; i < MISSILE_NUM; i++)
			{
				velocity.x = sinf(DirectX::XM_2PI / (float)MISSILE_NUM * (float)i + DirectX::XMConvertToRadians(offset));
				velocity.z = cosf(DirectX::XM_2PI / (float)MISSILE_NUM * (float)i + DirectX::XMConvertToRadians(offset));
				velocity *= 2.0f;
				newMissile->Initialize(model_, worldTransform_.translation_, velocity);
				missiles_.push_back(move(newMissile));
				newMissile = make_unique<EnemyBullet>();
			}
			break;
		case Enemy::Hard:
			velocity.x = sinf(DirectX::XMConvertToRadians(counter_ * 12));
			velocity.z = cosf(DirectX::XMConvertToRadians(counter_ * 12));
			velocity *= 2.0f;
			newMissile->Initialize(model_, worldTransform_.translation_, velocity);
			missiles_.push_back(move(newMissile));
			break;
		}
		counter_++;
		audio_->PlayWave(seHandle_[1]);
	}

	switch (state)
	{
	case Enemy::Easy:
		isEnd = counter_ >= 8;
		break;
	case Enemy::Normal:
		isEnd = counter_ >= 5;
		break;
	case Enemy::Hard:
		isEnd = counter_ >= 150;
		break;
	}
	if (isEnd)
	{
		isActionEnd = 1;
		counter_ = 0;
	}
}

void Enemy::BombAction()
{
	bool isEnd = 0;

	if (!isStart)
	{
		switch (state)
		{
		case Enemy::Easy:
		case Enemy::Normal:
			actionTimer_ = 50;
			break;
		case Enemy::Hard:
			actionTimer_ = 3;
			break;
		}
		isStart = 1;
	}

	if (actionTimer_.CountDown())
	{
		unique_ptr<Bomb> newBomb_ = make_unique<Bomb>();
		Vector3 bombSpd{};
		switch (state)
		{
		case Enemy::Normal:
			newBomb_->SetScale({ 5.0f,5.0f,5.0f });
		case Enemy::Easy:
			bombSpd = (*playerTranslation_ - worldTransform_.translation_) / 102.0f;
			bombSpd.y = 1.5f;
			break;
		case Enemy::Hard:
			static mt19937_64 engine(seedGen());
			static uniform_real_distribution<float> bombFallPos(-74.0f, 74.0f);
			bombSpd = { bombFallPos(engine),0, bombFallPos(engine) };
			bombSpd = (bombSpd - worldTransform_.translation_) / 102.0f;
			bombSpd.y = 1.5f;
			break;
		}
		counter_++;
		newBomb_->Initialize(model_, worldTransform_.translation_, bombSpd);
		bomb_.push_back(move(newBomb_));
	}

	switch (state)
	{
	case Enemy::Easy:
	case Enemy::Normal:
		isEnd = counter_ >= 3;
		break;
	case Enemy::Hard:
		isEnd = counter_ >= 50;
		break;
	}
	if (isEnd)
	{
		isActionEnd = 1;
		counter_ = 0;
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
		audio_->PlayWave(seHandle_[2]);
	}
	if (!isRippleExist)
	{
		worldTransform_.translation_.y += jumpSpd;
		jumpSpd -= 0.05f;
		if (state == State::Hard) { worldTransform_.translation_ += pressSpd; }
		actionTimer_ = 60;
	}
	else
	{
		rippleTransform_.scale_ += { 1.5f, 0, 1.5f };
		rippleTransform_.UpdateMatrix();
		rippleTransform_.TransferMatrix();
		if (actionTimer_.CountDown())
		{
			rippleTransform_.scale_ = { 1.0f,2.0f,1.0f };
			isRippleExist = 0;
			isActionEnd = 1;
		}
	}
}

void Enemy::Tackle()
{
	static float posX;

	if (!isStart)
	{
		isStart = 1;
		posX = worldTransform_.translation_.x;
		actionTimer_ = 50;
	}
	if (!isTackle_)
	{
		if (actionTimer_.CountDown())
		{
			float tSpd = 0;
			switch (state)
			{
			case Enemy::Easy:
				tSpd = 1.5f;
				break;
			case Enemy::Normal:
				tSpd = 2.0f;
				break;
			case Enemy::Hard:
				tSpd = 3.0f;
				break;
			}
			tackleSpd = toPlayer_ * tSpd;
			audio_->PlayWave(seHandle_[3]);
			isTackle_ = 1;
		}
		if (actionTimer_.GetPassTime() <= 40)
		{
			worldTransform_.translation_.x = posX + sinf(DirectX::XM_2PI / 10.0f * actionTimer_.GetPassTime());
		}
		else { worldTransform_.translation_.x = posX; }
	}
	if (isTackle_)
	{
		worldTransform_.translation_ += tackleSpd;

		if (isOver(worldTransform_.translation_.x) || isOver(worldTransform_.translation_.z))
		{
			worldTransform_.translation_ -= tackleSpd;
			isActionEnd = 1;
			isTackle_ = 0;
		}
	}
}

void Enemy::Warp()
{
	static mt19937_64 engine(seedGen());
	static uniform_real_distribution<float> warpPos(-75.0f, 75.0f);
	worldTransform_.translation_ = { warpPos(engine), 3.0f, warpPos(engine) };
	audio_->PlayWave(seHandle_[6]);
	isActionEnd = 1;
}

void Enemy::Update()
{
	missiles_.remove_if([](unique_ptr<EnemyBullet>& bullet) { return bullet->isDead_; });
	bomb_.remove_if([](unique_ptr<Bomb>& bomb) { return bomb->phase_ == Bomb::Phase::Dead; });
	if (!*isPlayerMove_) { if (bindTimer.CountDown()) { *isPlayerMove_ = 1; } }

	toPlayer_ = *playerTranslation_ - worldTransform_.translation_;
	toPlayer_.y = 0;

	if (isActionEnd)
	{
		if (idleTimer_.CountDown())
		{
			StateChange();

			int nextPhase = 0;
			do
			{
				nextPhase = PhaseChange();
			} while (phase_ == nextPhase);

			phase_ = nextPhase;

			isStart = 0;
			isActionEnd = 0;
			if (isHardMode_) { idleTimer_ = 0; }
		}
	}
	toPlayer_.normalize();
	if (!isActionEnd) { (this->*pPhaseFuncTable[phase_])(); }

	for (unique_ptr<EnemyBullet>& bullet : missiles_) { bullet->Update(); }
	for (unique_ptr<Bomb>& bomb : bomb_) { bomb->Update(); }

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void Enemy::Draw()
{
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_[preHp_ != hp_]);
	for (unique_ptr<EnemyBullet>& bullet : missiles_) { bullet->Draw(*viewProjection_); }
	for (unique_ptr<Bomb>& bomb : bomb_) { bomb->Draw(*viewProjection_); }
	if (phase_ == Phase::beam) { for (size_t i = 0; i < beam_.size(); i++) { beam_[i].Draw(*viewProjection_); } }
	if (isRippleExist) { pressRippleModel_->Draw(rippleTransform_, *viewProjection_); }
	preHp_ = hp_;
}

void (Enemy::* Enemy::pPhaseFuncTable[])() =
{
	&Enemy::Missile,&Enemy::BombAction,&Enemy::Press,
	&Enemy::BeamAction,&Enemy::Tackle,&Enemy::Warp
};

void Enemy::StateChange()
{
	if (!isHardMode_)
	{
		if (hp_ < 133) { state = State::Normal; }
		if (hp_ < 67) { state = State::Hard; }
	}
	else
	{
		state = State::Hard;
	}
}

int Enemy::PhaseChange()
{
	// RATE_TABLE[‹——£][ó‘Ô][s“®]
	static const vector<vector<vector<int>>> RATE_TABLE =
	{
		{	// missile, bomb, press, beam, tackle, warp
			{10,20,30,15,10,15},
			{20,20,10,20,10,20}
		},
		{
			{15,20, 0,15,20,30},
			{15,15,15,10,20,25}
		}
	};

	bool dis = toPlayer_.length() >= 40.0f;
	int number = rand() % 100 + 1;

	int rateSum = 0;
	for (size_t i = 0; i < RATE_TABLE[dis][state == State::Hard].size(); i++)
	{
		rateSum += RATE_TABLE[dis][state == State::Hard][i];
	}
	if (rateSum != 100) { assert(0); return -1; }

	for (size_t i = 0; i < RATE_TABLE[dis][state == State::Hard].size(); i++)
	{
		number -= RATE_TABLE[dis][state == State::Hard][i];
		if (number <= 0) { return i; }
	}
}

void Enemy::Clear()
{
	missiles_.clear();
	bomb_.clear();
	beam_.clear();
}

void Enemy::StopAudio()
{
	audio_->StopWave(seHandle_[7]);
}