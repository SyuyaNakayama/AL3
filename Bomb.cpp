#include "Bomb.h"
#include <assert.h>
#include "Collider/CollisionConfig.h"

void Bomb::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	model_ = model;
	textureHandle_[0] = TextureManager::Load("picture/bomb.png");
	textureHandle_[1] = TextureManager::Load("picture/explosion.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
	SetCollisionAttribute(CollisionAttribute::EnemyBomb);
	audio_ = Audio::GetInstance();
	seHandle_ = audio_->LoadWave("sound/SE/Explosion.mp3");
	phase_ = Phase::Alive;
}

void Bomb::Update()
{
	switch (phase_)
	{
	case Bomb::Alive:
		worldTransform_.translation_ += velocity_;
		velocity_.y -= 0.03f;

		if (worldTransform_.translation_.y <= 0)
		{
			phase_ = Phase::Explosion;
			//audio_->PlayWave(seHandle_);
		}
		break;
	case Bomb::Explosion:
		worldTransform_.scale_ += {0.2f, 0.2f, 0.2f};
		if (lifeTimer_.CountDown()) { phase_ = Phase::Dead; }
		break;
	}

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void Bomb::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_[phase_ == Phase::Explosion]);
}