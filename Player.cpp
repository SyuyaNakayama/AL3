#include "Player.h"
#include <assert.h>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	worldTransform_.Initialize();
}
void Player::Update() {
	const float MOVE_SPD = 0.4f;
	const Vector2 MOVE_LIMIT = {34.0f, 18.0f};

	worldTransform_.translation_.x +=
	  (input_->PushKey(DIK_RIGHT) - input_->PushKey(DIK_LEFT)) * MOVE_SPD;
	worldTransform_.translation_.y +=
	  (input_->PushKey(DIK_UP) - input_->PushKey(DIK_DOWN)) * MOVE_SPD;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -MOVE_LIMIT.x);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +MOVE_LIMIT.x);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -MOVE_LIMIT.y);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +MOVE_LIMIT.y);

	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}
void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}