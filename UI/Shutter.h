#pragma once

#include <SpriteBatch.h>
#include <SimpleMath.h>
#include "../DeviceResources.h"
#include <WICTextureLoader.h>

#include "../StepTimer.h"
#include "../MyEffect/Effect2D/Fade/Fade.h"

class Shutter
{
public:
	//エフェクト用の位置データ(高さ)
	const static float MAX_POS;
public:
	Shutter();
	~Shutter();

	//生成
	void Create() ;
	//初期化
	void Initialize();
	//更新
	void Update(const DX::StepTimer &timer);
	// 描画
	void Draw();
	// リセット
	void Reset();

	//フェードイン
	void FadeIn();
	//フェードアウト
	void FadeOut();
	//フェードイン・アウトの確認
	bool GetFade() const;

private:
	//フェードイン・アウトの確認
	bool                  m_fadeCheck;
	//フェードイン・アウト開始用
	bool                  m_fadeIn;
	bool                  m_fadeOut;

	//フェードエフェクト
	std::unique_ptr<Fade> m_fade;

	//時間計算用
	float                 m_fadeTimming;
};