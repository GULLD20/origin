//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "CommonStates.h"
#include "Scene\Projection.h"
#include "SimpleMath.h"
#include "Mouse.h"
#include "Keyboard.h"
#include <SpriteBatch.h>

#include "GeometricPrimitive.h"

#include "UI/Shutter.h"
#include "UI/Score.h"
#include "DataManager/DataManager.h"
#include "Map/Map.h"
#include "Character/List/CharacterMapList.h"

#include "SE�EBGM/Adx2.h"

class GameStateManager;


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game : public DX::IDeviceNotify
{
public:
	static LPCWSTR WINDOW_NAME;

public:

	Game() noexcept(false);
	~Game();

	// Initialization and management
	void Initialize(const HWND &window, int width, int height);

	// Basic game loop
	void Tick();

	// IDeviceNotify
	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowMoved();
	void OnWindowSizeChanged(int width, int height);

	// Properties
	void GetDefaultSize(int& width, int& height) const;

private:

	void Update(const DX::StepTimer &timer);
	void Render();
	void Clear();

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();

private:
	// �ˉe�s��
	std::unique_ptr<Projection>             m_pProjection;

	// Device resources.
	std::unique_ptr<DX::DeviceResources>    m_pDeviceResources;

	// Rendering loop timer.
	DX::StepTimer                           m_timer;

	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>	m_pState;

	// �}�E�X
	std::unique_ptr<DirectX::Mouse>			m_pMouse;

	//�V�[���Ǘ�
	std::unique_ptr<GameStateManager>       m_pGameStateManager;

	//�X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch>   m_pSpriteBatch;

	//�t�F�[�h
	std::unique_ptr<Shutter>                m_pShutter;

	//�X�R�A
	std::unique_ptr<Score>                  m_pScore;

	//�f�[�^�Ǘ�
	std::unique_ptr<DataManager>            m_pDataManager;

	//�}�b�v
	std::unique_ptr<Map>                    m_pMap;

	//�L�����N�^�[�Ǘ��N���X
	std::unique_ptr<CharacterMapList>       m_pCharacterMapList;

	//�T�E���h
	std::unique_ptr<ADX2>                   m_pSound;
};