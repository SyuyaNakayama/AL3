#include "Player.h"
#include <assert.h>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	worldTransform_.Initialize();
	worldTransform_.scale_.y = 2.0f;
}

void Player::Move() {
	const float MOVE_SPD = 0.4f;
	const Vector2 MOVE_LIMIT = {34.0f, 17.0f};

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

void Player::Attack() {
	if (!input_->TriggerKey(DIK_SPACE)) {
		return;
	}

	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_);

	bullets_.push_back(std::move(newBullet));
}

void Player::Update() {
	Move();
	Rotate();
	Attack();

	for (std::unique_ptr<PlayerBullet>& bullet:bullets_) {
		bullet->Update();
	}

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}