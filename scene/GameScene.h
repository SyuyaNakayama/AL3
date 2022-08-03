﻿#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Enemy/Enemy.h"
#include "Input.h"
#include "Model.h"
#include "Player/Player.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>
#include "Collider/CollisionManager.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public:         // メンバ関数
	~GameScene(); // デストラクタ

	void Initialize(); // 初期化
	void Update();     // 毎フレーム処理
	void Draw();       // 描画

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	Model* model_ = nullptr;
	Sprite* sprite_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;

	WorldTransform ground_;
	uint32_t groundPic_;
	// ゲームシーン用
	ViewProjection viewProjection_;
	Player* player_ = nullptr;
	std::unique_ptr<Enemy> enemy_ = nullptr;
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
};
