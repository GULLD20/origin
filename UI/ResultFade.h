#pragma once

#include <SpriteBatch.h>
#include <SimpleMath.h>
#include "../DeviceResources.h"
#include <WICTextureLoader.h>

#include "../StepTimer.h"
#include "../MyEffect/Effect2D/Fade/Black.h"

class ResultFade
{
public:
	//フェードの透明度
	const static float MAX_ALPHA;
public:
	ResultFade();
	~ResultFade();

	//生成
	void Create() ;
	//初期化
	void Initialize();
	//更新
	void Update(DX::StepTimer timer);
	// 描画
	void Draw();
	// リセット
	void Reset();

	//フェードインの開始
	void FadeIn();
	//フェードインの確認　Get
	bool GetFade() const;

private:
	//フェード確認
	bool                   m_fadeCheck;
	//フェードイン開始用
	bool                   m_fadeIn;

	//フェードエフェクト
	std::unique_ptr<Black> m_pBlack;

	//透明度計算用
	float                  m_alpha;
};