#pragma once
#include <DirectXMath.h>
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
#include "RailCamera.h"

using namespace std;
// ゲームシーン
class GameScene
{
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
	list<unique_ptr<Enemy>> enemy_;
	unique_ptr<Skydome> skydome_ = nullptr;
	unique_ptr<RailCamera> railCamera = nullptr;
	list<unique_ptr<EnemyBullet>> enemyBullets_;
	uint32_t enemyBulletTexture_;
public: // メンバ関数
	~GameScene(); // デストラクタ
	void Initialize(); // 初期化
	void Update();     // 毎フレーム処理
	void CheckAllCollisions();
	list<unique_ptr<EnemyBullet>> GetEnemyBullets() { return enemyBullets_; }
	void AddEnemyBullet(unique_ptr<EnemyBullet> enemyBullet);
	void Draw();       // 描画
};
