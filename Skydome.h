#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Skydome
{
private:
	WorldTransform worldTransform_;
	Model* model_;

public:
	void Initialize(Model* model);
	void Draw(ViewProjection viewProjection);
};