﻿#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Skydome.h"
#include <DirectXMath.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	~GameScene(); // デストラクタ

	void Initialize(); // 初期化
	void Update();     // 毎フレーム処理
	void Draw();       // 描画
	void CheckAllCollisions();
private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	ViewProjection viewProjection_;
	DebugCamera* debugCamera_ = nullptr;
	Player* player_ = nullptr;
	uint32_t playerPic;
	std::unique_ptr<Enemy> enemy_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;
};
