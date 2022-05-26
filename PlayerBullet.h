#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class PlayerBullet {
  private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
  public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewprojection);
};