#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

const float XM_PI = 3.1415926f;
const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2) {
	Matrix4 temp = m1;
	temp *= m2;
	return temp;
}

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();
	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(1);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
	worldTransforms_[0].Initialize();

	worldTransforms_[1].Initialize();
	worldTransforms_[1].translation_ = {0, 4.5f, 0};
	worldTransforms_[1].parent_ = &worldTransforms_[0];

	viewProjection_.Initialize();
}

void GameScene::Update() {
	const float MOVE_SPD = 0.2f;

	worldTransforms_[0].translation_.x +=
	  (input_->PushKey(DIK_RIGHT) - input_->PushKey(DIK_LEFT)) * MOVE_SPD;

	worldTransforms_[0].UpdateMatrix();
	worldTransforms_[0].TransferMatrix();

	worldTransforms_[1].UpdateMatrix();
	worldTransforms_[1].matWorld_ *= worldTransforms_[0].matWorld_;
	worldTransforms_[1].TransferMatrix();
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
	/// </summary>
	model_->Draw(worldTransforms_[0], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[1], viewProjection_, textureHandle_);
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
