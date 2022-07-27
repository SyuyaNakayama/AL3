#include "Player.h"
#include <assert.h>

const Vector3 DivWMul(Vector3 vec, Matrix4 mat)
{
	vec = mat * vec;
	vec /= mat.GetW();
	return vec;
}

void Player::Initialize(Model* model) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("picture/player.png");
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	worldTransform_.Initialize();
	worldTransform_.scale_.y = 2.0f;
	worldTransform_.translation_.z = 50.0f;
	worldTransform3DReticle_.Initialize();
	uint32_t textureReticle = TextureManager::Load("Picture/Reticle.png");
	sprite2DReticle_.reset(Sprite::Create(textureReticle, { 640,360 }, { 0,0,0,1.0f }, { 0.5f,0.5f }));
}

void Player::Move() {
	const float MOVE_SPD = 0.55f;
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
	Vector3 velocity = worldTransform3DReticle_.translation_ -
		worldTransform_.matWorld_.GetTranslationFromMatrix();
	velocity = velocity.normalize() * BULLET_SPD;

	unique_ptr<PlayerBullet> newBullet = make_unique<PlayerBullet>();
	newBullet->Initialize(model_, worldTransform_.matWorld_.GetTranslationFromMatrix(), velocity);

	bullets_.push_back(move(newBullet));
}

void Player::Update(RailCamera railCamera, const ViewProjection& viewProjection) {
	bullets_.remove_if([](unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	worldTransform_.UpdateMatrix();
	worldTransform_.matWorld_ *= railCamera.GetWorldTransform().matWorld_;
	worldTransform_.TransferMatrix();

	const float DIS_PLAYER_TO_3D_RETICLE = 50.0f;
	Vector3 offset = { 0,0,1.0f };
	offset = worldTransform_.matWorld_ * offset;
	offset = offset.normalize() * DIS_PLAYER_TO_3D_RETICLE;
	worldTransform3DReticle_.translation_ = offset + worldTransform_.matWorld_.GetTranslationFromMatrix();
	worldTransform3DReticle_.UpdateMatrix();
	worldTransform3DReticle_.TransferMatrix();

	Move();
	//Rotate();
	Attack(railCamera);

	for (unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Update(); }

	Vector3 positionReticle = worldTransform3DReticle_.translation_;
	Matrix4 matViewPort;
	matViewPort.GetViewPort({ 1280, 720 }, { 640,360 });
	Matrix4 matViewProjectionViewPort;
	matViewProjectionViewPort.SetScaleMat({ 1,1,1 });
	matViewProjectionViewPort *= viewProjection.matView;
	matViewProjectionViewPort *= viewProjection.matProjection;
	matViewProjectionViewPort *= matViewPort;
	positionReticle = DivWMul(positionReticle, matViewProjectionViewPort);
	sprite2DReticle_->SetPosition({ positionReticle.x,positionReticle.y });
}

void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Draw(viewProjection); }
	model_->Draw(worldTransform3DReticle_, viewProjection);
}

void Player::DrawUI()
{
	sprite2DReticle_->Draw();
}