#include "Player.h"
#include <assert.h>
#include "function.h"
#include "Collider/CollisionConfig.h"

void Player::Initialize(Model* model, ViewProjection* viewProjection)
{
	assert(model);
	model_ = model;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	viewProjection_ = viewProjection;
	bulletInterval_ = 40;
	angle_ = PI / 2.0f;
	SetCollisionAttribute(CollisionAttribute::CPlayer);
	SetRadius(5.0f);
	SetCollisionMask(CollisionMask::CPlayerMask);
	hp_ = 200;
}

void Player::Move()
{
	enum Spd { front, side, all };
	const float MOVE_SPD = 0.6f;
	Vector3 spd[3];
	Vector3 yAxis(0, 1.0f, 0);

	spd[front] = viewProjection_->target - viewProjection_->eye;
	spd[front].normalize();
	spd[side] = spd[front].cross({ 0,1.0f,0 });
	spd[side].normalize();

	spd[front] *= input_->PushKey(DIK_UP) - input_->PushKey(DIK_DOWN);
	spd[side] *= input_->PushKey(DIK_LEFT) - input_->PushKey(DIK_RIGHT);

	spd[all] = spd[front] + spd[side];
	spd[all].normalize();
	spd[all] *= MOVE_SPD;

	viewProjection_->eye += spd[all];
	viewProjection_->target += spd[all];

	Clamp(viewProjection_->eye.x);
	Clamp(viewProjection_->eye.z);
}

void Player::Rotate()
{
	static const float ROT_SPD = 0.04f;

	angle_ += (input_->PushKey(DIK_A) - input_->PushKey(DIK_D)) * ROT_SPD;
	viewProjection_->target =
	{
		viewProjection_->eye.x + cosf(angle_),
		viewProjection_->eye.y,
		viewProjection_->eye.z + sinf(angle_),
	};
}

void Player::Jump()
{
	static bool isJump = 0;
	if (!isJump && input_->TriggerKey(DIK_W)) { isJump = 1; }
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
	Vector3 velocity = Vector3{ cosf(angle_),0,sinf(angle_) } *BULLET_SPD;

	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	newBullet->Initialize(model_, viewProjection_->eye, velocity);

	bullets_.push_back(std::move(newBullet));
}

void Player::Update(Vector3 enemyTranslation)
{
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->isDead_; });

	Rotate();
	Move();
	Jump();
	Attack();

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Update(); }
	debugText_->SetPos(50, 50);
	debugText_->Printf("%ld", input_->GetMouseMove().lZ);
}

void Player::Draw()
{
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Draw(*viewProjection_); }
}