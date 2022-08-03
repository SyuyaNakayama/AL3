#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Collider/Collider.h"

class EnemyBullet :public Collider
{
private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	Vector3 velocity_;

public:
	bool isDead_ = 0;
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();

	void OnCollision() { isDead_ = 1; };
	const Vector3 GetWorldPosition() { return worldTransform_.translation_; };
	void Draw(const ViewProjection& viewProjection);
};