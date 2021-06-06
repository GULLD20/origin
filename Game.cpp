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
#include "SE�EBGM/acb/CueSheet_0.h"
///

extern void ExitGame();

using Microsoft::WRL::ComPtr;

LPCWSTR Game::WINDOW_NAME = L"�Q�[���̏�ԊǗ� [2a](�X�^�b�N����)";

Game::Game() noexcept(false)
{
	m_pDeviceResources = std::make_unique<DX::DeviceResources>();
	m_pDeviceResources->RegisterDeviceNotify(this);
	//�o�^�i�V���O���g���p�^�[�����j
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
	// �}�E�X�̍쐬
	m_pMouse = std::make_unique<DirectX::Mouse>();
	m_pMouse->SetWindow(window);

	m_pDeviceResources->SetWindow(window, width, height);

	m_pDeviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_pDeviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	auto device = m_pDeviceResources->GetD3DDevice();

	// �R�����X�e�[�g�쐬
	m_pState = std::make_unique<DirectX::CommonStates>(device);
	//�o�^�i�V���O���g���p�^�[�����j
	GameContext<DirectX::CommonStates>::Register(m_pState);

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/

	DebugFont* debugFont = DebugFont::GetInstance();
	debugFont->create(device, m_pDeviceResources->GetD3DDeviceContext());

	//�e�V�[���̐ݒ�
	m_pGameStateManager = std::make_unique<GameStateManager>();
	m_pGameStateManager->RegisterState<TitleState>("Title");
	m_pGameStateManager->RegisterState<PlayState>("Play");
	m_pGameStateManager->RegisterState<ResultState>("Result");
	m_pGameStateManager->RegisterState<PauseState>("Pause");
	m_pGameStateManager->SetStartState("Title");
	//�o�^�i�V���O���g���p�^�[�����j
	GameContext<GameStateManager>::Register(m_pGameStateManager);

	//�t�F�[�h�����̐ݒ�
	m_pShutter = std::make_unique<Shutter>();
	//�G�t�F�N�g�̍쐬
	m_pShutter->Create();
	//�e�f�[�^�̏�����
	m_pShutter->Initialize();
	//�o�^�i�V���O���g���p�^�[�����j
	GameContext<Shutter>::Register(m_pShutter);

	//�X�R�A�̍쐬
	m_pScore = std::make_unique<Score>();
	//�摜���̍쐬
	m_pScore->Create(device);
	//�o�^�i�V���O���g���p�^�[�����j
	GameContext<Score>::Register(m_pScore);

	//�f�[�^�Ǘ��N���X�̍쐬
	m_pDataManager = std::make_unique<DataManager>();
	//�f�[�^�̏�����
	m_pDataManager->Initialize();
	//�o�^�i�V���O���g���p�^�[�����j
	GameContext<DataManager>::Register(m_pDataManager);

	//�}�b�v�N���X�̍쐬
	m_pMap = std::make_unique<Map>();
	//���f���A�G�t�F�N�g�̍쐬
	m_pMap->Create();
	//�o�^�i�V���O���g���p�^�[�����j
	GameContext<Map>::Register(m_pMap);

	//�L�����N�^�[�̃��X�g�̍쐬
	m_pCharacterMapList= std::make_unique<CharacterMapList>();
	//�o�^�i�V���O���g���p�^�[�����j
	GameContext<CharacterMapList>::Register(m_pCharacterMapList);

	//�T�E���h�N���X�̍쐬
	m_pSound = std::make_unique<ADX2>();
	//�T�E���h�̓o�^
	m_pSound->Initialize("SE�EBGM/acf/SOUND.acf", "SE�EBGM/acb/CueSheet_0.acb");
	//���ʐݒ�
	m_pSound->SetVolume(1.0f);
	//�o�^�i�V���O���g���p�^�[�����j
	GameContext<ADX2>::Register(m_pSound);

	//�X�v���C�g�o�b�`�̐ݒ�
	m_pSpriteBatch = std::make_unique<DirectX::SpriteBatch>(m_pDeviceResources->GetD3DDeviceContext());
	//�o�^�i�V���O���g���p�^�[�����j
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

	//�V�[���}�l�[�W���[�̍X�V����
	m_pGameStateManager->Update(timer);
	//���Đ��̍X�V����
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
	//���݂̉�ʃT�C�Y
	width = Aspect::ASPECT_X;
	height = Aspect::ASPECT_Y;
	//�ő�̉�ʃT�C�Y
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

	// �E�C���h�E�T�C�Y����A�X�y�N�g����Z�o����
	RECT size = m_pDeviceResources->GetOutputSize();
	float aspectRatio = float(size.right) / float(size.bottom);

	// ��p��ݒ�
	float fovAngleY = DirectX::XMConvertToRadians(45.0f);

	// �ˉe�s����쐬����
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