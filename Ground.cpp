#include "Ground.h"

void Ground::Initialize(Model* model)
{
	model_ = model;
	texture_ = TextureManager::Load("picture/ground.png");
	worldTransform_.Initialize();
	worldTransform_.translation_.y = -7.0f;
	worldTransform_.scale_ = { 75.0f,5.0f,75.0f };
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}