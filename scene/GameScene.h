#pragma once

#include "Audio.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>

using namespace DirectX;

const XMFLOAT3 operator+(const XMFLOAT3& v1, const XMFLOAT3& v2);
const XMFLOAT3 operator-(const XMFLOAT3& v1, const XMFLOAT3& v2);
const XMFLOAT3 operator*(const XMFLOAT3& v, float s);
const XMFLOAT3 operator*(float s, const XMFLOAT3& v);
const XMFLOAT3 operator/(const XMFLOAT3& v, float s);

/// ゲームシーン
class GameScene {
  public:              // メンバ関数
	GameScene();       // コンストクラタ
	~GameScene();      // デストラクタ
	void Initialize(); // 初期化
	void Update();     // 毎フレーム処理
	void Draw();       // 描画

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;
	// ゲームシーン用
	WorldTransform worldTransform_[3];
	ViewProjection viewProjection_;

	int targetNum;
	XMFLOAT3 vecToNext;
};
