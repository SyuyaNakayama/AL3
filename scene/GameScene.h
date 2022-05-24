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

/// ゲームシーン
class GameScene {
  public:              // メンバ関数
	GameScene();       // コンストクラタ
	~GameScene();      // デストラクタ
	void Initialize(); // 初期化
	void Update();     // 毎フレーム処理
	void Draw();       // 描画

	enum PartId {
		Root,  // 大元
		Spine, // 脊椎
		Chest, // 胸
		Head,  // 頭
		ArmL,  // 左腕
		ArmR,  // 右腕
		Hip,   // 尻
		LegL,  // 左足
		LegR   // 右足
	};

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;
	// ゲームシーン用
	WorldTransform worldTransform_[10];
	ViewProjection viewProjection_;
	float tipRotAngle;
	float tipRotSpd;
};
