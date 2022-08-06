#include "Beam.h"

void Beam::Initialize(Model* model, int enemyState)
{
	model_ = model;
	textureHandle_ = TextureManager::Load("Picture/beam.png");
	switch (enemyState)
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
		worldTransforms_.resize(4);
		worldTransforms_[0].translation_.x = 75.0f;
		worldTransforms_[0].scale_.z = 75.0f;
		worldTransforms_[1].translation_.x = -75.0f;
		worldTransforms_[1].scale_.z = 75.0f;
		worldTransforms_[2].translation_.z = 75.0f;
		worldTransforms_[2].scale_.x = 75.0f;
		worldTransforms_[3].translation_.z = -75.0f;
		worldTransforms_[3].scale_.x = 75.0f;
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
	if (worldTransforms_.size() != 4 || worldTransforms_[0].translation_.x <= -75.0f) { return; }
	const float BEAM_SPD = 0.5f;
	worldTransforms_[0].translation_.x -= BEAM_SPD;
	worldTransforms_[1].translation_.x += BEAM_SPD;
	worldTransforms_[2].translation_.z -= BEAM_SPD;
	worldTransforms_[3].translation_.z += BEAM_SPD;

	for (size_t i = 0; i < 4; i++)
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