#pragma once

#include <SpriteBatch.h>
#include <SimpleMath.h>
#include "../DeviceResources.h"
#include <WICTextureLoader.h>

#include <vector>

class Score
{
public:
	//スコアのデータ
	struct ScoreData
	{
		//スコア
		int  score;
		//最新のスコアか判定
		bool nreScore;
	};

public:
	///R_ = ランキング用  F_一位表示用-------
	//一位表示位置
	const static DirectX::SimpleMath::Vector2 F_DRAW_POS;

	//横の位置を計算するときに使う値(一位)
	const static float                        F_CAL_SIDE_1;
	const static float                        F_CAL_SIDE_2;

	//高さを計算するときに使う値(ランキング)
	const static float                        R_CAL_HIGH;

	//横の位置を計算するときに使う値(ランキング)
	const static float                        R_CAL_SIDE_1;
	const static float                        R_CAL_SIDE_2;
	const static float                        R_CAL_SIDE_3;
	const static float                        R_CAL_SIDE_4;
	///--------------------------------------

	//表示する順位
	const static int                          DRAW_RANKING;

	//画像の半分の大きさ
	const static float                        TEXTURE_HALF_SIZE_X;

	//スコアの移動速度
	const static float                        SCORE_MOVE_SPEED;

	//スコアの初期位置(X)
	const static float                        SCORE_START_POS_X;

	//数字の画像の数
	const static int                          NUMBER_TEXTURE_NUM;

	//ランキング外の最新スコアの表示位置
	const static DirectX::SimpleMath::Vector2 NEW_SCORE_POS;

	//最新スコアの描画間隔
	const static int                          NEW_SCORE_DRAW_INTERVAL;

public:
	Score();
	~Score();

	//生成
	void Create(ID3D11Device1* &device);
	//初期化
	void Initialize();
	//リザルト用の更新処理
	void ResultUpdate();
	// リセット
	void Reset();

	//1位表示
	void DrawFirstRank();
	//ランキング表示
	void DrawRanking();

	//描画処理
	void Draw(int select, bool ranking,bool newScore, bool OutOfRanking);

	//スコアの読み込み
	void LoadScore();
	//ソート(並べ替えと表示データ以外削除)
	void ScoreSort();
	//スコアの保存
	void SaveScore();

	//ランキングの中に最新のスコアがあるか判定
	void CheckNewScore();

	//現在のスコアの取得(プレイ結果)
	void SetNowScore(int score);

	//スコアの移動をスキップする
	void MoveScoreSkip();

	//移動状態か取得
	bool GetMove() const;

private:
	//時間計算用
	void Calculation(const int score);
	//スコアの移動
	void MoveScore();
	//最新スコアの描画判定
	void NewScoreDrawCheck();

private:
	//画像保存
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pNumNormalList;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pNumNewList;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>              m_pColonNormal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>              m_pColonNew;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>              m_pRankingTable;

	//全てのスコア
	std::vector<ScoreData>                                        m_scoreDataList;
	//最新のスコア
	ScoreData                                                     m_newScore;

	//画像の表示の基本位置(r = ランキング用)
	DirectX::SimpleMath::Vector2                                  m_rBasePos;

	//最新スコアの描画間隔
	int                                                           m_newScoreDrawInterval;
	//最新スコアの描画判定
	bool                                                          m_newScoreDraw;

	//合計時間
	int                                                           m_time;
	//合計(秒)
	int                                                           m_second;
	//合計(分)
	int                                                           m_minute;
	//各桁ごとの数値
	//秒
	int                                                           m_oneDigitSecond;
	int                                                           m_tenDigitSecond;
	//分
	int                                                           m_oneDigitMinute;
	int                                                           m_tenDigitMinute;

	//スコアの移動
	std::vector<float>                                            m_calXList;
	//スコアの移動完了か判定
	std::vector<bool>                                             m_calXCheckList;

	//移動状態の判定
	bool                                                          m_move;

	//ランキング内に最新スコアがあるか
	bool                                                          m_newScoreCheck;
};