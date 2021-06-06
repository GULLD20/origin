#include "../pch.h"
#include "Shutter.h"

#include "../DeviceResources.h"
#include "../Scene/Projection.h"
#include "../Scene/GameContext.h"

//エフェクト用の位置データ(高さ)
const float Shutter::MAX_POS = 1.0f;

Shutter::Shutter()
{
}


Shutter::~Shutter()
{
	//シェーダーで使われていたデータの削除
	if (m_fade != nullptr)
	{
		m_fade->Lost();
		m_fade.reset(nullptr);
	}
}

//生成
void Shutter::Create()
{
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();

	m_fade = std::make_unique<Fade>();
	m_fade->Create(deviceResources, 1);
}

//初期化
void Shutter::Initialize()
{
	//フェードイン・アウト確認用
	m_fadeCheck = false;
	//時間計算用
	m_fadeTimming = MAX_POS;
	//フェードイン・アウト開始用
	m_fadeIn = false;
	m_fadeOut = false;

	m_fade->InitializeNormal(5, DirectX::SimpleMath::Vector3::Zero);
}

//更新
void Shutter::Update(const DX::StepTimer &timer)
{
	//フェード中なら
	if (m_fadeCheck)
	{
		//1秒を基準と処理にするため
		float elapsTime = static_cast<float>(timer.GetElapsedSeconds());

		//フェードイン処理
		if (m_fadeIn)
		{
			m_fadeTimming -= elapsTime;

			if (m_fadeTimming <= -MAX_POS)
			{
				m_fadeTimming = -MAX_POS;
				m_fadeIn = false;
				m_fadeCheck = false;
			}
		}
		//フェードアウト処理
		else if(m_fadeOut)
		{
			m_fadeTimming += elapsTime;

			if (m_fadeTimming >= MAX_POS)
			{
				m_fadeTimming = MAX_POS;
				m_fadeOut = false;
				m_fadeCheck = false;
			}
		}
	}

	m_fade->Update(timer);
	m_fade->SetFadeTimming(m_fadeTimming);
}

//描画
void Shutter::Draw()
{
	Projection* projection = GameContext<Projection>().Get();
	DirectX::SimpleMath::Vector3 eys = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::Identity;

	m_fade->SetRenderState(eys, view, projection->GetMatrix());
	m_fade->Render();
}

//リセット
void Shutter::Reset()
{

}


//フェードイン
void Shutter::FadeIn()
{
	m_fadeCheck = true;
	m_fadeIn = true;
}

//フェードアウト
void Shutter::FadeOut()
{
	m_fadeCheck = true;
	m_fadeOut = true;
}

//フェードイン・アウトの確認
bool Shutter::GetFade() const
{
	return m_fadeCheck;
}