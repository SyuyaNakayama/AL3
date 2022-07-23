#include "Player.h"
#include <assert.h>

void Player::Initialize(Model* model) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("picture/player.png");
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	worldTransform_.Initialize();
	worldTransform_.scale_.y = 2.0f;
	worldTransform_.translation_.z = 50.0f;
}

void Player::Move() {
	const float MOVE_SPD = 0.4f;
	const Vector2 MOVE_LIMIT = { 34.0f, 17.0f };

	worldTransform_.translation_.x +=
		(input_->PushKey(DIK_RIGHT) - input_->PushKey(DIK_LEFT)) * MOVE_SPD;
	worldTransform_.translation_.y +=
		(input_->PushKey(DIK_UP) - input_->PushKey(DIK_DOWN)) * MOVE_SPD;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -MOVE_LIMIT.x);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +MOVE_LIMIT.x);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -MOVE_LIMIT.y);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +MOVE_LIMIT.y);
}

void Player::Rotate() {
	const float ROT_SPD = 0.03f;

	worldTransform_.rotation_.y += (input_->PushKey(DIK_A) - input_->PushKey(DIK_D)) * ROT_SPD;
}

void Player::Attack(RailCamera railCamera) {
	if (!input_->TriggerKey(DIK_SPACE)) { return; }

	const float BULLET_SPD = 1.0f;
	Vector3 velocity(0, 0, BULLET_SPD);

	velocity = worldTransform_.matWorld_ * velocity;

	unique_ptr<PlayerBullet> newBullet = make_unique<PlayerBullet>();
	newBullet->Initialize(model_, worldTransform_.matWorld_.GetTranslationFromMatrix(), velocity);

	bullets_.push_back(move(newBullet));
}

void Player::Update(RailCamera railCamera) {
	bullets_.remove_if([](unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	Move();
	Rotate();
	Attack(railCamera);

	for (unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Update(); }

	worldTransform_.UpdateMatrix();
	worldTransform_.matWorld_ *= railCamera.GetWorldTransform().matWorld_;
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 350);
	debugText_->Printf(
		"Player:(%f,%f,%f)", worldTransform_.matWorld_.GetTranslationFromMatrix().x,
		worldTransform_.matWorld_.GetTranslationFromMatrix().y,
		worldTransform_.matWorld_.GetTranslationFromMatrix().z);
}

void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Draw(viewProjection); }
}