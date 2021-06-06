#pragma once


#include "GameState.h"

#include <vector>
#include <Keyboard.h>
#include <WICTextureLoader.h>
#include <SpriteBatch.h>
#include "../StepTimer.h"

#include "../UI/ResultFade.h"

class ResultState : public GameState
{
public:
	//保存するスコアの数
	const static int                          SCORE_NUM;
	//PUSHSPACEの画像の描画位置
	const static DirectX::SimpleMath::Vector2 PUSH_SPACE_DRAW_POS;
	//PushSpaceの描画間隔
	//通常
	const static int                          PUSH_SPACE_DRAW_INTERVAL_NORMAL;
	//短め
	const static int                          PUSH_SPACE_DRAW_INTERVAL_SHORT;
public:
	ResultState();

public:
	virtual ~ResultState();


public:
	void Initialize() override;
	void Update(DX::StepTimer timer) override;
	void Render() override;
	void Finalize() override;
private:
	//スコアの保存
	void SaveScore();

private:
	//キー
	std::unique_ptr<DirectX::Keyboard>               m_pKeyBord;
	//フェード処理(後ろを半透明にする)
	std::unique_ptr<ResultFade>                      m_pResultFade;
	//シーンが変わるタイミングを管理
	bool                                             m_chenge;
	//ゲームオーバーの画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pGameOver;
	//クリア判定
	bool                                             m_clear;

	//PUSHSPACEの画像をする時間
	int                                              m_pushspaceDrawCount;
	//PUSHSPACEの画像の描画判定
	bool                                             m_pushspaceDraw;
	//PUSHSPACEの描画間隔
	int                                              m_pushspaceDrawInterval;

	//Pushspaceの画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexturePushspace;

	//スキップ判定
	bool                                             m_skip;
};
