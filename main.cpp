#include "Audio.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "TextureManager.h"
#include "WinApp.h"
#include <time.h>
#include <memory>

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText = nullptr;
	std::unique_ptr<GameScene> gameScene = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow("LE2A_16_ナカヤマ_シュウヤ_AL3_ンボスの侵略2");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);
#pragma region 汎用機能初期化
	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize();

	// 3Dモデル静的初期化
	Model::StaticInitialize();
#pragma endregion
	// ゲームシーンの初期化
	gameScene = std::make_unique<GameScene>();
	gameScene->Initialize();
	srand(time(NULL));

	// メインループ
	while (!win->ProcessMessage())
	{
		input->Update();		// 入力関連の毎フレーム処理
		gameScene->Update();	// ゲームシーンの毎フレーム処理
		dxCommon->PreDraw();	// 描画開始
		gameScene->Draw();		// ゲームシーンの描画
		dxCommon->PostDraw();	// 描画終了
	}

	audio->Finalize(); // 各種解放
	win->TerminateGameWindow(); // ゲームウィンドウの破棄

	return 0;
}