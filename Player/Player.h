#pragma once
#include "DebugText.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Timer.h"
#include "Audio.h"
#include <Sprite.h>
#include <memory>
#include <list>

class Player :public Collider
{
private:
	Model* model_;
	Input* input_;
	DebugText* debugText_;
	ViewProjection* viewProjection_;
	Timer bulletInterval_;
	Vector2 angle_;
	Sprite* damageEffect_;
	Audio* audio_;
	std::vector<uint32_t> seHandle_;
	int preHp_ = hp_;

	void Move();
	void Rotate();
	void Jump();
	void Attack();
public:
	int hp_;
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	bool isMove = 1;

	void Initialize(Model* model, ViewProjection* viewProjection);
	void Update();
	void Draw();
	void DamageEffectDraw();
	void OnCollision() { hp_--; audio_->PlayWave(seHandle_[2], false, 0.5f); };
	const Vector3 GetWorldPosition() { return viewProjection_->eye; };
	const Vector3 GetRadius() { return { 1.25f,2.5f,1.25f }; }
};