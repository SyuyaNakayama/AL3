#pragma once
#include "DebugText.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include "Collider.h"

class Player :public Collider {
private:
	Model* model_;
	uint32_t textureHandle_;
	Input* input_;
	DebugText* debugText_;
	WorldTransform worldTransform_;
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	void Move();
	void Rotate();
	void Attack();

public:
	void Initialize(Model* model);
	void Update();
	void Draw(ViewProjection viewProjection);
	const Vector3 GetWorldPosition()override { return worldTransform_.translation_; };
	void OnCollision()override {};
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
};