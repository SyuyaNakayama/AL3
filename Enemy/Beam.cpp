#include "Beam.h"
#include "Collider/CollisionConfig.h"

void Beam::Initialize(Model* model, int* phase)
{
	model_ = model;
	textureHandle_[0] = TextureManager::Load("Picture/beam2.png");
	textureHandle_[1] = TextureManager::Load("Picture/beam.png");
	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
	SetCollisionAttribute(CollisionAttribute::EnemyBeam);
	phase_ = phase;
}

void Beam::Update(float spd)
{
	worldTransform_.translation_.x += spd;
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void Beam::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_[*phase_ == 2]);
}