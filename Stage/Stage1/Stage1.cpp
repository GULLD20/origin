#include "../../pch.h"
#include "Stage1.h"

#include "../../DeviceResources.h"
#include "../../Scene/GameContext.h"
#include "../../Scene/Projection.h"
#include "../../DataManager/DataManager.h"

#include "../../Map/Map.h"

Stage1::Stage1()
{
}


Stage1::~Stage1()
{
}

void Stage1::Initialize()
{
	//マップの生成
	Map* map = GameContext<Map>::Get();
	map->Reset();
	map->SetMapData(m_mapDataList);
	map->Initialize();

	//作成したエネミーの数の初期化
	m_creatEnemyNum = 0;

	//角から位置を決めるため
	float x = -(m_mapDataList.size()*0.5f);
	float z = -(m_mapDataList[0].size()*0.5f);

	for (int i = 0; i < static_cast<int>(m_characterData.size()); i++)
	{
		for (int j = 0; j <static_cast<int>(m_characterData[i].size()); j++)
		{
			if (m_characterData[i][j] != 0)
			{
				if (m_characterData[i][j] > 1)
				{
					//エネミーの作成(-2するのはEnemyTypが0から数値を指定するのに対してcsvの値は2以上だから)
					CreatEnemy((static_cast<Params::EnemyTyp>(m_characterData[i][j] - 2)), DirectX::SimpleMath::Vector3(x, 0.0f, z));
				}
				if (m_characterData[i][j] == 1)
				{
					//プレイヤーの作成、初期化
					PlayerInitialize(DirectX::SimpleMath::Vector3(x, 0.0f, z));
				}
			}
			x++;
		}
		x = -(m_mapDataList.size()*0.5f);;
		z++;
	}

	//エネミーの初期化
	EnemyInitialize();

	//射影行列
	Projection* projection = GameContext<Projection>().Get();

	//カメラ作成
	m_pFollowCamera = std::make_unique<FollowCamera>();
	m_pFollowCamera->Initialize();
	m_pFollowCamera->SetProjection(projection->GetMatrix());
}

void Stage1::Update(const DX::StepTimer &timer)
{
	//敵の更新処理
	EnemyUpdate(timer);
	//プレイヤーの更新処理
	PlayerUpdate(timer);
	m_pFollowCamera->SetTargetPosition(m_pPlayer->GetPos());

	//カメラの位置の設定計算
	Cal_Set_EysPos();
	//カメラの更新処理
	m_pFollowCamera->Update();

	DirectX::Keyboard& key = DirectX::Keyboard::Get();
	DirectX::Keyboard::State keyState = key.GetState();

	if (keyState.IsKeyDown(DirectX::Keyboard::G))
	{
		m_pFollowCamera->StartShake(DirectX::SimpleMath::Vector3(0.0f, 3.0f, 0.0f));
	}

}

void Stage1::Render()
{
	//マップ
	GameContext<Map>::Get()->Draw(m_pFollowCamera->GetView());

	// オブジェクトの描画
	m_pPlayer->Draw(m_pFollowCamera->GetView());

	for (int i = 0; i <static_cast<int>(m_pEnemy.size()); i++)
	{
		m_pEnemy[i]->Draw(m_pFollowCamera->GetView());
	}
}

void Stage1::Reset()
{

}

//プレイヤーの初期化処理
void Stage1::PlayerInitialize(const DirectX::SimpleMath::Vector3 &pos)
{
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();

	//プレイヤーの作成
	m_pPlayer = std::make_unique<Player>(pos, Params::PLAYER_ATTACK, Params::PLAYER_HP, Params::PLAYER_SPEED, Params::PLAYER_RANGE);
	m_pPlayer->Create(deviceResources, WeaponType::Sword);
	//プレイヤーの初期化
	m_pPlayer->Initialize();
}

//プレイヤーの更新処理
void Stage1::PlayerUpdate(const DX::StepTimer &timer)
{
	//プレイヤーの更新
	m_pPlayer->Update(timer);

	//敵ごとに確認
	for (int i = 0; i < static_cast<int>(m_pEnemy.size()); i++)
	{
		//敵に攻撃が当たっているか
		m_pPlayer->HitAttack(m_pEnemy[i]->GetID());
		//敵とプレイヤーが当たっているか
		m_pPlayer->HItEnemy(m_pEnemy[i]->GetID());
	}

	//Matrixの設定
	m_pPlayer->MatrixSetting();
}

//エネミーの初期化処理
void Stage1::EnemyInitialize()
{
	//敵の初期化
	for (int i = 0; i <static_cast<int>(m_pEnemy.size()); i++)
	{
		m_pEnemy[i]->CreateEffect();
		m_pEnemy[i]->Initialize();
		m_pEnemy[i]->EffectInitialize();
		//プレイヤーのIDを設定
		m_pEnemy[i]->SetPlayerID(m_pPlayer->GetID());
		//全ての敵を設定
		m_pEnemy[i]->SetOtherEnemy(m_pEnemy, m_pEnemy.size());
	}

	m_DethEnemyNum = 0;
}

//エネミーの更新処理
void Stage1::EnemyUpdate(const DX::StepTimer &timer)
{
	for (int i = 0; i <static_cast<int>(m_pEnemy.size()); i++)
	{
		//描画範囲内か確認
		m_pEnemy[i]->CameraWithinRange();
		//敵の更新処理
		m_pEnemy[i]->Update(timer);
	}
}

//カメラの位置の計算と設定
void Stage1::Cal_Set_EysPos()
{
	//カメラの位置を求める
	DirectX::SimpleMath::Vector3 eysPos = m_pPlayer->GetPos() + DirectX::SimpleMath::Vector3(0.0f, 8.0f, 12.5f);

	//m_debugCamera->update();
	//カメラの位置を設定
	m_pFollowCamera->SetEyePosition(eysPos);

	for (int i = 0; i < static_cast<int>(m_pEnemy.size()); i++)
	{
		//カメラの位置の設定
		m_pEnemy[i]->SetEysPos(eysPos);
	}
}