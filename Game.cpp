//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

///
#include "DebugObj\DebugFont.h"
#include "Scene\GameContext.h"
#include "Scene\GameStateManager.h"

#include "Scene\TitleState.h"
#include "Scene\PlayState.h"
#include "Scene/ResultState.h"
#include "Scene\PauseState.h"

#include "Aspect.h"
#include "SE・BGM/acb/CueSheet_0.h"
///

extern void ExitGame();

using Microsoft::WRL::ComPtr;

LPCWSTR Game::WINDOW_NAME = L"ゲームの状態管理 [2a](スタック方式)";

Game::Game() noexcept(false)
{
	m_pDeviceResources = std::make_unique<DX::DeviceResources>();
	m_pDeviceResources->RegisterDeviceNotify(this);
	//登録（シングルトンパターン化）
	GameContext<DX::DeviceResources>::Register(m_pDeviceResources);
}

Game::~Game()
{
	DebugFont* debugFont = DebugFont::GetInstance();
	debugFont->reset();


	m_pCharacterMapList->Reset();

	m_pSound->Finalize();
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(const HWND &window, int width, int height)
{
	// マウスの作成
	m_pMouse = std::make_unique<DirectX::Mouse>();
	m_pMouse->SetWindow(window);

	m_pDeviceResources->SetWindow(window, width, height);

	m_pDeviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_pDeviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	auto device = m_pDeviceResources->GetD3DDevice();

	// コモンステート作成
	m_pState = std::make_unique<DirectX::CommonStates>(device);
	//登録（シングルトンパターン化）
	GameContext<DirectX::CommonStates>::Register(m_pState);

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/

	DebugFont* debugFont = DebugFont::GetInstance();
	debugFont->create(device, m_pDeviceResources->GetD3DDeviceContext());

	//各シーンの設定
	m_pGameStateManager = std::make_unique<GameStateManager>();
	m_pGameStateManager->RegisterState<TitleState>("Title");
	m_pGameStateManager->RegisterState<PlayState>("Play");
	m_pGameStateManager->RegisterState<ResultState>("Result");
	m_pGameStateManager->RegisterState<PauseState>("Pause");
	m_pGameStateManager->SetStartState("Title");
	//登録（シングルトンパターン化）
	GameContext<GameStateManager>::Register(m_pGameStateManager);

	//フェード処理の設定
	m_pShutter = std::make_unique<Shutter>();
	//エフェクトの作成
	m_pShutter->Create();
	//各データの初期化
	m_pShutter->Initialize();
	//登録（シングルトンパターン化）
	GameContext<Shutter>::Register(m_pShutter);

	//スコアの作成
	m_pScore = std::make_unique<Score>();
	//画像等の作成
	m_pScore->Create(device);
	//登録（シングルトンパターン化）
	GameContext<Score>::Register(m_pScore);

	//データ管理クラスの作成
	m_pDataManager = std::make_unique<DataManager>();
	//データの初期化
	m_pDataManager->Initialize();
	//登録（シングルトンパターン化）
	GameContext<DataManager>::Register(m_pDataManager);

	//マップクラスの作成
	m_pMap = std::make_unique<Map>();
	//モデル、エフェクトの作成
	m_pMap->Create();
	//登録（シングルトンパターン化）
	GameContext<Map>::Register(m_pMap);

	//キャラクターのリストの作成
	m_pCharacterMapList= std::make_unique<CharacterMapList>();
	//登録（シングルトンパターン化）
	GameContext<CharacterMapList>::Register(m_pCharacterMapList);

	//サウンドクラスの作成
	m_pSound = std::make_unique<ADX2>();
	//サウンドの登録
	m_pSound->Initialize("SE・BGM/acf/SOUND.acf", "SE・BGM/acb/CueSheet_0.acb");
	//音量設定
	m_pSound->SetVolume(1.0f);
	//登録（シングルトンパターン化）
	GameContext<ADX2>::Register(m_pSound);

	//スプライトバッチの設定
	m_pSpriteBatch = std::make_unique<DirectX::SpriteBatch>(m_pDeviceResources->GetD3DDeviceContext());
	//登録（シングルトンパターン化）
	GameContext<DirectX::SpriteBatch>::Register(m_pSpriteBatch);
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

	Render();
}

// Updates the world.
void Game::Update(const DX::StepTimer &timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());

	// TODO: Add your game logic here.
	elapsedTime;

	//シーンマネージャーの更新処理
	m_pGameStateManager->Update(timer);
	//音再生の更新処理
	GameContext<ADX2>::Get()->Update();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	Clear();

	m_pDeviceResources->PIXBeginEvent(L"Render");
	auto context = m_pDeviceResources->GetD3DDeviceContext();

	// TODO: Add your rendering code here.
	context;

	m_pGameStateManager->Render();

	m_pDeviceResources->PIXEndEvent();

	// Show the new frame.
	m_pDeviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
	m_pDeviceResources->PIXBeginEvent(L"Clear");

	// Clear the views.
	auto context = m_pDeviceResources->GetD3DDeviceContext();
	auto renderTarget = m_pDeviceResources->GetRenderTargetView();
	auto depthStencil = m_pDeviceResources->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	// Set the viewport.
	auto viewport = m_pDeviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	m_pDeviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
	// TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
	auto r = m_pDeviceResources->GetOutputSize();
	m_pDeviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnWindowSizeChanged(int width, int height)
{
	if (!m_pDeviceResources->WindowSizeChanged(width, height))
		return;

	CreateWindowSizeDependentResources();

	// TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	//現在の画面サイズ
	width = Aspect::ASPECT_X;
	height = Aspect::ASPECT_Y;
	//最大の画面サイズ
	//width = 1920;
	//height = 1080;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
	auto device = m_pDeviceResources->GetD3DDevice();
	auto context = m_pDeviceResources->GetD3DDeviceContext();

	// TODO: Initialize device dependent objects here (independent of window size).
	device;
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
	// TODO: Initialize windows-size dependent objects here.

	// ウインドウサイズからアスペクト比を算出する
	RECT size = m_pDeviceResources->GetOutputSize();
	float aspectRatio = float(size.right) / float(size.bottom);

	// 画角を設定
	float fovAngleY = DirectX::XMConvertToRadians(45.0f);

	// 射影行列を作成する
	m_pProjection = std::make_unique<Projection>();
	m_pProjection->SetPerspectiveFieldOfView(
		fovAngleY,
		aspectRatio,
		0.01f,
		1000.0f
	);
	GameContext<Projection>::Register(m_pProjection);
}

void Game::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.

//	m_pPlayer->Reset();
////	m_pEnemy->Reset();
//	m_pEnemiesManager->Reset();
}

void Game::OnDeviceRestored()
{
	CreateDeviceDependentResources();

	CreateWindowSizeDependentResources();
}
#pragma endregion