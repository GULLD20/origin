#include "../pch.h"
#include "Score.h"

#include "../DataManager/DataManager.h"

#include <CommonStates.h>
#include "../Scene/GameContext.h"
#include "../Aspect.h"

#include "../MyLib/MyLib.h"

//一位表示位置
const DirectX::SimpleMath::Vector2 Score::F_DRAW_POS = DirectX::SimpleMath::Vector2(1160.0f, 10.0f);

//横の位置を計算するときに使う値
const float Score::F_CAL_SIDE_1 = 70.0f;
const float Score::F_CAL_SIDE_2 = 30.0f;

//高さを計算するときに使う値
const float Score::R_CAL_HIGH = 90.0f;

//横の位置を計算するときに使う値
const float Score::R_CAL_SIDE_1 = 35.0f;
const float Score::R_CAL_SIDE_2 = 70.0f;
const float Score::R_CAL_SIDE_3 = 105.0f;
const float Score::R_CAL_SIDE_4 = 140.0f;

//表示する順位
const int Score::DRAW_RANKING = 5;

//画像の半分の大きさ
const float Score::TEXTURE_HALF_SIZE_X = (504.0f * 0.5f);

//スコアの移動速度
const float Score::SCORE_MOVE_SPEED = 8.0f;

//スコアの初期位置(X)
const float Score::SCORE_START_POS_X = -1200.0f;

//数字の画像の数
const int Score::NUMBER_TEXTURE_NUM = 10;

//ランキング外の最新スコアの表示位置
const DirectX::SimpleMath::Vector2 Score::NEW_SCORE_POS = DirectX::SimpleMath::Vector2(750, 50.0f);

//最新スコアの描画間隔
const int Score::NEW_SCORE_DRAW_INTERVAL = 5;

Score::Score()
{

}

Score::~Score()
{
	if (m_pNumNormalList.size() > 0)
	{
		for (int i = 0; i< static_cast<int>(m_pNumNormalList.size()); i++)
		{
			m_pNumNormalList[i].Reset();
		}
		m_pNumNormalList.clear();
	}

	if (m_pNumNewList.size() > 0)
	{
		for (int i = 0;i< static_cast<int>(m_pNumNewList.size());i++)
		{
			m_pNumNewList[i].Reset();
		}
		m_pNumNewList.clear();
	}

	if (m_pColonNormal!= nullptr)
	{
		m_pColonNormal.Reset();
	}

	if (m_pColonNew != nullptr)
	{
		m_pColonNew.Reset();
	}

	if (m_pRankingTable != nullptr)
	{
		m_pRankingTable.Reset();
	}
}

//生成
void Score::Create(ID3D11Device1* &device)
{
	//メモリの確保
	m_pNumNormalList.resize(NUMBER_TEXTURE_NUM);
	m_pNumNewList.resize(NUMBER_TEXTURE_NUM);

	//画像の読み込み
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/0.png", NULL, m_pNumNormalList[0].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/1.png", NULL, m_pNumNormalList[1].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/2.png", NULL, m_pNumNormalList[2].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/3.png", NULL, m_pNumNormalList[3].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/4.png", NULL, m_pNumNormalList[4].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/5.png", NULL, m_pNumNormalList[5].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/6.png", NULL, m_pNumNormalList[6].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/7.png", NULL, m_pNumNormalList[7].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/8.png", NULL, m_pNumNormalList[8].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/9.png", NULL, m_pNumNormalList[9].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/コロン.png", NULL, m_pColonNormal.ReleaseAndGetAddressOf());

	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/0_New.png", NULL, m_pNumNewList[0].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/1_New.png", NULL, m_pNumNewList[1].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/2_New.png", NULL, m_pNumNewList[2].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/3_New.png", NULL, m_pNumNewList[3].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/4_New.png", NULL, m_pNumNewList[4].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/5_New.png", NULL, m_pNumNewList[5].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/6_New.png", NULL, m_pNumNewList[6].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/7_New.png", NULL, m_pNumNewList[7].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/8_New.png", NULL, m_pNumNewList[8].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/9_New.png", NULL, m_pNumNewList[9].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/コロン_New.png", NULL, m_pColonNew.ReleaseAndGetAddressOf());

	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/rankingTemporary.png", NULL, m_pRankingTable.ReleaseAndGetAddressOf());

	m_scoreDataList.resize(DRAW_RANKING);
}

