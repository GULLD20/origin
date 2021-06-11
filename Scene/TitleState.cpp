#include "../pch.h"

#include "TitleState.h"

#include "../DebugObj/DebugFont.h"
#include "GameContext.h"
#include "GameStateManager.h"

#include <Keyboard.h>
#include <CommonStates.h>

#include"../DataManager/DataManager.h"
#include"../UI/Score.h"
#include "../UI/Shutter.h"

#include "../MyLib/MyLib.h"
#include "../Map/Map.h"

//マップの数(現在は1つだがテストで表示するために3にする)
const int TitleState::MAX_MAP_SELECT = 3;
//マップの描画切り替え時間
const float TitleState::MAP_DRAW_CHANGE_TIME = 8.0;

TitleState::TitleState()
	: GameState()
	, m_pKeyBord(nullptr)
{
}



TitleState::~TitleState()
{
	if (m_pKeyBord != nullptr)
	{
		m_pKeyBord.reset(nullptr);
	}

	//if (m_pScreenCapture != nullptr)
	//{
	//	delete m_pScreenCapture;
	//	m_pScreenCapture = nullptr;
	//}
}



void TitleState::Initialize()
{
	DataManager* dataManager = GameContext<DataManager>::Get();

	m_pKeyBord = std::make_unique<DirectX::Keyboard>();

	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();
	ID3D11Device1* device = deviceResources->GetD3DDevice();
	ID3D11DeviceContext1* deviceContext = deviceResources->GetD3DDeviceContext();

	//インスタンスの取得
	Score* score = GameContext<Score>::Get();
	//スコアの読み込み
	score->LoadScore();
	//スコアのソート
	score->ScoreSort();
	//スコアクラスの初期化
	score->Initialize();

	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/NormalAction.png", NULL, m_pTextureTitle.ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/PUSH_SPACE.png", NULL, m_pTexturePushspace.ReleaseAndGetAddressOf());

	m_checge = false;

	Shutter* fade = GameContext<Shutter>::Get();
	fade->FadeOut();
	m_sceneTrandition = fade->GetFade();

	//PUSHSPACEの画像をする時間
	m_pushspaceDrawCount = 0;
	//PUSHSPACEの画像の描画判定
	m_pushspaceDraw = true;
	//PUSHSPACEの描画間隔
	m_pushspaceDrawInterval = 20;

	//マップの描画の切り替え用
	m_mapDrawChengeTimer = 0.0f;
	//描画するマップの種類
	m_mapSelect = 0;


	//m_pScreenCapture = new ScreenCapture();
	//m_pScreenCapture->Initialize();

	LoadMapData();

	//マップの生成
	Map* map = GameContext<Map>::Get();
	map->Reset();
	map->SetMapData(m_mapDataList[0]);
	map->Initialize();
}



void TitleState::Update(DX::StepTimer timer)
{
	DirectX::Keyboard::State keyState = m_pKeyBord->Get().GetState();
	Shutter* fade = GameContext<Shutter>::Get();

	m_pushspaceDrawCount++;

	//フェード更新
	fade->Update(timer);
	m_sceneTrandition = fade->GetFade();

	if (m_pushspaceDrawCount % m_pushspaceDrawInterval == 0)
	{
		m_pushspaceDraw = !m_pushspaceDraw;
	}

	//スペースキーを押したら
	if (keyState.IsKeyDown(DirectX::Keyboard::Space))
	{
		if (!m_sceneTrandition)
		{
			//シーン遷移の判定
			m_checge = true;
			//フェードイン開始
			fade->FadeIn();
			//PUSHSPACEの描画間隔を短くする
			m_pushspaceDrawInterval = 5;
		}
	}

	//シーン遷移
	if (m_checge && !fade->GetFade())
	{
		//プレイシーンに変更
		GameStateManager* gameStateManager = GameContext<GameStateManager>().Get();
		gameStateManager->RequestState("Play");
	}

	//m_pScreenCapture->Update(timer);

	m_mapDrawChengeTimer += static_cast<float>(timer.GetElapsedSeconds());

	if (m_mapDrawChengeTimer >= MAP_DRAW_CHANGE_TIME)
	{
		m_mapSelect++;

		if (m_mapSelect >= MAX_MAP_SELECT)
		{
			m_mapSelect = 0;
		}

		//マップの生成
		Map* map = GameContext<Map>::Get();
		//格納されているデータを削除する
		map->Reset();
		//次に表示するマップのデータを入れる
		map->SetMapData(m_mapDataList[m_mapSelect]);
		//マップの生成
		map->Initialize();

		m_mapDrawChengeTimer = 0.0f;
	}
}



