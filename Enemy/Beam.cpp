#include "Beam.h"

void Beam::Initialize(Model* model, int enemyState)
{
	model_ = model;
	enemyState_ = enemyState;
	textureHandle_ = TextureManager::Load("Picture/beam.png");
	switch (enemyState_)
	{
	case 0:
		worldTransforms_.resize(5);
		for (size_t i = 0; i < worldTransforms_.size(); i++)
		{
			worldTransforms_[i].scale_.x = 75.0f;
			worldTransforms_[i].translation_.z = 75.0f - 150.0f / 6.0f * (float)(i + 1);
		}
		break;
	case 1:
		worldTransforms_.resize(8);
		for (size_t i = 0; i < worldTransforms_.size() / 2; i++)
		{
			worldTransforms_[i].scale_.x = 75.0f;
			worldTransforms_[i].translation_.z = 75.0f - 150.0f / 5.0f * (float)(i + 1);
		}
		for (size_t i = worldTransforms_.size() / 2; i < worldTransforms_.size(); i++)
		{
			worldTransforms_[i].scale_.z = 75.0f;
			worldTransforms_[i].translation_.x = 75.0f - 150.0f / 5.0f * (float)(i - 3);
		}
		break;
	case 2:
		worldTransforms_.resize(2);
		worldTransforms_[0].translation_.x = 75.0f;
		worldTransforms_[0].scale_.z = 75.0f;
		worldTransforms_[1].translation_.x = -75.0f;
		worldTransforms_[1].scale_.z = 75.0f;
		break;
	}

	for (size_t i = 0; i < worldTransforms_.size(); i++)
	{
		worldTransforms_[i].Initialize();
		worldTransforms_[i].UpdateMatrix();
		worldTransforms_[i].TransferMatrix();
	}
}

void Beam::Update()
{
	if (worldTransforms_.size() != 2) { return; }
	const float BEAM_SPD = 0.5f;
	worldTransforms_[0].translation_.x -= BEAM_SPD;
	worldTransforms_[1].translation_.x += BEAM_SPD;

	for (size_t i = 0; i < worldTransforms_.size(); i++)
	{
		worldTransforms_[i].UpdateMatrix();
		worldTransforms_[i].TransferMatrix();
	}
}

void Beam::Draw(ViewProjection viewProjection)
{
	for (size_t i = 0; i < worldTransforms_.size(); i++)
	{
		model_->Draw(worldTransforms_[i], viewProjection, textureHandle_);
	}
}