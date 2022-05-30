#pragma once
#include "DebugText.h"
#include "EnemyBullet.h"
#include "Model.h"
#include "TimedCall.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <list>
#include <memory>

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
	std::list<std::unique_ptr<TimedCall>> timedCalls_;

	void Approach();
	void ApproachInit();
	void Leave();
	void Fire();
	void CallFire();

  public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewprojection);
};
