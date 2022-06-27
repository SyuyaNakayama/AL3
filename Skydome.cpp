#include "Skydome.h"
#include "ViewProjection.h"
#include <assert.h>

void Skydome::Initialize(Model* model)
{
	assert(model);
	//model_ = model;
	worldTransform_.Initialize();
}

void Skydome::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}