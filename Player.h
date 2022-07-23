#pragma once
#include "DebugText.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include "RailCamera.h"

using namespace std;

class Player {
private:
	Model* model_;
	uint32_t textureHandle_;
	Input* input_;
	DebugText* debugText_;
	WorldTransform worldTransform_;
	list<unique_ptr<PlayerBullet>> bullets_;

	void Move();
	void Rotate();
	void Attack(RailCamera railCamera);
public:
	void Initialize(Model* model);
	void Update(RailCamera railCamera);
	void Draw(ViewProjection viewProjection);
	Vector3 GetPosition(){ return worldTransform_.matWorld_.GetTranslationFromMatrix(); };
	void OnCollision() {};
	const list<unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
};