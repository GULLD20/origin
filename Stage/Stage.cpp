#include "../pch.h"
#include "Stage.h"

#include "../MyLib/MyLib.h"

#include "../Character/Enemy/Goblin/Goblin.h"
#include "../Character/Enemy/LongRange/LongRange.h"
#include "../Character/Enemy/LongRangeRowling/LongRangeRowling.h"
#include "../Character/Enemy/Stamp/Stamp.h"
#include "../Character/Enemy/Rowling/Rowling.h"

#include "../DataManager/DataManager.h"

Stage::Stage()
	: m_pPlayer(nullptr)
	, m_pFollowCamera(nullptr)
{
}


Stage::~Stage()
{
	if (m_pPlayer != nullptr)
	{
		m_pPlayer.reset(nullptr);
	}

	if (m_pEnemy.size() != 0)
	{
		for (int i = 0; i < static_cast<int>(m_pEnemy.size()); i++)
		{
			m_pEnemy[i].reset(nullptr);
		}
		m_pEnemy.clear();
	}

	if (m_pFollowCamera != nullptr)
	{
		m_pFollowCamera.reset(nullptr);
	}
}

//エネミーの作成
void Stage::CreatEnemy(Params::EnemyTyp typ, DirectX::SimpleMath::Vector3 pos)
{
	//エネミーを作成したか判定
	bool create = false;

	//エネミーとモデルの作成
	switch (typ)
	{
		//ゴブリン
	case Params::EnemyTyp::Goblin:
		m_pEnemy.push_back(std::make_unique<Goblin>(pos, Params::GOBLIN_ATTACK, Params::GOBLIN_HP, Params::GOBLIN_SPEED, Params::GOBLIN_RANGE));
		m_pEnemy[m_creatEnemyNum]->CreateModel(L"Resources/CMO/Goblin.cmo");
		create = true;
		break;

		//遠距離攻撃の敵
	case Params::EnemyTyp::LongRange:
		m_pEnemy.push_back(std::make_unique<LongRange>(pos, Params::LONGRANGE_ATTACK, Params::LONGRANGE_HP, Params::LONGRANGE_SPEED, Params::LONGRANGE_RANGE));
		m_pEnemy[m_creatEnemyNum]->CreateModel(L"Resources/CMO/LongRange.cmo");
		create = true;
		break;

		//回転する遠距離攻撃の敵
	case Params::EnemyTyp::LongRangeRowling:
		m_pEnemy.push_back(std::make_unique<LongRangeRowling>(pos, Params::LONGRANGEROWLING_ATTACK, Params::LONGRANGEROWLING_HP, Params::LONGRANGEROWLING_SPEED, Params::LONGRANGEROWLING_RANGE));
		m_pEnemy[m_creatEnemyNum]->CreateModel(L"Resources/CMO/LongRangeRowling.cmo");
		create = true;
		break;

		//押しつぶす敵
	case Params::EnemyTyp::Stamp:
		m_pEnemy.push_back(std::make_unique<Stamp>(pos, Params::STAMP_ATTACK, Params::STAMP_HP, Params::STAMP_SPEED, Params::STAMP_RANGE));
		m_pEnemy[m_creatEnemyNum]->CreateModel(L"Resources/CMO/Stamp.cmo");
		create = true;
		break;

		//回転攻撃の敵
	case Params::EnemyTyp::Rowling:
		m_pEnemy.push_back(std::make_unique<Rowling>(pos, Params::ROWLING_ATTACK, Params::ROWLING_HP, Params::ROWLING_SPEED, Params::ROWLING_RANGE));
		m_pEnemy[m_creatEnemyNum]->CreateModel(L"Resources/CMO/Rowling.cmo");
		create = true;
		break;

	default:
		break;
	}

	if (create)
	{
		//作成した合計数を増やす
		m_creatEnemyNum++;
	}
}

//何体エネミーの体力が0になったか
void Stage::EnemyDethCount()
{
	//HPがゼロになった敵の数の初期化
	m_DethEnemyNum = 0;

	//敵のHPがなくなったら
	for (int i = 0; i < static_cast<int>(m_pEnemy.size()); i++)
	{
		//敵が死んでいるとき
		if (m_pEnemy[i]->IsDeth())
		{
			//死亡した敵の数を計算
			m_DethEnemyNum++;
		}
	}
}

//ゲームクリア
bool Stage::GameClear()
{
	//クリア処理
	if (m_DethEnemyNum >= static_cast<int>(m_pEnemy.size()))
	{
		return true;
	}

	EnemyDethCount();
	return false;
}

//ゲームオーバー
bool Stage::GameOver()
{
	if (m_pPlayer->GetHP() <= 0.0f)
	{
		return true;
	}

	return false;
}

//CSVを読み込む
void Stage::LoadCsv(char* failName)
{
	//CSVファイルを1行ずつ保存
	std::vector<std::string> allDataStr = MyLib::LoadFail_TextForm(failName);

	//1行ずつ保存した文字列を分割してそれを格納する変数
	std::vector<std::vector<std::string>> separate;
	separate.resize(allDataStr.size());

	//文字列を分割してそれを格納する
	for (int i = 0; i < static_cast<int>(allDataStr.size()); i++)
	{
		separate[i] = MyLib::SeparateString(allDataStr[i], ',');
	}

	m_csvData = separate;
}

//読み込んだデータを変数に格納する
void Stage::SettingCsvData()
{
	//csvのデータがマップかキャラクターのどちらを表しているか
	std::string typ = "";
	//要素の位置(縦)
	int select = 0;

	//読み込んだデータをint型に変換して格納
	for (int i = 0; i < static_cast<int>(m_csvData.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(m_csvData[i].size()); j++)
		{
			//1つの要素しか入っていないときは
			//マップかキャラクターを表しているか
			//マップのサイズを表している
			if (m_csvData[i].size() == 1)
			{
				//現在どのデータを扱っているか
				typ = m_csvData[i][j];
				//判定用のデータを格納してエラーが出ないため
				i++;
				//要素の位置を初期化
				select = 0;
			}

			if (typ == "Stage")
			{
				m_mapDataList[select][j] = std::stoi(m_csvData[i][j]);
			}
			else if (typ == "Character")
			{
				m_characterData[select][j] = std::stoi(m_csvData[i][j]);
			}
			else if (typ == "縦")
			{
				//メモリの確保(m_MapDataとm_CharacterDataの要素の確保)
				m_mapDataList.resize(std::stoi(m_csvData[i][j]));
				m_characterData.resize(std::stoi(m_csvData[i][j]));
			}
			else if (typ == "横")
			{
				//要素数
				int size = std::stoi(m_csvData[i][j]);
				for (int k = 0; k < size; k++)
				{
					//メモリの確保(m_MapDataとm_CharacterDataの要素の確保)
					m_mapDataList[k].resize(size);
					m_characterData[k].resize(size);
				}
			}
		}
		select++;
	}
}