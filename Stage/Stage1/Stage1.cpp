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
	//�}�b�v�̐���
	Map* map = GameContext<Map>::Get();
	map->Reset();
	map->SetMapData(m_mapDataList);
	map->Initialize();

	//�쐬�����G�l�~�[�̐��̏�����
	m_creatEnemyNum = 0;

	//�p����ʒu�����߂邽��
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
					//�G�l�~�[�̍쐬(-2����̂�EnemyTyp��0���琔�l���w�肷��̂ɑ΂���csv�̒l��2�ȏゾ����)
					CreatEnemy((static_cast<Params::EnemyTyp>(m_characterData[i][j] - 2)), DirectX::SimpleMath::Vector3(x, 0.0f, z));
				}
				if (m_characterData[i][j] == 1)
				{
					//�v���C���[�̍쐬�A������
					PlayerInitialize(DirectX::SimpleMath::Vector3(x, 0.0f, z));
				}
			}
			x++;
		}
		x = -(m_mapDataList.size()*0.5f);;
		z++;
	}

	//�G�l�~�[�̏�����
	EnemyInitialize();

	//�ˉe�s��
	Projection* projection = GameContext<Projection>().Get();

	//�J�����쐬
	m_pFollowCamera = std::make_unique<FollowCamera>();
	m_pFollowCamera->Initialize();
	m_pFollowCamera->SetProjection(projection->GetMatrix());
}

void Stage1::Update(const DX::StepTimer &timer)
{
	//�G�̍X�V����
	EnemyUpdate(timer);
	//�v���C���[�̍X�V����
	PlayerUpdate(timer);
	m_pFollowCamera->SetTargetPosition(m_pPlayer->GetPos());

	//�J�����̈ʒu�̐ݒ�v�Z
	Cal_Set_EysPos();
	//�J�����̍X�V����
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
	//�}�b�v
	GameContext<Map>::Get()->Draw(m_pFollowCamera->GetView());

	// �I�u�W�F�N�g�̕`��
	m_pPlayer->Draw(m_pFollowCamera->GetView());

	for (int i = 0; i <static_cast<int>(m_pEnemy.size()); i++)
	{
		m_pEnemy[i]->Draw(m_pFollowCamera->GetView());
	}
}

void Stage1::Reset()
{

}

//�v���C���[�̏���������
void Stage1::PlayerInitialize(const DirectX::SimpleMath::Vector3 &pos)
{
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();

	//�v���C���[�̍쐬
	m_pPlayer = std::make_unique<Player>(pos, Params::PLAYER_ATTACK, Params::PLAYER_HP, Params::PLAYER_SPEED, Params::PLAYER_RANGE);
	m_pPlayer->Create(deviceResources, WeaponType::Sword);
	//�v���C���[�̏�����
	m_pPlayer->Initialize();
}

//�v���C���[�̍X�V����
void Stage1::PlayerUpdate(const DX::StepTimer &timer)
{
	//�v���C���[�̍X�V
	m_pPlayer->Update(timer);

	//�G���ƂɊm�F
	for (int i = 0; i < static_cast<int>(m_pEnemy.size()); i++)
	{
		//�G�ɍU�����������Ă��邩
		m_pPlayer->HitAttack(m_pEnemy[i]->GetID());
		//�G�ƃv���C���[���������Ă��邩
		m_pPlayer->HItEnemy(m_pEnemy[i]->GetID());
	}

	//Matrix�̐ݒ�
	m_pPlayer->MatrixSetting();
}

//�G�l�~�[�̏���������
void Stage1::EnemyInitialize()
{
	//�G�̏�����
	for (int i = 0; i <static_cast<int>(m_pEnemy.size()); i++)
	{
		m_pEnemy[i]->CreateEffect();
		m_pEnemy[i]->Initialize();
		m_pEnemy[i]->EffectInitialize();
		//�v���C���[��ID��ݒ�
		m_pEnemy[i]->SetPlayerID(m_pPlayer->GetID());
		//�S�Ă̓G��ݒ�
		m_pEnemy[i]->SetOtherEnemy(m_pEnemy, m_pEnemy.size());
	}

	m_DethEnemyNum = 0;
}

//�G�l�~�[�̍X�V����
void Stage1::EnemyUpdate(const DX::StepTimer &timer)
{
	for (int i = 0; i <static_cast<int>(m_pEnemy.size()); i++)
	{
		//�`��͈͓����m�F
		m_pEnemy[i]->CameraWithinRange();
		//�G�̍X�V����
		m_pEnemy[i]->Update(timer);
	}
}

//�J�����̈ʒu�̌v�Z�Ɛݒ�
void Stage1::Cal_Set_EysPos()
{
	//�J�����̈ʒu�����߂�
	DirectX::SimpleMath::Vector3 eysPos = m_pPlayer->GetPos() + DirectX::SimpleMath::Vector3(0.0f, 8.0f, 12.5f);

	//m_debugCamera->update();
	//�J�����̈ʒu��ݒ�
	m_pFollowCamera->SetEyePosition(eysPos);

	for (int i = 0; i < static_cast<int>(m_pEnemy.size()); i++)
	{
		//�J�����̈ʒu�̐ݒ�
		m_pEnemy[i]->SetEysPos(eysPos);
	}
}