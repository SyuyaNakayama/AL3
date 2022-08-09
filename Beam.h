#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>
#include <cstdint>
#include "Collider/Collider.h"

class Beam :public Collider
{
private:
	Model* model_;
	uint32_t textureHandle_;
	int enemyState_;
public:
	std::vector<WorldTransform> worldTransforms_;
	void Initialize(Model* model, int enemyState);
	void Update();
	void Draw(ViewProjection viewProjection);
	void Clear() { worldTransforms_.clear(); }
	void OnCollision() {}
	const Vector3 GetRadius() { return {}; }
	const Vector3 GetWorldPosition() { return {}; }
};