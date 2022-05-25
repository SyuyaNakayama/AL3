#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"

class Player {
  private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	Input* input_;
	DebugText* debugText_;
  public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection viewProjection);
};