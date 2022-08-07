#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>
#include <cstdint>

class Beam
{
private:
	std::vector<WorldTransform> worldTransforms_;
	Model* model_;
	uint32_t textureHandle_;
public:
	void Initialize(Model* model,int enemyState);
	void Update();
	void Draw(ViewProjection viewProjection);
	void Clear() { worldTransforms_.clear(); }
};