#include "../pch.h"

#include "ResultState.h"

#include "../DebugObj/DebugFont.h"
#include "GameContext.h"
#include "GameStateManager.h"

#include <algorithm>

#include "../DataManager/DataManager.h"
#include "../UI/Shutter.h"
#include "../UI/Score.h"

#include <fstream>

//保存するスコアの数
const int ResultState::SCORE_NUM = 10;
//PUSHSPACEの画像の描画位置
const DirectX::SimpleMath::Vector2 ResultState::PUSH_SPACE_DRAW_POS = DirectX::SimpleMath::Vector2(0.0f, 75.0f);
//PushSpaceの描画間隔
//通常
const int ResultState::PUSH_SPACE_DRAW_INTERVAL_NORMAL = 20;
//短め
const int ResultState::PUSH_SPACE_DRAW_INTERVAL_SHORT = 5;

ResultState::ResultState()
	: GameState()
	, m_pKeyBord(nullptr)
	, m_pResultFade(nullptr)
	, m_pGameOver(nullptr)
{
}



ResultState::~ResultState()
{
	//if (m_pKeyBord != nullptr)
	//{
	//	m_pKeyBord.reset(nullptr);
	//}

	if (m_pResultFade != nullptr)
	{
		m_pResultFade.reset(nullptr);
	}

	if (m_pGameOver != nullptr)
	{
		m_pGameOver.Reset();
	}

}



void ResultState::Initialize()
{
	//m_pKeyBord = std::make_unique<DirectX::Keyboard>();

	//データ作成に必要なものを取得
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();
	ID3D11Device1* device = deviceResources->GetD3DDevice();
	ID3D11DeviceContext1* deviceContext = deviceResources->GetD3DDeviceContext();

	//スコア表示クラスの初期化
	Score* score = GameContext<Score>::Get();
	score->Initialize();

	//フェード処理(後ろを半透明にする)の初期化
	m_pResultFade = std::make_unique<ResultFade>();
	m_pResultFade->Create();
	m_pResultFade->Initialize();
	//フェードイン(後ろを半透明にする)の開始
	m_pResultFade->FadeIn();

	//シーンが変わるタイミングを管理する変数の初期化
	m_chenge = false;

	//クリア判定の取得
	m_clear = GameContext<DataManager>::Get()->GetClear();

	if(!m_clear)
	{
		//画像の読み込み
		DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/GameOver.png", NULL, m_pGameOver.ReleaseAndGetAddressOf());
	}

	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/PUSH_SPACE.png", NULL, m_pTexturePushspace.ReleaseAndGetAddressOf());

	//PUSHSPACEの画像をする時間
	m_pushspaceDrawCount = 0;
	//PUSHSPACEの画像の描画判定
	m_pushspaceDraw = true;
	//PUSHSPACEの描画間隔
	m_pushspaceDrawInterval = PUSH_SPACE_DRAW_INTERVAL_NORMAL;

	//スキップ判定
	m_skip = false;

	//スコアの保存
	SaveScore();
}



void ResultState::Update(DX::StepTimer timer)
{
	//キーの取得
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	//画面を隠すフェード処理クラスの取得
	Shutter* fade = GameContext<Shutter>::Get();

	//PUSHSPACEの描画間隔の計算
	m_pushspaceDrawCount++;

	if (m_pushspaceDrawCount % m_pushspaceDrawInterval == 0)
	{
		m_pushspaceDraw = !m_pushspaceDraw;
	}

	Score* score = GameContext<Score>::Get();

	score->ResultUpdate();

	//スペースキーを押したら
	if (keyState.IsKeyDown(DirectX::Keyboard::Space))
	{
		if (!score->GetMove())
		{
			if (!m_skip)
			{
				//フェードインの開始
				m_chenge = true;
				fade->FadeIn();
				//PUSHSPACEの描画間隔を短くする
				m_pushspaceDrawInterval = PUSH_SPACE_DRAW_INTERVAL_SHORT;
			}
		}
		else
		{
			//スコアの移動のスキップ
			score->MoveScoreSkip();
		}
		//スキップ完了
		m_skip = true;
	}
	else
	{
		//スキップ後にスペースボタンを離す
		if (m_skip)
		{
			m_skip = false;
		}
	}

	//画面を隠すフェード処理の更新処理
	fade->Update(timer);

	//フェードインが終了したらシーン遷移(タイトルシーンへ)
	if (m_chenge && !fade->GetFade())
	{
		GameStateManager* gameStateManager = GameContext<GameStateManager>().Get();
		gameStateManager->RequestState("Title");
	}

	//フェード処理(後ろを半透明にする)の更新処理
	m_pResultFade->Update(timer);
}



void ResultState::Render()
{
	//描画に必要なものを取得
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();
	ID3D11Device1* device = deviceResources->GetD3DDevice();

	GameContext<DirectX::SpriteBatch>::Get()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, GameContext<DirectX::CommonStates>::Get()->NonPremultiplied());

	m_pResultFade->Draw();

	if (!m_pResultFade->GetFade())
	{
		if (m_clear)
		{
			//ランキングの表示
			GameContext<Score>::Get()->DrawRanking();
		}
		else
		{
			//ゲームオーバーの表示
			GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pGameOver.Get(),DirectX::SimpleMath::Vector2::Zero);
		}
	}

	//PushSpaceの描画
	if (m_pushspaceDraw)
	{
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pTexturePushspace.Get(), PUSH_SPACE_DRAW_POS);
	}

	GameContext<DirectX::SpriteBatch>::Get()->End();

	GameContext<Shutter>::Get()->Draw();
}



void ResultState::Finalize()
{

}

//スコアの保存
void ResultState::SaveScore()
{
	//スコアの取得(経過時間)
	int time = static_cast<int>(GameContext<DataManager>::Get()->GetTime());

	//全てのスコアをリストに格納
	Score* score = GameContext<Score>::Get();
	//最新のスコアの設定
	score->SetNowScore(time);
	//スコアのソート
	score->ScoreSort();
	//スコアの保存
	score->SaveScore();
	//最新のスコアがランキング内に入っているか判定
	score->CheckNewScore();
}