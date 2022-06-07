﻿#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "DebugCamera.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	
	~GameScene();	// デストラクタ
	
	void Initialize();	// 初期化
	void Update();		// 毎フレーム処理
	void Draw();		// 描画

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	Model* model_ =nullptr;
	DebugCamera* debugCamera_ = nullptr;

	uint32_t textureHandle_ = 0;
	// ゲームシーン用
	WorldTransform worldTransforms_[2];
	ViewProjection viewProjection_;
};
