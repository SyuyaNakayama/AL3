#include "EnemyBullet.h"
#include <assert.h>
#include "function.h"
#include "Collider/CollisionConfig.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("picture/missile.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_.x = 2.0f;
	velocity_ = velocity;
	SetCollisionAttribute(CollisionAttribute::Enemy);
}

void EnemyBullet::Update()
{
	worldTransform_.translation_ += velocity_;

	if (isOver(worldTransform_.translation_.x) ||
		isOver(worldTransform_.translation_.z))
	{
		isDead_ = 1;
	}

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
