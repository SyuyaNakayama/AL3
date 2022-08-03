#pragma once
#include "DebugText.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include "Timer.h"
#include "Bomb.h"

class Enemy :public Collider
{
private:
	enum Phase { beam, missile, bomb, press, tackle, summon };

	Model* model_;
	uint32_t textureHandle_;
	Vector3 velocity_;
	DebugText* debugText_;
	size_t phase_ = Phase::bomb;
	Vector3* playerTranslation_;
	ViewProjection* viewProjection_;
	Timer attackTimer_ = 20;
	Vector3 toPlayer_;
	bool isActionEnd = 0, isStart = 0;
	Vector3 tackleSpd{};
	int hp_ = 500;

	void Beam();
	void Missile();
	void BombAction();
	void Press();
	void Tackle();
	void Summon();

	static void (Enemy::* pPhaseFuncTable[])();
public:
	std::list<std::unique_ptr<EnemyBullet>> missiles_;
	std::unique_ptr<Bomb> bomb_;

	WorldTransform worldTransform_;
	void Initialize(Model* model, Vector3* playerTranslation, ViewProjection* viewProjection);
	void Update();
	void Draw();
	void OnCollision() { hp_--; }
	const Vector3 GetWorldPosition() { return worldTransform_.translation_; }
};