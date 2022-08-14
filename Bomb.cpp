#include "Bomb.h"
#include <assert.h>
#include "Collider/CollisionConfig.h"

void Bomb::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	model_ = model;
	debugText_ = DebugText::GetInstance();
	textureHandle_[0] = TextureManager::Load("picture/bomb.png");
	textureHandle_[1] = TextureManager::Load("picture/explosion.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
	isDead_ = 0;
	isExplosion = 0;
	SetCollisionAttribute(CollisionAttribute::EnemyBomb);
}

void Bomb::Update()
{
	if (worldTransform_.translation_.y <= 0) { isDead_ = 1, isExplosion = 1; }
	if (isExplosion) { if (lifeTimer_.CountDown()) { isExplosion = 0; } }

	if (!isDead_)
	{
		worldTransform_.translation_ += velocity_;
		velocity_.y -= 0.03f;
	}
	else if (isExplosion)
	{
		worldTransform_.scale_ += {0.2f, 0.2f, 0.2f};
	}

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void Bomb::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_[isExplosion]);
}