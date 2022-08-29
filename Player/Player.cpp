#include "Player.h"
#include <assert.h>
#include "function.h"
#include "Collider/CollisionConfig.h"
#include <DirectXMath.h>

void Player::Initialize(Model* model, ViewProjection* viewProjection)
{
	model_ = model;
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	viewProjection_ = viewProjection;
	bulletInterval_ = 15;
	SetCollisionAttribute(CollisionAttribute::Player);
	SetCollisionMask(CollisionMask::Player);
	hp_ = 150; preHp_ = hp_; angle_ = {}; isMove = 1;
	damageEffect_ = Sprite::Create(TextureManager::Load("Picture/Beam.png"), {});
	damageEffect_->SetSize({ 1280,720 });
	cursolMode = 0;
	seHandle_.push_back(audio_->LoadWave("sound/SE/PlayerShot.mp3"));
	seHandle_.push_back(audio_->LoadWave("sound/SE/PlayerJump.mp3"));
	seHandle_.push_back(audio_->LoadWave("sound/SE/PlayerDamage.mp3"));
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

	spd[front] *= input_->PushKey(DIK_W) - input_->PushKey(DIK_S);
	spd[side] *= input_->PushKey(DIK_A) - input_->PushKey(DIK_D);

	spd[all] = spd[front] + spd[side];
	spd[all].y = 0;
	spd[all].normalize();
	spd[all] *= MOVE_SPD;

	viewProjection_->eye += spd[all];
	viewProjection_->target += spd[all];

	Clamp(viewProjection_->eye.x);
	Clamp(viewProjection_->eye.z);
}

void Player::Rotate()
{
	static int sensitivity = 6;
	for (size_t i = 0; i < 9; i++)
	{
		if (input_->TriggerKey(DIK_1 + i)) { sensitivity = i; }
	}
	int rotateSpd = (9 - sensitivity) * 2;
	angle_.y += DirectX::XMConvertToRadians(input_->GetMouseMove().lX / rotateSpd);
	angle_.x -= DirectX::XMConvertToRadians(input_->GetMouseMove().lY / rotateSpd);
	Clamp(angle_.x, 1, -0.5f);
	viewProjection_->target =
	{
		viewProjection_->eye.x + sinf(angle_.y),
		viewProjection_->eye.y + angle_.x,
		viewProjection_->eye.z + cosf(angle_.y)
	};
}

void Player::Jump()
{
	static bool isJump = 0;
	if (!isJump)
	{
		if (input_->IsTriggerMouse(0))
		{
			isJump = 1;
			audio_->PlayWave(seHandle_[1]);
		}
	}
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
			jumpSpd = JUMP_HEIGHT_MAX;
			isJump = 0;
		}
	}
}

void Player::Attack()
{
	if (!bulletInterval_.CountDown()) { return; }

	const float BULLET_SPD = 1.5f;
	Vector3 velocity = (viewProjection_->target - viewProjection_->eye) * BULLET_SPD;

	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	newBullet->Initialize(model_, viewProjection_->target, velocity);

	bullets_.push_back(std::move(newBullet));
	audio_->PlayWave(seHandle_[0],false,0.25f);
}

void Player::Update()
{
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->isDead_; });

	if (isMove)
	{
		Move();
		Jump();
		Rotate();
		Attack();
	}
	
	if (input_->IsTriggerMouse(1)) { cursolMode = !cursolMode; }
	if (cursolMode) { SetCursorPos(760, 480); }

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Update(); }
}

void Player::Draw()
{
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Draw(*viewProjection_); }
}

void Player::DamageEffectDraw()
{
	if (preHp_ != hp_) { damageEffect_->Draw(); }
	preHp_ = hp_;
}