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

	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			worldTransforms_[y][x].Initialize();
			worldTransforms_[y][x].translation_ = {
			  -16.0f + 4.0f * (float)x, 16.0f - 4.0f * (float)y, 0};
			worldTransforms_[y][x].UpdateMatrix();
		}
	}

	viewProjection_.Initialize();
}

void GameScene::Update() {
	const float MOVE_TARGRT_SPD = 0.2f;
	const float MOVE_FOV_ANGLE_Y_SPD = 0.01f;

	viewProjection_.target.x += (input_->PushKey(DIK_D) - input_->PushKey(DIK_A)) * MOVE_TARGRT_SPD;
	viewProjection_.target.y += (input_->PushKey(DIK_W) - input_->PushKey(DIK_S)) * MOVE_TARGRT_SPD;

	viewProjection_.fovAngleY +=
	  (input_->PushKey(DIK_DOWN) - input_->PushKey(DIK_UP)) * MOVE_FOV_ANGLE_Y_SPD;

	if (viewProjection_.fovAngleY >= XM_PI) {
		viewProjection_.fovAngleY = XM_PI;
	}
	if (viewProjection_.fovAngleY <= 0.01f) {
		viewProjection_.fovAngleY = 0.01f;
	}

	viewProjection_.UpdateMatrix();

	debugText_->SetPos(20, 20);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(20, 40);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
	  viewProjection_.target.z);

	debugText_->SetPos(20, 60);
	debugText_->Printf(
	  "up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

	debugText_->SetPos(20, 80);
	debugText_->Printf("fovAngleY(Degree):%f", XMConvertToDegrees(viewProjection_.fovAngleY));

	debugText_->SetPos(20, 100);
	debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
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

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			model_->Draw(worldTransforms_[y][x], viewProjection_, textureHandle_);
		}
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