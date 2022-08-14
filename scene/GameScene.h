#pragma once

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
#include "Collider/CollisionManager.h"
#include "Ground.h"
#include <DirectXMath.h>
#include <memory>
#include <vector>

using namespace std;

/// ゲームシーン
class GameScene
{
public:
	GameScene();
	void Initialize();
	void Update();
	void Draw();
	void LoadResources();

	enum Scene { Title, HowToPlay, Play, GameClear, GameOver };
private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	Model* model_ = nullptr;
	Sprite* background_ = nullptr;
	vector<Sprite*> themeSprite_;
	vector<uint32_t> bgm_;
	vector<uint32_t> se_;

	// ゲームシーン用
	Scene scene_ = Scene::Title;
	ViewProjection viewProjection_;
	unique_ptr<Player> player_ = nullptr;
	unique_ptr<Enemy> enemy_ = nullptr;
	unique_ptr<CollisionManager> collisionManager_ = nullptr;
	Ground ground_;
	Sprite* reticle_;
};