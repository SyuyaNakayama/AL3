#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DebugText.h"

class RailCamera
{
private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	DebugText* debugText_;
public:
	void Initialize(Vector3 position, Vector3 rotation);
	void Update(Vector3 transSpd, Vector3 rotaSpd);
	WorldTransform GetWorldTransform() { return worldTransform_; }
	ViewProjection GetViewProjection() { return viewProjection_; }
};