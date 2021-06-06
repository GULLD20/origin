#include "../../../pch.h"

#include "LongRangeAttack.h"

#include <GeometricPrimitive.h>
#include <Effects.h>

#include "../../../DeviceResources.h"
#include "../../../Scene/GameContext.h"

//�����蔻��̒��S�����[�̒���(Vector3)
const DirectX::SimpleMath::Vector3 LongRangeAttack::BASE_CENTER_TOP = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.4f);
//�����蔻��̉~�̑傫��
const float LongRangeAttack::CIRCLE_SIZE = 0.2f;

//�R���X�g�E�f�X�g
LongRangeAttack::LongRangeAttack()
	:m_pModel(nullptr)
	, m_speed(0.0f)
	, m_attackRange(0)
	, m_speedCal(0.0f)
{

}

LongRangeAttack::~LongRangeAttack()
{
	if (m_pModel != nullptr)
	{
		m_pModel.reset(nullptr);
	}

	//�v�f����������
	if (m_bulletList.size() > 0)
	{
		m_bulletList.clear();
	}
}

//���f���̍쐬
void LongRangeAttack::CreatModel()
{
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();

	// �G�t�F�N�g�t�@�N�g���̍쐬
	DirectX::EffectFactory* factory = new DirectX::EffectFactory(deviceResources->GetD3DDevice());
	// �e�N�X�`���̓ǂݍ��݃p�X�w��
	factory->SetDirectory(L"Resources/CMO");

	// �t�@�C�����w�肵�ă��f���f�[�^�ǂݍ���
	m_pModel = DirectX::Model::CreateFromCMO(
		deviceResources->GetD3DDevice(),
		L"Resources/CMO/LongRangeAttack.cmo",
		*factory
	);

	delete factory;
}

//����������
void LongRangeAttack::Initialize(float speed,int attackRange)
{
	//�U���͈͂̎w��
	m_attackRange = attackRange;
	//���x�̐ݒ�
	m_speed = speed;
	//�f�[�^�̊m��
	m_bulletList.resize(100);
	//������
	for (int i = 0; i < static_cast<int>(m_bulletList.size()); i++)
	{
		m_bulletList[i].active = false;
		m_bulletList[i].pos = DirectX::SimpleMath::Vector3::Zero;
		m_bulletList[i].startPos = DirectX::SimpleMath::Vector3::Zero;
		m_bulletList[i].capsule3D =
		{
			BASE_CENTER_TOP,
			-BASE_CENTER_TOP,
			CIRCLE_SIZE ,
		};
	}

	//���x�v�Z�p�ϐ��̏�����
	m_speedCal = 0.0f;

	//�Ԃ����J�v�Z���f�[�^(�����蔻��̃f�[�^)�̗v�f�ʒu�̏�����
	m_capsuleSelect = 0;
}

//�X�V����
void LongRangeAttack::Update(float elapsedTime)
{
	m_capsuleSelect = -1;

	for (int i = 0; i < static_cast<int>(m_bulletList.size()); i++)
	{
		if (m_bulletList[i].active)
		{
			//�ړ�����
			Move(i, elapsedTime);
			//�U���I������
			AttackEnd(i);
			//�����蔻��̐ݒ�
			CapsuleDataSetting(i);
			//���[���h�s��̐ݒ�
			MatrixSetting(i);
		}
	}
}

//�`�揈��
void LongRangeAttack::Draw(const DirectX::SimpleMath::Matrix &viewMatrix, const DirectX::SimpleMath::Matrix &projection)
{
	//�`��Ɏg���f�[�^���擾
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();
	ID3D11DeviceContext1* deviceContext = deviceResources->GetD3DDeviceContext();
	DirectX::CommonStates* CommonState = GameContext<DirectX::CommonStates>().Get();

	for (int i = 0; i < static_cast<int>(m_bulletList.size()); i++)
	{
		if (m_bulletList[i].active)
		{
			//���f���̕`��
			m_pModel->Draw(deviceContext, *CommonState, m_bulletList[i].matrix, viewMatrix, projection);
		}
	}
}

//���Z�b�g
void LongRangeAttack::Reset()
{

}

