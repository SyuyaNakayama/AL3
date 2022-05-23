#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
using namespace DirectX;

const XMFLOAT3 operator+(const XMFLOAT3& v1, const XMFLOAT3& v2) { return v1 + v2; }
const XMFLOAT3 operator-(const XMFLOAT3& v1, const XMFLOAT3& v2) {
	XMFLOAT3 temp(v1);
	return temp -= v2;
}
const XMFLOAT3 operator*(const XMFLOAT3& v, float s) {
	XMFLOAT3 temp(v);
	return temp *= s;
}
const XMFLOAT3 operator*(float s, const XMFLOAT3& v) { return v * s; }
const XMFLOAT3 operator/(const XMFLOAT3& v, float s) { return v / s; }

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();

	targetNum = 0;
	vecToNext = {0, 0, 0};

	for (size_t i = 0; i < 3; i++) {
		worldTransform_[i].translation_ = {
		  5.0f * cosf(XM_2PI / 3.0f * (float)i + XM_PI / 2.0f),
		  5.0f * sinf(XM_2PI / 3.0f * (float)i + XM_PI / 2.0f), 0.0f};
		worldTransform_[i].Initialize();
	}

	viewProjection_.eye.z = -25;
	viewProjection_.target = {0, 0, 0};
	viewProjection_.Initialize();
}

void GameScene::Update() {
	static bool isMove = 0;
	static XMFLOAT3 direction;
	float spd = 0.4f;

	if (input_->TriggerKey(DIK_SPACE)) {
		isMove = 1;
		if (++targetNum == 3) {
			targetNum = 0;
		}
		direction = worldTransform_[targetNum].translation_ - viewProjection_.target;
		direction.Norm();
	}
	if (isMove) {
		XMFLOAT3 temp = worldTransform_[targetNum].translation_ - viewProjection_.target;
		XMFLOAT3 temp2 = direction * spd;
		float length = temp.Length();
		if (length < temp2.Length()) {
			viewProjection_.target = worldTransform_[targetNum].translation_;
		} else {
			viewProjection_.target += direction * spd;
		}
	}

	viewProjection_.UpdateMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
	  viewProjection_.target.z);
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
	for (size_t i = 0; i < 3; i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	}
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