#include "../pch.h"
#include "ResultFade.h"

#include "../DeviceResources.h"
#include "../Scene/Projection.h"
#include "../Scene/GameContext.h"

//フェードの透明度
const float ResultFade::MAX_ALPHA = 0.7f;

ResultFade::ResultFade()
{
}


ResultFade::~ResultFade()
{
	if (m_pBlack != nullptr)
	{
		m_pBlack->Lost();
		m_pBlack.reset();
	}
}

//生成
void ResultFade::Create()
{
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();

	m_pBlack = std::make_unique<Black>();
	m_pBlack->Create(deviceResources, 1);
}

//初期化
void ResultFade::Initialize()
{
	//フェード確認用
	m_fadeCheck = false;
	//透明度計算用
	m_alpha = 0.0f;
	//フェードイン開始用
	m_fadeIn = false;

	m_pBlack->InitializeNormal(5, DirectX::SimpleMath::Vector3(0, 0, 0));
}

//更新
void ResultFade::Update(DX::StepTimer timer)
{
	//フェード処理の判定
	if (m_fadeCheck)
	{
		//フェードインの処理
		if (m_fadeIn)
		{
			//だんだんと透明度を上げる
			m_alpha += static_cast<float>(timer.GetElapsedSeconds());
		}

		//フェードが設定した透明度に達したら
		if (m_alpha >= MAX_ALPHA)
		{
			m_alpha = MAX_ALPHA;
			//フェード処理の終了
			m_fadeCheck = false;
		}
	}

	//シェーダー処理
	m_pBlack->Update(timer);
	m_pBlack->SetAlpha(m_alpha);
}

//描画
void ResultFade::Draw()
{
	Projection* projection = GameContext<Projection>().Get();
	DirectX::SimpleMath::Vector3 eys = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::Identity;

	//フェードの描画
	m_pBlack->SetRenderState(eys, view, projection->GetMatrix());
	m_pBlack->Render();
}

//リセット
void ResultFade::Reset()
{

}


//フェードインの開始
void ResultFade::FadeIn()
{
	m_fadeCheck = true;
	m_fadeIn = true;
}

//フェードインの確認　Get
bool ResultFade::GetFade() const
{
	return m_fadeCheck;
}