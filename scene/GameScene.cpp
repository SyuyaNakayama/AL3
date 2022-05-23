#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete sprite_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	model_ = Model::Create();
	textureHandle_ = TextureManager::Load("mario.jpg");
	reticlePic_ = TextureManager::Load("reticle.png");
	sprite_ = Sprite::Create(reticlePic_, {640-64, 360-64});

	// 乱数シード生成器
	std::random_device seed_gen;
	// メルセンヌ・ツイスタ
	std::mt19937_64 engine(seed_gen());
	// 乱数範囲
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	for (size_t i = 0; i < _countof(worldTransform_); i++) {
		worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};
		worldTransform_[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};
		worldTransform_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};
		worldTransform_[i].Initialize();
	}
	viewProjection_.fovAngleY = XMConvertToRadians(40.0f);
	viewProjection_.Initialize();

	reticle = 0;
}

void GameScene::Update() {
	const float MOVE_TARGRT_SPD = 0.4f;
	    viewProjection_.target.x +=
	  (input_->PushKey(DIK_RIGHT) - input_->PushKey(DIK_LEFT)) * MOVE_TARGRT_SPD;
	viewProjection_.target.y +=
	  (input_->PushKey(DIK_UP) - input_->PushKey(DIK_DOWN)) * MOVE_TARGRT_SPD;

	if (input_->TriggerKey(DIK_SPACE)) {
		reticle = !reticle;
	}

	if (reticle) {
		viewProjection_.fovAngleY = XMConvertToRadians(20.0f);
	} else {
		viewProjection_.fovAngleY = XMConvertToRadians(40.0f);
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

	for (int i = 0; i < _countof(worldTransform_); i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// ここに前景スプライトの描画処理を追加できる
	if (reticle) {
		sprite_->Draw();
	}
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}