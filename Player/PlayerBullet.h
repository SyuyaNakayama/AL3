#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Collider/Collider.h"
#include <cassert>

class PlayerBullet :public Collider {
private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	Vector3 velocity_;
public:
	bool isDead_ = 0;

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void OnCollision() { isDead_ = 1; };
	const Vector3 GetWorldPosition() { return worldTransform_.translation_; };
	const Vector3 GetRadius() { return worldTransform_.scale_; }
};