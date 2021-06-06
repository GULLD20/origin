#include "../pch.h"
#include "EnemyHP.h"

#include <SpriteBatch.h>
#include <string>

#include "../Scene/GameContext.h"

//画像の大きさ
const int EnemyHP::TEXTUER_SIZE_X             = 142;
const int EnemyHP::TEXTUER_SIZE_Y             = 17;

//位置の基準
const DirectX::SimpleMath::Vector2 EnemyHP::BASE_POS  = DirectX::SimpleMath::Vector2(10.0f, 450.0f);
//HPゲージの位置計算用
const DirectX::SimpleMath::Vector2 EnemyHP::HP_POS_CAL = DirectX::SimpleMath::Vector2(44.0f, 40.6f);
//縦の位置の計算用
const float EnemyHP::POS_CAL_Y                = 70.0f;

EnemyHP::EnemyHP()
{

}

EnemyHP::~EnemyHP()
{
	//m_Texture.Reset();
	if (m_pHpGuage != nullptr)
	{
		m_pHpGuage.Reset();
	}

	if (m_pBack != nullptr)
	{
		m_pBack.Reset();
	}

	if (m_pDiscovery != nullptr)
	{
		m_pDiscovery.Reset();
	}

	if (m_dataMapList.size() > 0)
	{
		m_dataMapList.clear();
	}

	if (m_pName != nullptr)
	{
		m_pName->Reset();
	}
}

//生成
void EnemyHP::Create(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	//画像の読み込み
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/HPGuage/Enemy/EnemyHPGuage.png", NULL, m_pHpGuage.ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/HPGuage/Enemy/Back.png", NULL, m_pBack.ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/HPGuage/Enemy/Discovery.png", NULL, m_pDiscovery.ReleaseAndGetAddressOf());

	m_pName = std::make_unique<DrawName>();
	m_pName->Create(device);
}

//初期化
void EnemyHP::Initialize(float maxHp, const std::string& name, int createNumber)
{
	//敵のHPを入れる
	m_dataMapList[createNumber].hp = 0.0f;
	//HPの最大値を入れる
	m_dataMapList[createNumber].maxHp = maxHp;
	//生きているか
	m_dataMapList[createNumber].alive = true;
	//画像の表示サイズ
	m_dataMapList[createNumber].textureSize =
	{
		0,
		0,
		TEXTUER_SIZE_X,
		TEXTUER_SIZE_Y,
	};
	//敵の名前を入れる
	m_dataMapList[createNumber].name = name;

	m_pName->Initialize();
}

// 更新
void EnemyHP::Update(float hp, int createNumber)
{
	m_setingPos = 0;

	//敵のHPの取得
	m_dataMapList[createNumber].hp = hp;

	//敵のHPの１％の値を求める
	float onePercent = m_dataMapList[createNumber].maxHp * 0.01f;

	//現在の敵のHPが何％残っているか求める
	float RemainingGuage = m_dataMapList[createNumber].hp / onePercent;

	//画像の横の大きさの１％を求める
	float textuerSizeOnePercent = TEXTUER_SIZE_X * 0.01f;

	//求めたtextuerSizeOnePercentとRemainingGuageをかけた画像をどれくらい表示するか決める(X)
	int sizeX = static_cast<int>(textuerSizeOnePercent * RemainingGuage);

	//画像の大きさの設定
	m_dataMapList[createNumber].textureSize =
	{
		0,
		0,
		sizeX,
		TEXTUER_SIZE_Y,
	};
}

//描画
void EnemyHP::Draw(int createNumber,DirectX::CommonStates& state)
{
	//画像の表示
	for (int i =0;i<static_cast<int>(m_dataMapList.size());i++)
	{
		if (m_dataMapList[createNumber].alive)
		{
			if (i == createNumber)
			{
				if (m_dataMapList[createNumber].discovery)
				{
					//後ろの黒い画像を表示
					GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pBack.Get(), DirectX::SimpleMath::Vector2(BASE_POS.x , BASE_POS.y + (m_setingPos*POS_CAL_Y)));
					//名前の表示
					m_pName->Draw(m_dataMapList[createNumber].name, DirectX::SimpleMath::Vector2(BASE_POS.x, BASE_POS.y + (m_setingPos*POS_CAL_Y)), state);
					//HPゲージの表示
					GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pHpGuage.Get(), DirectX::SimpleMath::Vector2((BASE_POS.x + HP_POS_CAL.x), ((BASE_POS.y + HP_POS_CAL.y) + (m_setingPos*POS_CAL_Y))), &m_dataMapList[createNumber].textureSize);

					//発見状態の表示
					if (m_dataMapList[createNumber].battle)
					{
						GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pDiscovery.Get(), DirectX::SimpleMath::Vector2(BASE_POS.x, BASE_POS.y + (m_setingPos*POS_CAL_Y)));
					}

					//複数の敵の場合位置をずらすために++する
					m_setingPos++;
				}
			}
		}
	}
}

//リセット
void EnemyHP::Reset()
{

}

//生きているかのデータを入れる
void EnemyHP::SetAlive(bool alive, int createNumber)
{
	m_dataMapList[createNumber].alive = alive;
}

//発見されているかのデータを入れる
void EnemyHP::SetDiscovery(bool discovery, int createNumber)
{
	m_dataMapList[createNumber].discovery = discovery;
}

//戦闘中
void EnemyHP::SetBattle(bool battle, int createNumber)
{
	m_dataMapList[createNumber].battle= battle;
}