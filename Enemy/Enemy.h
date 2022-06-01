#pragma once
#include "DebugText.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include "Timer.h"

class Enemy
{
private:
	Model* model_;
	uint32_t textureHandle_;
	Vector3 velocity_;
	DebugText* debugText_;
	std::list<std::unique_ptr<EnemyBullet>> missiles_;
	size_t phase_;
	Vector3* playerTranslation_;
	Timer attackTimer_;
	Vector3 toPlayer_;
	bool isActionEnd;
	Vector3 tackleSpd;

	void Beam();
	void Missile();
	void Bomb();
	void Press();
	void Tackle();
	void Summon();

	static void (Enemy::* pPhaseFuncTable[])();
public:
	WorldTransform worldTransform_;
	void Initialize(Model* model, Vector3* playerTranslation);
	void Update();
	void Draw(const ViewProjection& viewprojection);
};
