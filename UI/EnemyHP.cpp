#include "../pch.h"
#include "EnemyHP.h"

#include <SpriteBatch.h>
#include <string>

#include "../Scene/GameContext.h"

//�摜�̑傫��
const int EnemyHP::TEXTUER_SIZE_X             = 142;
const int EnemyHP::TEXTUER_SIZE_Y             = 17;

//�ʒu�̊
const DirectX::SimpleMath::Vector2 EnemyHP::BASE_POS  = DirectX::SimpleMath::Vector2(10.0f, 450.0f);
//HP�Q�[�W�̈ʒu�v�Z�p
const DirectX::SimpleMath::Vector2 EnemyHP::HP_POS_CAL = DirectX::SimpleMath::Vector2(44.0f, 40.6f);
//�c�̈ʒu�̌v�Z�p
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

//����
void EnemyHP::Create(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	//�摜�̓ǂݍ���
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/HPGuage/Enemy/EnemyHPGuage.png", NULL, m_pHpGuage.ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/HPGuage/Enemy/Back.png", NULL, m_pBack.ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/HPGuage/Enemy/Discovery.png", NULL, m_pDiscovery.ReleaseAndGetAddressOf());

	m_pName = std::make_unique<DrawName>();
	m_pName->Create(device);
}

//������
void EnemyHP::Initialize(float maxHp, const std::string& name, int createNumber)
{
	//�G��HP������
	m_dataMapList[createNumber].hp = 0.0f;
	//HP�̍ő�l������
	m_dataMapList[createNumber].maxHp = maxHp;
	//�����Ă��邩
	m_dataMapList[createNumber].alive = true;
	//�摜�̕\���T�C�Y
	m_dataMapList[createNumber].textureSize =
	{
		0,
		0,
		TEXTUER_SIZE_X,
		TEXTUER_SIZE_Y,
	};
	//�G�̖��O������
	m_dataMapList[createNumber].name = name;

	m_pName->Initialize();
}

// �X�V
void EnemyHP::Update(float hp, int createNumber)
{
	m_setingPos = 0;

	//�G��HP�̎擾
	m_dataMapList[createNumber].hp = hp;

	//�G��HP�̂P���̒l�����߂�
	float onePercent = m_dataMapList[createNumber].maxHp * 0.01f;

	//���݂̓G��HP�������c���Ă��邩���߂�
	float RemainingGuage = m_dataMapList[createNumber].hp / onePercent;

	//�摜�̉��̑傫���̂P�������߂�
	float textuerSizeOnePercent = TEXTUER_SIZE_X * 0.01f;

	//���߂�textuerSizeOnePercent��RemainingGuage���������摜���ǂꂭ�炢�\�����邩���߂�(X)
	int sizeX = static_cast<int>(textuerSizeOnePercent * RemainingGuage);

	//�摜�̑傫���̐ݒ�
	m_dataMapList[createNumber].textureSize =
	{
		0,
		0,
		sizeX,
		TEXTUER_SIZE_Y,
	};
}

//�`��
void EnemyHP::Draw(int createNumber,DirectX::CommonStates& state)
{
	//�摜�̕\��
	for (int i =0;i<static_cast<int>(m_dataMapList.size());i++)
	{
		if (m_dataMapList[createNumber].alive)
		{
			if (i == createNumber)
			{
				if (m_dataMapList[createNumber].discovery)
				{
					//���̍����摜��\��
					GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pBack.Get(), DirectX::SimpleMath::Vector2(BASE_POS.x , BASE_POS.y + (m_setingPos*POS_CAL_Y)));
					//���O�̕\��
					m_pName->Draw(m_dataMapList[createNumber].name, DirectX::SimpleMath::Vector2(BASE_POS.x, BASE_POS.y + (m_setingPos*POS_CAL_Y)), state);
					//HP�Q�[�W�̕\��
					GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pHpGuage.Get(), DirectX::SimpleMath::Vector2((BASE_POS.x + HP_POS_CAL.x), ((BASE_POS.y + HP_POS_CAL.y) + (m_setingPos*POS_CAL_Y))), &m_dataMapList[createNumber].textureSize);

					//������Ԃ̕\��
					if (m_dataMapList[createNumber].battle)
					{
						GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pDiscovery.Get(), DirectX::SimpleMath::Vector2(BASE_POS.x, BASE_POS.y + (m_setingPos*POS_CAL_Y)));
					}

					//�����̓G�̏ꍇ�ʒu�����炷���߂�++����
					m_setingPos++;
				}
			}
		}
	}
}

//���Z�b�g
void EnemyHP::Reset()
{

}

//�����Ă��邩�̃f�[�^������
void EnemyHP::SetAlive(bool alive, int createNumber)
{
	m_dataMapList[createNumber].alive = alive;
}

//��������Ă��邩�̃f�[�^������
void EnemyHP::SetDiscovery(bool discovery, int createNumber)
{
	m_dataMapList[createNumber].discovery = discovery;
}

//�퓬��
void EnemyHP::SetBattle(bool battle, int createNumber)
{
	m_dataMapList[createNumber].battle= battle;
}