#pragma once
#include "DebugText.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include "Timer.h"

class Player :public Collider
{
private:
	const float PI = 3.141592654f;

	Model* model_;
	Input* input_;
	DebugText* debugText_;
	ViewProjection* viewProjection_;
	int hp_ = 200;
	Timer bulletInterval_;
	float angle_;

	void Move();
	void Rotate();
	void Jump();
	void Attack();
public:
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	void Initialize(Model* model, ViewProjection* viewProjection);
	void Update(Vector3 enemyTranslation);
	void Draw();
	void OnCollision() { hp_--; };
	const Vector3 GetWorldPosition() { return viewProjection_->eye; };
};