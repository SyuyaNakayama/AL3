#include "Player.h"
#include <assert.h>
#include "function.h"

void Player::Initialize(Model* model, ViewProjection* viewProjection)
{
	assert(model);
	model_ = model;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	viewProjection_ = viewProjection;
	bulletInterval_ = 40;
}

void Player::Move()
{
	const float MOVE_SPD = 0.6f;
	Vector3 spd(0, 0, 0);
	Vector3 yAxis(0, 1.0f, 0);

	if (input_->PushKey(DIK_LEFT)) { spd += toEnemy_.cross(yAxis); }
	if (input_->PushKey(DIK_RIGHT)) { spd -= toEnemy_.cross(yAxis); }
	if (input_->PushKey(DIK_UP)) { spd += toEnemy_; }
	if (input_->PushKey(DIK_DOWN)) { spd -= toEnemy_; }

	spd.normalize();
	spd *= MOVE_SPD;
	viewProjection_->eye += spd;

	Clamp(viewProjection_->eye.x);
	Clamp(viewProjection_->eye.z);
}

void Player::Jump()
{
	static bool isJump = 0;
	if (!isJump && input_->TriggerKey(DIK_SPACE)) { isJump = 1; }
	if (isJump)
	{
		const float JUMP_HEIGHT_MAX = 0.85f;
		static float jumpSpd = JUMP_HEIGHT_MAX;
		viewProjection_->eye.y += jumpSpd;
		viewProjection_->target.y += jumpSpd;
		jumpSpd -= 0.07f;
		if (viewProjection_->eye.y < 0)
		{
			viewProjection_->eye.y = 0;
			viewProjection_->target.y = 0;
			jumpSpd = JUMP_HEIGHT_MAX;
			isJump = 0;
		}
	}
}

void Player::Attack()
{
	if (!bulletInterval_.CountDown()) { return; }

	const float BULLET_SPD = 1.0f;
	Vector3 velocity = toEnemy_ * BULLET_SPD;

	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	newBullet->Initialize(model_, viewProjection_->eye, velocity);

	bullets_.push_back(std::move(newBullet));
}

void Player::Update(Vector3 enemyTranslation)
{
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });
	toEnemy_ = enemyTranslation - viewProjection_->eye;
	toEnemy_.y = 0;
	toEnemy_.normalize();
	viewProjection_->target = enemyTranslation;

	Move();
	Jump();
	Attack();

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Update(); }

	debugText_->SetPos(50, 50);
	debugText_->Printf("(%f,%f,%f)", viewProjection_->eye.x, viewProjection_->eye.y, viewProjection_->eye.z);
}

void Player::Draw() {
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(*viewProjection_);
	}
}