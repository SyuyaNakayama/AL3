#pragma once
#include "DebugText.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Enemy {
  private:
	enum class Phase { Approach, Leave };

	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	Vector3 velocity_;
	DebugText* debugText_;
	Phase phase_ = Phase::Approach;
	void Approach();
	void Leave();
	static void (Enemy::*pPhaseFuncTable[])();

  public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewprojection);
};