//初期化
void Score::Initialize()
{
	//要素の確保
	m_calXList.resize(DRAW_RANKING);
	m_calXCheckList.resize(DRAW_RANKING);

	//スコアの描画位置の初期化
	for (int i = 0; i < DRAW_RANKING; i++)
	{
		m_calXList[i] = SCORE_START_POS_X;
		m_calXCheckList[i] = false;
	}

	m_time = 0;
	m_second = 0;
	m_minute = 0;
	m_oneDigitSecond = 0;
	m_tenDigitSecond = 0;
	m_oneDigitMinute = 0;
	m_tenDigitMinute = 0;

	//移動状態
	m_move = true;

	//最新スコアの初期化
	m_newScore.score = 0;
	m_newScore.nreScore = false;

	//ランキング内に最新スコアがあるか
	m_newScoreCheck = false;

	//最新スコアの描画間隔の初期化
	m_newScoreDrawInterval = 0;
	//最新スコアの描画判定
	m_newScoreDraw = false;
}

//リザルト用の更新処理
void Score::ResultUpdate()
{
	//位置の初期化
	//ランキングの外枠の位置を決める(横は画面の中心から画像の半分の大きさをずらすため,縦は50.0f)
	m_rBasePos = DirectX::SimpleMath::Vector2((Aspect::ASPECT_X*0.5f) - TEXTURE_HALF_SIZE_X, 50.0f);

	MoveScore();
}

//リセット
void Score::Reset()
{
	//m_Texture.Reset();
	for (int i = 0; i < static_cast<int>(m_pNumNormalList.size()); i++)
	{
		m_pNumNormalList[i].Reset();
	}

	for (int i = 0; i <static_cast<int>(m_pNumNewList.size()); i++)
	{
		m_pNumNewList[i].Reset();
	}

	m_pColonNormal.Reset();
	m_pColonNew.Reset();
}

//1位表示
void Score::DrawFirstRank()
{
	//どの画像を表示するかを決める(0番目が一位のスコアだから)
	Calculation(m_scoreDataList[0].score);
	//画像の表示
	Draw(0, false, false, false);
}

//ランキング表示
void Score::DrawRanking()
{
	//ランキングの外枠を表示
	GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pRankingTable.Get(), m_rBasePos);

	//DRAW_RANKINGの数分表示
	for (int i = 0; i < DRAW_RANKING; i++)
	{
		//画像を表示する位置の高さを調整
		m_rBasePos += DirectX::SimpleMath::Vector2(0.0f, R_CAL_HIGH);

		//表示する数値を求める
		Calculation(m_scoreDataList[i].score);

		if (m_scoreDataList[i].nreScore)
		{
			NewScoreDrawCheck();

			//最新スコアを点滅させる
			if (m_newScoreDraw)
			{
				Draw(i, true, true, false);
			}
		}
		else
		{
			Draw(i, true, false,false);
		}
	}

	//最新のスコアの表示(ランキング外)
	if (!m_newScoreCheck)
	{
		//表示する数値を求める
		Calculation(m_newScore.score);

		NewScoreDrawCheck();

		//最新スコアを点滅させる
		if (m_newScoreDraw)
		{
			//この0は関係ない
			Draw(0, false, true, true);
		}
	}
}

