#include "RailCamera.h"

void RailCamera::Initialize(Vector3 position,Vector3 rotation)
{
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	debugText_ = DebugText::GetInstance();
}

void RailCamera::Update(Vector3 translSpd, Vector3 rotaSpd)
{
	worldTransform_.translation_ += translSpd;
	worldTransform_.rotation_ += rotaSpd;
	worldTransform_.UpdateMatrix();

	viewProjection_.eye = worldTransform_.translation_;
	Vector3 forward(0, 0, 1);
	forward = worldTransform_.matWorld_ * forward;
	viewProjection_.target = viewProjection_.eye + forward;
	Vector3 up(0, 1, 0);
	viewProjection_.up = worldTransform_.matWorld_ * up;
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	debugText_->SetPos(20, 20);
	debugText_->Printf("viewProjection_.eye:(%f,%f,%f)",
		viewProjection_.eye.x, viewProjection_.eye.y,
		viewProjection_.eye.z);
}