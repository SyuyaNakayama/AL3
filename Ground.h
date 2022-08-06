#pragma once
#include "WorldTransform.h"
#include <cstdint>
#include "Model.h"
#include "ViewProjection.h"
#include "Matrix4.h"

class Ground
{
private:
	WorldTransform worldTransform_;
	uint32_t texture_;
	Model* model_;
public:
	void Initialize(Model* model);
	void Draw(ViewProjection viewProjection_) { model_->Draw(worldTransform_, viewProjection_, texture_); }
};