#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2) {
	Matrix4 temp = m1;
	temp *= m2;
	return temp;
}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	delete player_;
	delete modelSkydome_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	model_ = Model::Create();
	//modelSkydome_ = Model::CreateFromOBJ("skydome", 1);
	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(1);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
	viewProjection_.Initialize();
	player_ = new Player;
	player_->Initialize(model_);
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(model_, { 10.0f, 0, 50.0f });
	enemy_->SetPlayer(player_);
	//skydome_->Initialize(model_);
}

void GameScene::CheckAllCollisions()
{
	Vector3 posA, posB;

	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();
#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetPosition();

	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		posB = bullet->GetPosition();
		posB -= posA;
		if (posB.length() <= 3.0f)
		{
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion
#pragma region 自弾と敵キャラの当たり判定
	posA = enemy_->GetPosition();

	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		posB = bullet->GetPosition();
		posB -= posA;
		if (posB.length() <= 3.0f)
		{
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion
#pragma region 自弾と敵弾の当たり判定
	for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets) {
		for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets)
		{
			posA = playerBullet->GetPosition();
			posB = enemyBullet->GetPosition();
			posB -= posA;
			if (posB.length() <= 2.0f)
			{
				playerBullet->OnCollision();
				enemyBullet->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::Update() {
	player_->Update();
	if (enemy_) { enemy_->Update(); }
	CheckAllCollisions();
	debugCamera_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	player_->Draw(viewProjection_);
	if (enemy_) { enemy_->Draw(viewProjection_); }
	//skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
