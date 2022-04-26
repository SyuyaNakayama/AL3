#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

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
	// 乱数シード生成器
	std::random_device seed_gen;
	// メルセンヌ・ツイスタ
	std::mt19937_64 engine(seed_gen());
	// 乱数範囲
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	worldTransform_.Initialize();

	for (size_t i = 0; i < 3; i++) {
		viewProjection_[i].eye = {posDist(engine), posDist(engine), posDist(engine)};
		viewProjection_[i].Initialize();
	}
}

void GameScene::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		if (++viewNum == 3) {
			viewNum = 0;
		}
	}

	debugText_->SetPos(50, 50);
	debugText_->Printf("NowCamera:%d", viewNum);
	for (size_t i = 0; i < 3; i++) {
		debugText_->SetPos(50, 90 + 60 * i);
		debugText_->Printf("Camera%d", i + 1);
		debugText_->SetPos(50, 110 + 60 * i);
		debugText_->Printf(
		  "eye:(%f,%f,%f)", viewProjection_[i].eye.x, viewProjection_[i].eye.y,
		  viewProjection_[i].eye.z);
	}
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
	model_->Draw(worldTransform_, viewProjection_[viewNum], textureHandle_);
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
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}