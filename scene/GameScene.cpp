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
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	model_ = Model::Create();
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
}

void GameScene::CheckAllCollisions()
{
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

	std::list<Collider*> colliders_;
	colliders_.push_back(player_);
	colliders_.push_back(enemy_.get());
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		colliders_.push_back(bullet.get());
	}
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		colliders_.push_back(bullet.get());
	}

	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		Collider* colliderA = *itrA;
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
			Collider* colliderB = *itrB;
			CheckCollisionPair(*itrA, *itrB);
		}

	}
}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{
	if (!(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) ||
		!(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()))
	{
		return;
	}

	Vector3 colliderAPos = colliderA->GetWorldPosition(),
		colliderBPos = colliderB->GetWorldPosition();

	Vector3 vecAtoB = colliderAPos - colliderBPos;
	float disAtoB = vecAtoB.length();

	if (disAtoB <= colliderA->GetRadius() + colliderB->GetRadius())
	{
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
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
