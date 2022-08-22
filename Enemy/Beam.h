#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>
#include <cstdint>
#include "Collider/Collider.h"

class Beam :public Collider
{
private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_[2];
public:
	int* phase_;

	void Initialize(Model* model,int* phase);
	void Update(float spd);
	void Draw(ViewProjection viewProjection);
	void OnCollision() {}
	void SetScaleX() { worldTransform_.scale_ = { 75.0f,1,1 }; }
	void SetScaleZ() { worldTransform_.scale_ = { 1,1,75.0f }; }
	void SetPositionX(float posX) { worldTransform_.translation_ = { posX,0,0 }; }
	void SetPositionZ(float posZ) { worldTransform_.translation_ = { 0,0,posZ }; }
	const Vector3 GetRadius() { return worldTransform_.scale_; }
	const Vector3 GetWorldPosition() { return worldTransform_.translation_; }
};