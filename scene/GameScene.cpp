#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();

	// キャラクターの大元
	worldTransform_[PartId::Root].Initialize();
	// 脊椎
	worldTransform_[PartId::Spine].parent_ = &worldTransform_[PartId::Root];
	worldTransform_[PartId::Spine].Initialize();
	//上半身
	worldTransform_[PartId::Chest].parent_ = &worldTransform_[PartId::Spine];
	worldTransform_[PartId::Chest].Initialize();

	worldTransform_[PartId::Head].translation_ = {0, 3.0f, 0};
	worldTransform_[PartId::Head].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::Head].Initialize();

	worldTransform_[PartId::ArmL].translation_ = {-3.0f, 0, 0};
	worldTransform_[PartId::ArmL].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::ArmL].Initialize();

	worldTransform_[PartId::ArmR].translation_ = {3.0f, 0, 0};
	worldTransform_[PartId::ArmR].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::ArmR].Initialize();
	// 下半身
	worldTransform_[PartId::Hip].translation_ = {0, -3.0f, 0};
	worldTransform_[PartId::Hip].parent_ = &worldTransform_[PartId::Spine];
	worldTransform_[PartId::Hip].Initialize();

	worldTransform_[PartId::LegL].translation_ = {-3.0f, -3.0f, 0};
	worldTransform_[PartId::LegL].parent_ = &worldTransform_[PartId::Hip];
	worldTransform_[PartId::LegL].Initialize();

	worldTransform_[PartId::LegR].translation_ = {3.0f, -3.0f, 0};
	worldTransform_[PartId::LegR].parent_ = &worldTransform_[PartId::Hip];
	worldTransform_[PartId::LegR].Initialize();

	viewProjection_.eye = {0, 0, -25};
	viewProjection_.Initialize();
}

void GameScene::Update() {
	const float CHARACTER_SPD = 0.2f;
	const float CHARACTER_ROT_SPD = 0.05f;

	// 移動
	worldTransform_[PartId::Root].translation_.x +=
	  (input_->PushKey(DIK_RIGHT) - input_->PushKey(DIK_LEFT)) * CHARACTER_SPD;
	// 上半身の回転
	worldTransform_[PartId::Chest].rotation_.y +=
	  (input_->PushKey(DIK_I) - input_->PushKey(DIK_U) + input_->PushKey(DIK_A) -
	   input_->PushKey(DIK_D)) *
	  CHARACTER_ROT_SPD;
	// 下半身の回転
	worldTransform_[PartId::Hip].rotation_.y += (input_->PushKey(DIK_K) - input_->PushKey(DIK_J) +
	                                             input_->PushKey(DIK_A) - input_->PushKey(DIK_D)) *
	                                            CHARACTER_ROT_SPD;

	// Armの回転
	worldTransform_[PartId::ArmL].rotation_.x -= CHARACTER_ROT_SPD * 2;
	worldTransform_[PartId::ArmR].rotation_.x += CHARACTER_ROT_SPD * 2;
	// Legの回転
	worldTransform_[PartId::LegL].rotation_.x += CHARACTER_ROT_SPD * 2;
	worldTransform_[PartId::LegR].rotation_.x -= CHARACTER_ROT_SPD * 2;

	for (size_t i = 0; i < 9; i++) {
		worldTransform_[i].UpdateMatrix();
	}

	debugText_->SetPos(50, 100);
	debugText_->Printf(
	  "Root:(%f,%f,%f)", worldTransform_[PartId::Root].translation_.x,
	  worldTransform_[PartId::Root].translation_.y, worldTransform_[PartId::Root].translation_.z);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// ここに背景スプライトの描画処理を追加できる

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// ここに3Dオブジェクトの描画処理を追加できる
	for (size_t i = PartId::Chest; i < 9; i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// ここに前景スプライトの描画処理を追加できる

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}