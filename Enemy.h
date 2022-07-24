#pragma once
#include "DebugText.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"

class Player;
class GameScene;

class Enemy {
private:
	enum class Phase { Approach, Leave };

	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	Vector3 velocity_;
	DebugText* debugText_;
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	Phase phase_ = Phase::Approach;
	static const int FIRE_INTERVAL = 60;
	int fireTimer;
	Player* player_;
	GameScene* gameScene_;

	void Approach();
	void ApproachInit();
	void Leave();

	void Fire();
public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewprojection);
	void SetPlayer(Player* player) { player_ = player; }
	const Vector3 GetPosition() { return worldTransform_.translation_; }
	void OnCollision() {};
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
};
