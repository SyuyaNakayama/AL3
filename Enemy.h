#pragma once
#include "DebugText.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Enemy {
  private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	Vector3 velocity_;
	DebugText* debugText_;

  public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewprojection);
};