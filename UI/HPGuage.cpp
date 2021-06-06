#include "../pch.h"
#include "HPGuage.h"

#include "../DataManager/DataManager.h"

HPGuage::HPGuage()
{
}


HPGuage::~HPGuage()
{
	
}

//生成
void HPGuage::Create(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	//画像の読み込み
	m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/HPGuage/Player/HPBack.png", NULL, m_Guage[0].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/HPGuage/Player/HP.png", NULL, m_Guage[1].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/HPGuage/Player/HPFrame.png", NULL, m_Guage[2].ReleaseAndGetAddressOf());
}

//初期化
void HPGuage::Initialize()
{
	//位置の初期化
	m_Pos = DirectX::SimpleMath::Vector2(10.0f,10.0f);
	//プレイヤーのHPを入れる
	m_HP = 0.0f;

	m_TextureSize =
	{
		0,
		0,
		TEXTUERSIZEX,
		TEXTUERSIZEY,
	};
}

// 更新
void HPGuage::Update()
{
	DataManager*DataManager = DataManager::CreateInstance();
	//プレイヤーのHPの取得
	m_HP = DataManager->GetPlayerHP();

	//プレイヤーのHPの１％の値を求める
	float onePercent = DataManager->GetPlayerMaxHP() * 0.01f;

	//現在のプレイヤーのHPが何％残っているか求める
	float RemainingGuage = m_HP / onePercent;

	//画像の横の大きさの１％を求める
	float textuerSizeOnePercent = TEXTUERSIZEX * 0.01f;

	//求めたtextuerSizeOnePercentとRemainingGuageをかけた画像をどれくらい表示するか決める(X)
	int sizeX = static_cast<int>(textuerSizeOnePercent * RemainingGuage);

	m_TextureSize =
	{
		0,
		0,
		sizeX,
		TEXTUERSIZEY,
	};
}

//描画
void HPGuage::Draw()
{
	//画像の表示
	m_SpriteBatch->Begin();

	m_SpriteBatch->Draw(m_Guage[0].Get(), m_Pos);
	m_SpriteBatch->Draw(m_Guage[1].Get(), m_Pos,&m_TextureSize);
	m_SpriteBatch->Draw(m_Guage[2].Get(), m_Pos);

	m_SpriteBatch->End();
}

//リセット
void HPGuage::Reset()
{
	//リセット
	//m_Texture.Reset();
	for (int i = 0; i < 3; i++)
	{
		m_Guage[i].Reset();
	}
	m_SpriteBatch.reset();
}