//�U���J�n����
void LongRangeAttack::Shot(const DirectX::SimpleMath::Vector3 &pos, const DirectX::SimpleMath::Matrix &rotation)
{
	for (int i = 0; i < static_cast<int>(m_bulletList.size()); i++)
	{
		if (!m_bulletList[i].active)
		{
			//�g���Ă����ԂɕύX
			m_bulletList[i].active = true;
			//�����ʒu�̎w��
			m_bulletList[i].pos = pos;
			m_bulletList[i].startPos= pos;
			//�p�x�̎w��
			m_bulletList[i].rotation = rotation;
			break;
		}
	}
}

//�U�������������Ȃ�
void LongRangeAttack::HitAttack()
{
	//�U���I������
	m_bulletList[m_capsuleSelect].active = false;
}

//�����蔻���Get
Collision::Capsule3D LongRangeAttack::GetCapsuleData(const DirectX::SimpleMath::Vector3 &playerPos)
{
	//return�ŕԂ��f�[�^���w�肷��
	int select = -1;

	//�x�N�g���̒���
	//����v�Z���ďo������
	float lengthNow = 100.0f;
	//�ł��Z������
	float lengthRet = 100.0f;

	for (int i = 0; i < static_cast<int>(m_bulletList.size()); i++)
	{
		//�g���Ă���Ȃ�
		if (m_bulletList[i].active)
		{
			//�e����v���C���[�Ɍ������x�N�g���̒���
			lengthNow = (playerPos - m_bulletList[i].pos).Length();

			//�v���C���[�Ɉ�ԋ߂��f�[�^���c��
			if (lengthRet > lengthNow)
			{
				lengthRet = lengthNow;
				//return�ŕԂ��f�[�^���w��
				select = i;
			}
		}
	}

	//�����w��ł��Ȃ�������
	if (select == -1)
	{
		return Collision::Capsule3D
		{
			DirectX::SimpleMath::Vector3(0.0f,100.0f,0.0f),
			DirectX::SimpleMath::Vector3(0.0f,100.0f,0.0f),
		    0.0f,
		};
	}

	//�n�����f�[�^�̗v�f�ʒu��ۑ�
	m_capsuleSelect = select;

	//�f�[�^���w�肵�ăf�[�^��Ԃ�
	return m_bulletList[select].capsule3D;
}

//�ړ�����
void LongRangeAttack::Move(int select ,float elapsedTime)
{
	//�ړ��x�N�g��
	DirectX::SimpleMath::Vector3 vec;
	//�ړ��x�N�g���̌v�Z
	vec = DirectX::SimpleMath::Vector3::Transform(
		DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f),
		m_bulletList[select].rotation)
		*m_speed
		*m_speedCal;

	//�ړ�����
	m_bulletList[select].pos += (vec * elapsedTime);
}

//�U���I������
void LongRangeAttack::AttackEnd(int select)
{
	//�e�̔��ˈʒu���猻�݂̈ʒu�܂ł̃x�N�g���̒���
	float length = (m_bulletList[select].pos - m_bulletList[select].startPos).Length();

	//�e�̔򋗗������E�𒴂�����
	if (length >= m_attackRange)
	{
		//�U���I������
		m_bulletList[select].active = false;
	}
}

//�����蔻��̉�]
void LongRangeAttack::CapsuleDataSetting(int select)
{
	//�����蔻��̑傫����ݒ�
	DirectX::SimpleMath::Vector3 center_top =
		DirectX::SimpleMath::Vector3::Transform(BASE_CENTER_TOP, m_bulletList[select].rotation);

	m_bulletList[select].capsule3D =
	{
		m_bulletList[select].pos + center_top,
		m_bulletList[select].pos - center_top,
		CIRCLE_SIZE,
	};
}

//�s��̐ݒ�
void LongRangeAttack::MatrixSetting(int select)
{
	//�p�x�̐ݒ�
	m_bulletList[select].matrix = m_bulletList[select].rotation;
	//�ʒu�̐ݒ�
	m_bulletList[select].matrix *= DirectX::SimpleMath::Matrix::CreateTranslation(m_bulletList[select].pos);
}