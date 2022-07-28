#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Collider.h"

class PlayerBullet :public Collider {
private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	Vector3 velocity_;
	static const int LIFE_TIME = 60 * 5;
	int deathTimer_;
	bool isDead_;
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	bool IsDead() const { return isDead_; }
	void OnCollision()override { isDead_ = 1; };
	const Vector3 GetWorldPosition()override { return worldTransform_.translation_; }
};