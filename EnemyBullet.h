#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class EnemyBullet 
{
private:
	static const int LIFE_TIME = 60 * 5;

	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	Vector3 velocity_;
	int deathTimer_;
	bool isDead_;

public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	bool IsDead() const { return isDead_; }
	void OnCollision() { isDead_ = 1; };
	const Vector3 GetPosition() { return worldTransform_.translation_; }
};