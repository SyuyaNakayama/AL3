#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

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

	for (size_t i = 0; i < _countof(worldTransforms_); i++)
	{
		worldTransforms_[i].Initialize();
	}
	worldTransforms_[0].scale_.y = 2.0f;

	worldTransforms_[1].parent_ = &worldTransforms_[0];
	worldTransforms_[1].scale_.y = 0.5f;
	worldTransforms_[1].translation_ = { 0,0.5f,2.0f };

	viewProjection_.Initialize();
}

void GameScene::Update() {
	const float MOVE_SPD = 0.5f;
	const float ROTA_SPD = 0.1f;
	const char MODE_NAME[2][14]
	{
		"BioWalk","CameraEyeMove"
	};

	if (input_->TriggerKey(DIK_Q))
	{
		if (++mode >= 2) { mode = 0; }
	}

	if (mode == 0)
	{
		worldTransforms_[0].rotation_.y += (input_->PushKey(DIK_D) - input_->PushKey(DIK_A)) * ROTA_SPD;
		worldTransforms_[0].translation_ += worldTransforms_[0].matWorld_.MatrixProduct(
			{ 0,0,(input_->PushKey(DIK_W) - input_->PushKey(DIK_S)) * MOVE_SPD });
	}
	if (mode == 1)
	{
		enum Axis { x, z };
		Vector3 cameraTargetAxis[2];

		cameraTargetAxis[Axis::z] = debugCamera_->GetViewProjection().target - debugCamera_->GetViewProjection().eye;
		cameraTargetAxis[Axis::z].normalize();
		cameraTargetAxis[Axis::x] = cameraTargetAxis[Axis::z].cross(debugCamera_->GetViewProjection().up);
		cameraTargetAxis[Axis::x].normalize();

		worldTransforms_[0].translation_ -= (input_->PushKey(DIK_D) - input_->PushKey(DIK_A)) * cameraTargetAxis[Axis::x] * MOVE_SPD;
		worldTransforms_[0].translation_ += (input_->PushKey(DIK_W) - input_->PushKey(DIK_S)) * cameraTargetAxis[Axis::z] * MOVE_SPD;
	}

	worldTransforms_[0].UpdateMatrix();
	worldTransforms_[0].TransferMatrix();
	worldTransforms_[1].UpdateMatrix();
	worldTransforms_[1].matWorld_ *= worldTransforms_[0].matWorld_;
	worldTransforms_[1].TransferMatrix();

	debugCamera_->Update();

	debugText_->SetPos(0, 0);
	debugText_->Printf("mode:%s", MODE_NAME[mode]);
	debugText_->SetPos(0, 20);
	debugText_->Printf("mouse:CameraMove");
	debugText_->SetPos(0, 40);
	debugText_->Printf("Q:ModeChange");
	debugText_->SetPos(0, 60);
	debugText_->Printf("ADWS:Move");
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
	model_->Draw(worldTransforms_[0], debugCamera_->GetViewProjection(), textureHandle_);
	model_->Draw(worldTransforms_[1], debugCamera_->GetViewProjection(), textureHandle_);
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
