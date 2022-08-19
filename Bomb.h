#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Collider/Collider.h"
#include "Timer.h"
#include "Audio.h"

class Bomb :public Collider
{
private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_[2];
	Vector3 velocity_;
	DebugText* debugText_;
	Timer lifeTimer_ = 20;
	Audio* audio_;
	uint32_t seHandle_;
public:
	bool isDead_ = 1, isExplosion = 0;
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void SetScale(Vector3 scale) { worldTransform_.scale_ = scale; }
	void Draw(const ViewProjection& viewProjection);
	void OnCollision() { if (!isDead_) { isDead_ = 1; isExplosion = 1; } }
	const Vector3 GetRadius() { return worldTransform_.scale_; }
	const Vector3 GetWorldPosition() { return worldTransform_.translation_; }
};