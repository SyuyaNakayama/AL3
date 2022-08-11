#pragma once
#include "DebugText.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include "Timer.h"
#include "Bomb.h"
#include "Beam.h"

class Enemy :public Collider
{
private:
	enum Phase { beam, missile, bomb, press, tackle };
	enum State { Easy, Normal, Hard };

	Model* model_;
	Model* pressRippleModel_;
	uint32_t textureHandle_;
	DebugText* debugText_;
	size_t phase_ = Phase::bomb;
	Vector3 velocity_, toPlayer_;
	Vector3* playerTranslation_;
	Vector3 tackleSpd{};
	ViewProjection* viewProjection_;
	Timer missileInterval_ = 40;
	Timer beamTimer_ = 300;
	bool isActionEnd = 0;
	const float JUMP_SPD_INIT = 2.0f;
	float jumpSpd = JUMP_SPD_INIT;
	State state = State::Normal;
	Timer bombTimer_ = 75;
	Timer idleTimer_ = 100;
	Timer rippleLifeTimer = 50;
	bool* isPlayerMove_;

	void BeamAction(), Missile();
	void BombAction();
	void Press(), Tackle();
	static void (Enemy::* pPhaseFuncTable[])();
	void StateChange();
public:
	int hp_;
	WorldTransform worldTransform_;
	std::list<std::unique_ptr<EnemyBullet>> missiles_;
	std::list<std::unique_ptr<Bomb>> bomb_;
	std::vector<Beam> beam_;
	WorldTransform rippleTransform_;
	bool isRippleExist = 0;
	bool isStart = 0;

	void Initialize(Model* model, Vector3* playerTranslation, ViewProjection* viewProjection,bool* isPlayerMove);
	void Update(), Draw();
	void Clear();
	void OnCollision() { hp_--; }
	const Vector3 GetRadius() { return worldTransform_.scale_; }
	const Vector3 GetWorldPosition() { return worldTransform_.translation_; }
};