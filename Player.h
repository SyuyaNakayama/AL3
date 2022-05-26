#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "WorldTransform.h"

class Player {
  private:
	Model* model_;
	uint32_t textureHandle_;
	Input* input_;
	DebugText* debugText_;
	WorldTransform worldTransform_;
	PlayerBullet* bullet_;

	void Move();
	void Rotate();
	void Attack();

  public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection viewProjection);
};