#include "Skydome.h"
#include "ViewProjection.h"
#include <assert.h>

void Skydome::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 100,100,100 };
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}