//描画処理
void Score::Draw(int select, bool ranking, bool newScore, bool OutOfRanking)
{
	DirectX::SimpleMath::Vector2 pos1 = F_DRAW_POS - DirectX::SimpleMath::Vector2(F_CAL_SIDE_1, 0.0f);
	DirectX::SimpleMath::Vector2 pos2 = F_DRAW_POS - DirectX::SimpleMath::Vector2(F_CAL_SIDE_2, 0.0f);
	DirectX::SimpleMath::Vector2 pos3 = F_DRAW_POS;
	DirectX::SimpleMath::Vector2 pos4 = F_DRAW_POS + DirectX::SimpleMath::Vector2(F_CAL_SIDE_2, 0.0f);
	DirectX::SimpleMath::Vector2 pos5 = F_DRAW_POS + DirectX::SimpleMath::Vector2(F_CAL_SIDE_1, 0.0f);

	if (ranking)
	{
		//スコアの表示(m_rBasePos + Vector2の値は右に画像ををずらすため)
		pos1 = m_rBasePos + DirectX::SimpleMath::Vector2(TEXTURE_HALF_SIZE_X + m_calXList[select], 0.0f);
		pos2 = m_rBasePos + DirectX::SimpleMath::Vector2(R_CAL_SIDE_1 + TEXTURE_HALF_SIZE_X + m_calXList[select], 0.0f);
		pos3 = m_rBasePos + DirectX::SimpleMath::Vector2(R_CAL_SIDE_2 + TEXTURE_HALF_SIZE_X + m_calXList[select], 0.0f);
		pos4 = m_rBasePos + DirectX::SimpleMath::Vector2(R_CAL_SIDE_3 + TEXTURE_HALF_SIZE_X + m_calXList[select], 0.0f);
		pos5 = m_rBasePos + DirectX::SimpleMath::Vector2(R_CAL_SIDE_4 + TEXTURE_HALF_SIZE_X + m_calXList[select], 0.0f);
	}

	if (OutOfRanking)
	{
		//スコアの表示(m_rBasePos + Vector2の値は右に画像ををずらすため)
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNewList[m_tenDigitMinute].Get(), NEW_SCORE_POS + DirectX::SimpleMath::Vector2(TEXTURE_HALF_SIZE_X, R_CAL_HIGH));
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNewList[m_oneDigitMinute].Get(), NEW_SCORE_POS + DirectX::SimpleMath::Vector2(R_CAL_SIDE_1 + TEXTURE_HALF_SIZE_X, R_CAL_HIGH));
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pColonNew.Get(), NEW_SCORE_POS + DirectX::SimpleMath::Vector2(R_CAL_SIDE_2 + TEXTURE_HALF_SIZE_X, R_CAL_HIGH));
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNewList[m_tenDigitSecond].Get(), NEW_SCORE_POS + DirectX::SimpleMath::Vector2(R_CAL_SIDE_3 + TEXTURE_HALF_SIZE_X, R_CAL_HIGH));
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNewList[m_oneDigitSecond].Get(), NEW_SCORE_POS + DirectX::SimpleMath::Vector2(R_CAL_SIDE_4 + TEXTURE_HALF_SIZE_X, R_CAL_HIGH));

		//スコアの表示(m_rBasePos + Vector2の値は右に画像ををずらすため)
		pos1 = NEW_SCORE_POS + DirectX::SimpleMath::Vector2(TEXTURE_HALF_SIZE_X, R_CAL_HIGH);
		pos2 = NEW_SCORE_POS + DirectX::SimpleMath::Vector2(R_CAL_SIDE_1 + TEXTURE_HALF_SIZE_X, R_CAL_HIGH);
		pos3 = NEW_SCORE_POS + DirectX::SimpleMath::Vector2(R_CAL_SIDE_2 + TEXTURE_HALF_SIZE_X, R_CAL_HIGH);
		pos4 = NEW_SCORE_POS + DirectX::SimpleMath::Vector2(R_CAL_SIDE_3 + TEXTURE_HALF_SIZE_X, R_CAL_HIGH);
		pos5 = NEW_SCORE_POS + DirectX::SimpleMath::Vector2(R_CAL_SIDE_4 + TEXTURE_HALF_SIZE_X, R_CAL_HIGH);
	}

	if (!newScore)
	{
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNormalList[m_tenDigitMinute].Get(), pos1);
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNormalList[m_oneDigitMinute].Get(), pos2);
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pColonNormal.Get(), pos3);
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNormalList[m_tenDigitSecond].Get(), pos4);
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNormalList[m_oneDigitSecond].Get(), pos5);
	}
	else
	{
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNewList[m_tenDigitMinute].Get(), pos1);
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNewList[m_oneDigitMinute].Get(), pos2);
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pColonNew.Get(), pos3);
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNewList[m_tenDigitSecond].Get(), pos4);
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNewList[m_oneDigitSecond].Get(), pos5);
	}
}

void Score::LoadScore()
{
	//テキストの読み込み
	std::vector<std::string> score = MyLib::LoadFail_TextForm("Resources/Data/Score.csv");

	//テキストに書かれているものをint型に変換して保存する
	for (int i = 0; i < DRAW_RANKING; i++)
	{
		m_scoreDataList[i].score = std::stoi(score[i]);
		m_scoreDataList[i].nreScore = false;
	}
}

