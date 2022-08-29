#include "GameScene.h"
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
	model_ = Model::Create();
	LoadResources();
	reticle_ = Sprite::Create(TextureManager::Load("Picture/Reticle.png"), { 640,360 }, { 1,1,1,1 }, { 0.5f,0.5f });

	player_ = make_unique<Player>();
	enemy_ = make_unique<Enemy>();
	collisionManager_ = make_unique<CollisionManager>();
}

void GameScene::Initialize()
{
	viewProjection_.eye = { 0,0,-50 };
	viewProjection_.target = { 0,0,-49 };
	viewProjection_.Initialize();
	ground_.Initialize(model_);
	player_->Initialize(model_, &viewProjection_);
	enemy_->Initialize(model_, &viewProjection_.eye, &viewProjection_, &player_->isMove, isHardMode);
	player_->Clear();
	enemy_->Clear();
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
			isHardMode = 0;
		}
		break;
	case GameScene::HowToPlay:
		if (input_->TriggerKey(DIK_SPACE))
		{
			scene_ = GameScene::Play;
			if (input_->PushKey(DIK_LSHIFT)) { isHardMode = 1; }
			Initialize();
			audio_->StopWave(playBGMHandle);
		}
		player_->Update();
		viewProjection_.UpdateMatrix();
		break;
	case GameScene::Play:
		if (input_->TriggerKey(DIK_RETURN)) { scene_ = Scene::Pause; }

		player_->Update();
		enemy_->Update();
		collisionManager_->CheckAllCollisions(player_.get(), enemy_.get());
		viewProjection_.UpdateMatrix();

		if (player_->hp_ <= 0)
		{
			scene_ = Scene::GameOver;
			player_->bullets_.clear();
			enemy_->Clear();
			enemy_->StopAudio();
		}
		if (enemy_->hp_ <= 0) { scene_ = Scene::GameClear; enemy_->StopAudio(); }

		hpGauge_[2]->SetSize({ player_->hp_ * 2.5f * 4.0f / 3.0f,64 });
		hpGauge_[3]->SetSize({ enemy_->hp_ * 2.5f,64 });
		break;
	case GameScene::Pause:
		if (input_->TriggerKey(DIK_RETURN)) { scene_ = Scene::Play; }
		break;
	case GameScene::GameClear:
		if (input_->TriggerKey(DIK_ESCAPE)) { scene_ = GameScene::Title; isGetLink = 0; }
		if (input_->TriggerKey(DIK_SPACE))
		{
			FILE* file;
			if (!isHardMode)
			{
				if (fopen_s(&file, "エンディングリンク.txt", "w") == 0)
				{
					fprintf(file, "クリアおめでとうございます。こちらがエンディングを見るためのリンクです。\n");
					fprintf(file, "検索エンジンで下記URLを貼り付けて下さい。\n\n");
					fprintf(file, "https://drive.google.com/file/d/1baJpTZh645Fu9sv8loQcIi_KjeRuPLEa/view?usp=sharing");
					fprintf(file, "\n\n追記:チュートリアルで左SHIFTキーを押しながらゲームを開始すると...？");
				}
			}
			else
			{
				if (fopen_s(&file, "真エンディングリンク.txt", "w") == 0)
				{
					fprintf(file, "ハードモードクリアおめでとうございます。ここまで遊んで頂きありがとうございます。\n");
					fprintf(file, "検索エンジンで下記URLを貼り付けて下さい。\n\n");
					fprintf(file, "https://drive.google.com/file/d/1dvZLAf-0L9Kxhf1NkmYfMowHyXQCt1Uc/view?usp=sharing");
					fprintf(file, "\n\nこれは真エンディングを見るためのリンクです。");
				}
			}
			if (file) { fclose(file); }
			isGetLink = 1;
		}
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
	AudioManage();
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// ここに背景スプライトの描画処理を追加できる
	background_->Draw();
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
	case GameScene::Pause:
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
	if (isGetLink) { themeSprite_[Scene::Pause + 2 + isHardMode]->Draw(); }
	player_->DamageEffectDraw();
	switch (scene_)
	{
	case Scene::Play: for (size_t i = 0; i < hpGauge_.size(); i++) { hpGauge_[i]->Draw(); }
	case Scene::HowToPlay: reticle_->Draw();
		break;
	case Scene::Pause: themeSprite_[Scene::Pause + 1]->Draw();
		break;
	}

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
	themeSprite_.push_back(Sprite::Create(TextureManager::Load("picture/HowToPlay.png"), {}));
	themeSprite_.push_back(nullptr);
	themeSprite_.push_back(Sprite::Create(TextureManager::Load("picture/GameClear.png"), {}));
	themeSprite_.push_back(Sprite::Create(TextureManager::Load("picture/GameOver.png"), {}));
	themeSprite_.push_back(Sprite::Create(TextureManager::Load("white1x1.png"), { 128,72 }, { 0,0,0,0.95f }));
	themeSprite_.push_back(Sprite::Create(TextureManager::Load("picture/pausemenu.png"), { 138,72 }));
	themeSprite_.push_back(Sprite::Create(TextureManager::Load("picture/GetLink.png"), {}));
	themeSprite_.push_back(Sprite::Create(TextureManager::Load("picture/GetLink2.png"), {}));
	for (size_t i = 0; i < themeSprite_.size(); i++)
	{
		if (!themeSprite_[i]) { continue; }
		if (i != Scene::HowToPlay) { themeSprite_[i]->SetSize({ 1280.0f,720.0f }); }
		if (i == Scene::Pause) { themeSprite_[i]->SetSize({ 1024.0f,576.0f }); }
	}
	themeSprite_[Scene::Pause + 1]->SetSize(Vector2(480, 240) * 2.1f);

	float leftSpace = 44;

	hpGauge_.push_back(Sprite::Create(TextureManager::Load("picture/player.png"), { leftSpace,20 }));
	hpGauge_.push_back(Sprite::Create(TextureManager::Load("picture/enemy.png"), { 640 + leftSpace,20 }));
	hpGauge_.push_back(Sprite::Create(TextureManager::Load("white1x1.png"), { leftSpace + 42,20 }, { 0,0,0.95f,1 }));
	hpGauge_.push_back(Sprite::Create(TextureManager::Load("white1x1.png"), { 640 + leftSpace + 42,20 }, { 0.95f,0,0,1 }));
	hpGauge_[0]->SetSize({ 32,64 });
	hpGauge_[1]->SetSize({ 32,64 });
#pragma endregion
#pragma region オーディオ読み込み
	bgm_.push_back(audio_->LoadWave("sound/bgm/title.mp3"));
	bgm_.push_back(audio_->LoadWave("sound/bgm/tutorial.mp3"));
	bgm_.push_back(audio_->LoadWave("sound/bgm/stageclear.mp3"));
#pragma endregion
}

void GameScene::AudioManage()
{
	static int preScene = -1;
	if (scene_ != preScene)
	{
		audio_->StopWave(playBGMHandle);
		switch (scene_)
		{
		case GameScene::Title:
			playBGMHandle = audio_->PlayWave(bgm_[0],true);
			break;
		case GameScene::HowToPlay:
			playBGMHandle = audio_->PlayWave(bgm_[1], true);
			break;
		case GameScene::GameClear:
			audio_->PlayWave(bgm_[2]);
			break;
		}
	}

	preScene = scene_;
}