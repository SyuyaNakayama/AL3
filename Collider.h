#pragma once
#include "Vector3.h"

class Collider
{
private:
	float radius_ = 1.0f;
	uint32_t collisionAttribute_
public:
	float GetRadius() { return radius_; }
	void SetRadius(float radius) { radius_ = radius; }
	virtual void OnCollision() = 0;
	const virtual Vector3 GetWorldPosition() = 0;
};