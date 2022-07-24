#pragma once
#include "DebugText.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"

class Player;
class GameScene;

class Enemy
{
private:
	enum class Phase { Approach, Leave };

	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	Vector3 velocity_;
	DebugText* debugText_;
	Phase phase_ = Phase::Approach;
	static const int FIRE_INTERVAL = 60;
	int fireTimer;
	Player* player_;
	GameScene* gameScene_;
	bool isDead = 0;

	void Approach();
	void ApproachInit();
	void Leave();
	void Fire();
public:
	void Initialize(Model* model, const Vector3& position,Player* player);
	void Update();
	void Draw(const ViewProjection& viewprojection);
	const Vector3 GetPosition() { return worldTransform_.translation_; }
	void OnCollision() { isDead = 1; }
	bool IsDead() { return isDead; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
};