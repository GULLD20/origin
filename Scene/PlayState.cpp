#include "../pch.h"

#include "PlayState.h"

#include <Keyboard.h>

#include "../DebugObj/DebugCamera.h"
#include "../DebugObj/DebugFont.h"
#include "../DeviceResources.h"
#include "GameContext.h"
#include "GameStateManager.h"
#include "../DebugObj/GridFloor.h"

#include "../DebugObj/DebugCapsule.h"

#include "../Character/List/CharacterMapList.h"

#include "../Stage/Stage1/Stage1.h"

#include "../UI/Time.h"

#include "../DataManager/DataManager.h"

#include "../Map/Map.h"

#include "../SE・BGM/Adx2.h"
#include "../SE・BGM/acb/CueSheet_0.h"

#include "../UI/Shutter.h"

#include "../MyEffect/Effect2D/Purpose/Purpose.h"

PlayState::PlayState()
	: GameState()
	, m_pKeyBord(nullptr)
	, m_pStage(nullptr)
	, m_pPurpose(nullptr)
{
}



PlayState::~PlayState()
{
	if (m_pKeyBord != nullptr)
	{
		m_pKeyBord.reset(nullptr);
	}

	if (m_pStage != nullptr)
	{
		m_pStage.reset();
	}

	if (m_pPurpose != nullptr)
	{
		m_pPurpose->Lost();
		m_pPurpose.reset(nullptr);
	}
}



void PlayState::Initialize()
{
	//データ管理クラスの初期化
	GameContext<DataManager>::Get()->Initialize();

	//キーボード作成
	m_pKeyBord = std::make_unique<DirectX::Keyboard>();
	//ステージの作成
	m_pStage = std::make_unique<Stage1>();
	m_pStage->LoadCsv("Resources/CSV/map1.csv");
	m_pStage->SettingCsvData();

	//ステージの初期化
	m_pStage->Initialize();

	//その他の初期化処理(タイマー(スコアのUI、目的表示のエフェクト))
	OtherInitialize();

	//BGMの再生
	GameContext<ADX2>::Get()->Play(CRI_CUESHEET_0_BGM);

	//フェードアウト処理
	GameContext<Shutter>::Get()->FadeOut();
}

void PlayState::Update(DX::StepTimer timer)
{
	Shutter* shutter = GameContext<Shutter>::Get();

	//フェードアウト処理
	if (shutter->GetFade())
	{
		shutter->Update(timer);
		return;
	}

	DirectX::Keyboard::State keyState = m_pKeyBord->GetState();
	if (keyState.IsKeyDown(DirectX::Keyboard::P))
	{
		//ポウズ画面
		GameStateManager* gameStateManager = GameContext<GameStateManager>().Get();
		gameStateManager->PushState("Pause");
	}

	//ゲームクリア判定
	GameClear();
	//ゲームオーバー判定
	GameOver();

	//ステージの更新処理
	m_pStage->Update(timer);

	//その他の更新処理(タイマー(スコアのUI、目的表示のエフェクト))
	OtherUpdate(timer);

	//現在の合計時間を設定
	GameContext<DataManager>::Get()->SetTime(m_pTime->GetPlayTime());
}



void PlayState::Render()
{
	GameContext<DirectX::SpriteBatch>::Get()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, GameContext<DirectX::CommonStates>::Get()->NonPremultiplied());

	//ステージの描画
	m_pStage->Render();

	//目的の描画
	m_pPurpose->Render();

	DebugFont* debugFont = DebugFont::GetInstance();
	debugFont->print(10, 60, L"PlayState");
	debugFont->draw();
	debugFont->print(10, 100, L"[P] Pause");

	debugFont->draw();

	//現在の時間(スコア)の表示
	m_pTime->Draw();

	GameContext<DirectX::SpriteBatch>::Get()->End();
	//フェード処理中のみ描画
	Shutter* shutter = GameContext<Shutter>::Get();
	if (shutter->GetFade())
	{
		shutter->Draw();
	}
}



void PlayState::Finalize()
{
	m_pTime->Reset();
}


//その他の初期化処理(タイマー(スコアのUI、目的表示のエフェクト))
void PlayState::OtherInitialize()
{
	DX::DeviceResources*   deviceResources = GameContext<DX::DeviceResources>::Get();
	ID3D11Device1*          device = deviceResources->GetD3DDevice();
	ID3D11DeviceContext1*   deviceContext = deviceResources->GetD3DDeviceContext();

	//時間UIの作成
	m_pTime = std::make_unique<Time>();
	m_pTime->Create(device);
	//時間UIの初期化処理
	m_pTime->Initialize();
	//目的表示のエフェクトの作成
	m_pPurpose = std::make_unique<Purpose>();

	//目的表示のエフェクトの初期化
	m_pPurpose->Create(deviceResources, L"Resources/Textures/Purpose/AllDestroy.png", 1);
	m_pPurpose->InitializeNormal(5, DirectX::SimpleMath::Vector3(0, 0, 0));
}

//その他の更新処理(タイマー(スコアのUI、目的表示のエフェクト))
void PlayState::OtherUpdate(DX::StepTimer timer)
{
	//1フレームにかかった時間を設定
	m_pTime->SetElapsedTime(static_cast<float>(timer.GetElapsedSeconds()));
	m_pTime->Update();

	//目的の表示クラスの更新
	m_pPurpose->SetRenderState(m_pStage->GetCamera()->GetEysPos(), m_pStage->GetCamera()->GetView(), GameContext<Projection>().Get()->GetMatrix());
	m_pPurpose->SetPlayTotalTime(m_pTime->GetPlayTime());
	m_pPurpose->Update(timer);
}

//ゲームクリア
void PlayState::GameClear()
{
	//クリア判定
	if (m_pStage->GameClear())
	{
		//ゲームクリア
		GameContext<DataManager>::Get()->SetClear(true);
		//リザルトシーンに変更
		GameStateManager* gameStateManager = GameContext<GameStateManager>().Get();
		gameStateManager->PushState("Result");
	}
}

//ゲームオーバー
void PlayState::GameOver()
{
	//判定(StageのGameOverを使う)
	if (m_pStage->GameOver())
	{
		//ゲームオーバー
		GameContext<DataManager>::Get()->SetClear(false);
		//リザルトシーンに変更
		GameStateManager* gameStateManager = GameContext<GameStateManager>().Get();
		gameStateManager->PushState("Result");
	}
}