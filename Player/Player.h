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
	Timer bulletInterval_;
	Vector2 angle_;

	void Move();
	void Rotate();
	void Jump();
	void Attack();
public:
	int hp_;
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	bool isMove = 1;

	void Initialize(Model* model, ViewProjection* viewProjection);
	void Update(Vector3 enemyTranslation);
	void Draw();
	void OnCollision() { hp_--; };
	const Vector3 GetWorldPosition() { return viewProjection_->eye; };
};