//ソート(並べ替えと表示データ以外削除)
void Score::ScoreSort()
{
	//ソート処理
	ScoreData t;

	for (int i = 0; i < static_cast<int>(m_scoreDataList.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(m_scoreDataList.size()); j++)
		{
			if (m_scoreDataList[i].score < m_scoreDataList[j].score)
			{
				t = m_scoreDataList[i];
				m_scoreDataList[i] = m_scoreDataList[j];
				m_scoreDataList[j] = t;
			}
		}
	}

	//余分なデータの削除
	int restNum = static_cast<int>(m_scoreDataList.size()) - DRAW_RANKING;
	for (int i = 0; i < restNum; i++)
	{
		m_scoreDataList.pop_back();
	}
}

//スコアの保存
void Score::SaveScore()
{
	//テキストを開く(スコアの保存先)
	std::ofstream ofs("Resources/Data/Score.csv");

	//テキストにスコアを保存
	for (int i = 0; i < static_cast<int>(m_scoreDataList.size()); i++)
	{
		ofs << m_scoreDataList[i].score << std::endl;
	}
}

//ランキングの中に最新のスコアがあるか判定
void Score::CheckNewScore()
{
	for (int i = 0; i < static_cast<int>(m_scoreDataList.size()); i++)
	{
		if (m_scoreDataList[i].nreScore)
		{
			m_newScoreCheck = true;
			break;
		}
	}
}

//現在のスコアの取得(プレイ結果)
void Score::SetNowScore(int score)
{
	//全てのデータを保存する
	std::vector<ScoreData> all;

	//新しいデータを入れるためもともとの要素数+1でメモリを確保する
	all.resize(m_scoreDataList.size() + 1);

	//データの格納
	for (int i = 0; i < static_cast<int>(all.size()); i++)
	{
		//要素数は0から順番なため-1
		if (i == static_cast<int>(all.size()-1))
		{
			//最新データを入れる
			all[i].score = score;
			all[i].nreScore = true;
			break;
		}

		//これまでのデータを入れる
		all[i] = m_scoreDataList[i];
	}

	m_scoreDataList = all;

	//要素数は0から順番なため-1
	//最新データを入れる
	m_newScore = all[static_cast<int>(all.size() - 1)];
}

//スコアの移動をスキップする
void Score::MoveScoreSkip()
{
	//スコアの描画位置の初期化
	for (int i = 0; i < DRAW_RANKING; i++)
	{
		m_calXList[i] = 0.0f;
		m_calXCheckList[i] = true;
	}

	m_move = false;
}


//移動状態か取得
bool Score::GetMove() const
{
	return m_move;
}

//時間計算用
void Score::Calculation(const int score)
{
	//分を求めるために60で割る
	m_minute = score / 60;

	//分を求める(十桁を求めるために10で割る)
	m_tenDigitMinute = m_minute / 10;

	//分を求める(一桁を求めるために十桁(十桁にするために10をかける)の数値を引く)
	m_oneDigitMinute = m_minute - (m_tenDigitMinute * 10);;

	//秒を求めるために分を引く(分の数値に60をかけて秒に変換する)
	m_second = score - (60 * m_minute);

	//秒を求める(十桁を求めるために10で割る)
	m_tenDigitSecond = m_second / 10;

	//秒を求める(一桁を求めるために十桁(十桁にするために10をかける)の数値を引く)
	m_oneDigitSecond = m_second - (m_tenDigitSecond * 10);
}

//スコアの移動
void Score::MoveScore()
{
	for (int i = 0; i < DRAW_RANKING; i++)
	{
		//一つ目の移動
		if (i - 1 < 0)
		{
			m_calXList[i] += SCORE_MOVE_SPEED;
			if (m_calXList[i] >= 0)
			{
				//移動完了
				m_calXList[i] = 0;
				m_calXCheckList[i] = true;
			}
			continue;
		}

		//それ以外の移動
		if (m_calXCheckList[i - 1])
		{
			m_calXList[i] += SCORE_MOVE_SPEED;

			if (m_calXList[i] >= 0)
			{
				//移動完了
				m_calXList[i] = 0.0f;
				m_calXCheckList[i] = true;

				//要素は0から順番なため-1
				if (i == DRAW_RANKING - 1)
				{
					//全ての移動が完了
					m_move = false;
				}
			}
		}
	}
}

//最新スコアの描画判定
void Score::NewScoreDrawCheck()
{
	m_newScoreDrawInterval++;

	//最新スコアの描画間隔を計算する
	if (m_newScoreDrawInterval >= NEW_SCORE_DRAW_INTERVAL)
	{
		m_newScoreDraw = !m_newScoreDraw;
		m_newScoreDrawInterval = 0;
	}
}
