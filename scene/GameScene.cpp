#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::~GameScene()
{
	delete model_;
	delete sprite_;
	delete debugCamera_;
	delete player_;
}

void GameScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	model_ = Model::Create();
	sprite_ = Sprite::Create(TextureManager::Load("picture/BackGround.png"), {});
	sprite_->SetSize({ 1280.0f,720.0f });
	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(1);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
	groundPic_ = TextureManager::Load("picture/ground.png");
	viewProjection_.eye.z = -20;
	viewProjection_.Initialize();
	ground_.Initialize();
	ground_.translation_.y = -7.0f;
	ground_.scale_ = { 75.0f,5.0f,75.0f };
	ground_.UpdateMatrix();
	ground_.TransferMatrix();
	player_ = new Player;
	player_->Initialize(model_, &viewProjection_);
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(model_, &viewProjection_.eye, &viewProjection_);
	collisionManager_ = std::make_unique<CollisionManager>();
}

void GameScene::Update()
{
	player_->Update(enemy_->worldTransform_.translation_);
	if (enemy_) { enemy_->Update(); }
	collisionManager_->CheckAllCollisions(player_,enemy_.get());
	debugCamera_->Update();
	viewProjection_.UpdateMatrix();
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// ここに背景スプライトの描画処理を追加できる
	//sprite_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// ここに3Dオブジェクトの描画処理を追加できる
	player_->Draw();
	if (enemy_) {
		enemy_->Draw();
	}
	model_->Draw(ground_, viewProjection_, groundPic_);
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