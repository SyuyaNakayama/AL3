﻿#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <string.h>

using namespace DirectX;

GameScene::GameScene()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	model_ = Model::Create();
	LoadResources();
	AxisIndicator::GetInstance()->SetVisible(1);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	reticle_ = Sprite::Create(TextureManager::Load("Picture/Reticle.png"), { 640,360 }, { 1,1,1,1 }, { 0.5f,0.5f });

	player_ = make_unique<Player>();
	enemy_ = make_unique<Enemy>();
	collisionManager_ = make_unique<CollisionManager>();
}

void GameScene::Initialize()
{
	viewProjection_.eye = { 0,0,-20 };
	viewProjection_.target = { 0,0,-19 };
	viewProjection_.Initialize();
	ground_.Initialize(model_);
	player_->Initialize(model_, &viewProjection_);
	enemy_->Initialize(model_, &viewProjection_.eye, &viewProjection_, &player_->isMove);
}

void GameScene::Update()
{
	switch (scene_)
	{
	case GameScene::Title:
		if (input_->TriggerKey(DIK_SPACE)) 
		{
			scene_ = GameScene::HowToPlay; 
			Initialize();
		}
		break;
	case GameScene::HowToPlay:
		if (input_->TriggerKey(DIK_SPACE))
		{
			scene_ = GameScene::Play;
			Initialize();
		}
		player_->Update();
		viewProjection_.UpdateMatrix();
		break;
	case GameScene::Play:
		player_->Update();
		enemy_->Update();
		collisionManager_->CheckAllCollisions(player_.get(), enemy_.get());
		viewProjection_.UpdateMatrix();

		if (player_->hp_ <= 0)
		{
			scene_ = Scene::GameOver;
			player_->bullets_.clear();
			enemy_->Clear();
		}
		if (enemy_->hp_ <= 0) { scene_ = Scene::GameClear; }

		break;
	case GameScene::GameClear:
		if (input_->TriggerKey(DIK_ESCAPE)) { scene_ = GameScene::Title; }
		break;
	case GameScene::GameOver:
		if (input_->TriggerKey(DIK_SPACE))
		{
			scene_ = GameScene::Play;
			Initialize();
		}
		if (input_->TriggerKey(DIK_ESCAPE)) { scene_ = GameScene::Title; }
		break;
	}
	debugText_->SetPos(50, 110);
	debugText_->PrintVector3("viewProjection_.eye", viewProjection_.eye);
	debugText_->SetPos(50, 130);
	debugText_->PrintVector3("viewProjection_.target", viewProjection_.target);
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// ここに背景スプライトの描画処理を追加できる
	//background_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion
#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// ここに3Dオブジェクトの描画処理を追加できる
	switch (scene_)
	{
	case GameScene::Play:
		enemy_->Draw();
	case GameScene::HowToPlay:
		player_->Draw();
		ground_.Draw(viewProjection_);
		break;
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// ここに前景スプライトの描画処理を追加できる
	if (themeSprite_[scene_]) { themeSprite_[scene_]->Draw(); }
	if (scene_ == Scene::Play || scene_ == Scene::HowToPlay) { reticle_->Draw(); }

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScene::LoadResources()
{
#pragma region スプライト読み込み
	background_ = Sprite::Create(TextureManager::Load("picture/BackGround.png"), {});
	background_->SetSize({ 1280.0f,720.0f });
	themeSprite_.push_back(Sprite::Create(TextureManager::Load("picture/Title.png"), {}));
	themeSprite_.push_back(nullptr);
	themeSprite_.push_back(nullptr);
	themeSprite_.push_back(Sprite::Create(TextureManager::Load("picture/GameClear.png"), {}));
	themeSprite_.push_back(Sprite::Create(TextureManager::Load("picture/GameOver.png"), {}));
	for (size_t i = 0; i < themeSprite_.size(); i++)
	{
		if (!themeSprite_[i])continue;
		themeSprite_[i]->SetSize({ 1280.0f,720.0f });
	}
#pragma endregion
#pragma region オーディオ読み込み
	//HANDLE hFind;
	//WIN32_FIND_DATAA findData;
	//string fileName;
	//string pass;
	//const char STR[2][2][22]
	//{
	//	{"Sound/BGM/","Resources/Sound/BGM/*"},
	//	{"Sound/SE/","Resources/Sound/SE/*"},
	//};
	//
	//for (size_t i = 0; i < 2; i++)
	//{
	//	fileName = STR[i][0];
	//	pass = STR[i][1];
	//	hFind = FindFirstFileA(pass.c_str(), &findData);
	//	if (hFind == INVALID_HANDLE_VALUE) { return; }
	//	for (; FindNextFileA(hFind, &findData);)
	//	{
	//		if (strstr(findData.cFileName, ".mp3") == nullptr) { continue; }
	//		if (i == 0) { bgm_.push_back(audio_->LoadWave(fileName + findData.cFileName)); }
	//		else{ se_.push_back(audio_->LoadWave(fileName + findData.cFileName)); }
	//	}
	//}
	//
	//FindClose(hFind);
#pragma endregion
}