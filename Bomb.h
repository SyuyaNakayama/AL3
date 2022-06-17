#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Bomb
{
private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	Vector3 velocity_;
public:
	bool isDead_ = 1;
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
};