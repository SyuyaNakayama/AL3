#pragma once
#include "DebugText.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include "Timer.h"

class Player
{
private:
	const float PI = 3.141592654f;

	Model* model_;
	Input* input_;
	DebugText* debugText_;
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	ViewProjection* viewProjection_;
	int hp;
	Timer bulletInterval_;
	float angle_;

	void Move();
	void Rotate();
	void Jump();
	void Attack();
public:
	void Initialize(Model* model, ViewProjection* viewProjection);
	void Update(Vector3 enemyTranslation);
	void Draw();
};