void TitleState::Render()
{
	DirectX::CommonStates* commonState = GameContext<DirectX::CommonStates>().Get();

	DirectX::SimpleMath::Matrix matrix;
	matrix = DirectX::SimpleMath::Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(0.0f, 15.0f, 75.5f), DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Up);
	//タイトル画面でのマップ表示（2つ目と3つ目は仮置き）
	GameContext<Map>::Get()->TitleDraw(matrix);

	////画像の表示
	GameContext<DirectX::SpriteBatch>::Get()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, GameContext<DirectX::CommonStates>::Get()->NonPremultiplied());

	//現段階での一位を表示
	GameContext<Score>::Get()->DrawFirstRank();
	
	//タイトルの表示
	GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pTextureTitle.Get(), DirectX::SimpleMath::Vector2(0,10.0f));
	
	//PushSpaceの描画判定（点滅描画）
	if (m_pushspaceDraw)
	{
		//PushSpaceの描画
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pTexturePushspace.Get(), DirectX::SimpleMath::Vector2(0, 0));
	}

	//m_pScreenCapture->Draw();

	GameContext<DirectX::SpriteBatch>::Get()->End();

	//フェードエフェクトの描画
	GameContext<Shutter>::Get()->Draw();
}



void TitleState::Finalize()
{

}

//マップデータを読み込む
void TitleState::LoadMapData()
{
	std::vector<std::vector<std::string>> allDataStr;
	int size = 0;
	std::vector<std::vector<std::vector<std::string>>> separate;
	//csvファイルの読み込み
	allDataStr = LoadCSV();
	size = GettingElementCount(allDataStr);
	separate = SeparateString(size, allDataStr);
	

	//csvのデータがマップかキャラクターのどちらを表しているか
	std::string typ = "";
	//要素の位置(縦)
	int select = 0;

	m_mapDataList.resize(size);

	for (int i = 0; i < size; i++)
	{
		//読み込んだデータをint型に変換して格納
		for (int j = 0; j < static_cast<int>(separate[i].size()); j++)
		{
			for (int k = 0; k < static_cast<int>(separate[i][j].size()); k++)
			{
				//1つの要素しか入っていないときは
				//マップかキャラクターを表しているか
				//マップのサイズを表している
				if (separate[i][j].size() == 1)
				{
					//現在どのデータを扱っているか
					typ = separate[i][j][k];
					//判定用のデータを格納してエラーが出ないため
					j++;
					//要素の位置を初期化
					select = 0;
				}

				if (typ == "Stage")
				{
					m_mapDataList[i][select][k] = std::stoi(separate[i][j][k]);
				}
				else if (typ == "縦")
				{
					//メモリの確保(m_MapDataとm_CharacterDataの要素の確保)
					m_mapDataList[i].resize(std::stoi(separate[i][j][k]));
				}
				else if (typ == "横")
				{
					//要素数
					int memorySize = std::stoi(separate[i][j][k]);
					for (int l = 0; l < memorySize; l++)
					{
						//メモリの確保(m_MapDataとm_CharacterDataの要素の確保)
						m_mapDataList[i][l].resize(memorySize);
					}
				}
			}
			select++;
		}
	}
}

//csvファイルの読み込み
std::vector<std::vector<std::string>> TitleState::LoadCSV()
{
	//ファイルを指定するための数値(ファイルの数)
	int k = 0;
	std::vector<std::vector<std::string>> allDataStr;
	allDataStr.resize(100);
	while (true)
	{
		//ファイル名の指定
		std::stringstream ss;
		//ファイル名の指定
		ss << "Resources/CSV/map" << k + 1 << ".csv";

		//CSVファイルを1行ずつ保存
		std::vector < std::string> csvData = MyLib::LoadFail_TextForm(ss.str().data());

		allDataStr[k] = csvData;
		//データが存在しないため終了
		if (csvData.size() == 0)
		{
			break;
		}

		k++;
	}

	return allDataStr;
}

//要素数の取得
int TitleState::GettingElementCount(const std::vector<std::vector<std::string>> &allData)
{
	//サイズの指定
	int size = 0;
	for (int i = 0; i < static_cast<int>(allData.size()); i++)
	{
		if (allData[i].size() == 0)
		{
			break;
		}

		size++;
	}

	return size;
}

//文字列の分割
std::vector<std::vector<std::vector<std::string>>> TitleState::SeparateString(int size, const std::vector<std::vector<std::string>> &allData)
{
	//1行ずつ保存した文字列を分割してそれを格納する変数
	std::vector<std::vector<std::vector<std::string>>> separate;
	separate.resize(size);

	//列ごとのメモリを確保
	for (int i = 0; i < size; i++)
	{
		separate[i].resize(static_cast<int>(allData[i].size()));
	}

	//文字列を分割してそれを格納する
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < static_cast<int>(allData[i].size()); j++)
		{
			separate[i][j] = MyLib::SeparateString(allData[i][j], ',');
		}
	}

	return separate;
}