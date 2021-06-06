#include "../../../../pch.h"
#include "Bow.h"

#include "../../DataManager/DataManager.h"

//����̃T�C�Y
const DirectX::SimpleMath::Vector3 Bow::SIZE = DirectX::SimpleMath::Vector3(0.2f, 0.2f, 0.2f);

//����̊�{�ʒu(�ړ���)
const DirectX::SimpleMath::Vector3 Bow::BASE_POSITION = DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f);

// ����̊�{�ʒu(�U����)
const DirectX::SimpleMath::Vector3 Bow::BASE_POSITION_ATTACK = DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f);

//�R���{�̐�
const int Bow::MAX_COMBO = 3;
//�U������
const float Bow::ATTACK_FLAME = 10.0f;

//����̉�](�R���{)
const std::vector<DirectX::SimpleMath::Vector3> Bow::ANGLE=
{
	DirectX::SimpleMath::Vector3(0.0f,0.0f,0.0f),
	DirectX::SimpleMath::Vector3(0.0f,0.0f,0.0f),
	DirectX::SimpleMath::Vector3(0.0f,0.0f,0.0f) ,
	DirectX::SimpleMath::Vector3(0.0f,0.0f,0.0f)
};

Bow::Bow()
{
}


Bow::~Bow()
{
}

void Bow::Create(DX::DeviceResources* &DeviceResources)
{
	// �G�t�F�N�g�t�@�N�g���̍쐬
	DirectX::EffectFactory * factory = new DirectX::EffectFactory(DeviceResources->GetD3DDevice());
	// �e�N�X�`���̓ǂݍ��݃p�X�w��
	factory->SetDirectory(L"Resources/CMO");

	// �t�@�C�����w�肵�ă��f���f�[�^�ǂݍ���
	m_pModel = DirectX::Model::CreateFromCMO(
		DeviceResources->GetD3DDevice(),
		L"Resources/CMO/testSword.cmo",
		*factory
	);

	delete factory;
}

//����������
void Bow::Initialize()
{
	m_weapon.attack = false;
	m_weapon.combo = 0;
	m_weapon.angle = ANGLE[m_weapon.combo];
	m_weapon.flame = 0.0f;
	m_weapon.vector = DirectX::SimpleMath::Vector3::Zero;
	m_weapon.betweenAttacksTime = 0.0f;
	m_weapon.afterTheAttack = false;
}

//�X�V����
void Bow::Update(float elapsedSeconds)
{
	MatrixSetting();

	//�U������(��]�p�x)
	if (m_weapon.attack)
	{
		//�U�����[�V��������
		Attack();

		//�U����̏��
		if (!m_weapon.attack)
		{
			m_weapon.afterTheAttack = true;
		}
	}
	//�U����
	if (m_weapon.afterTheAttack)
	{
		//�U����̈�莞�Ԍ�(���݂�0.5�b)
		if (m_weapon.betweenAttacksTime >= BETWEENATTACKSTIME)
		{
			ResetWeaponData();
		}
		m_weapon.betweenAttacksTime += elapsedSeconds;
	}
}

//�`�揈��
void Bow::Draw(ID3D11DeviceContext1* context, const DirectX::CommonStates& State, const DirectX::SimpleMath::Matrix &viewMatrix, const DirectX::SimpleMath::Matrix &proj)
{
	// ���f���`��
	m_pModel->Draw(context, State, m_weapon.matrix, viewMatrix, proj);

}

//���Z�b�g
void Bow::Reset()
{
}

//�U���J�n
void Bow::StartAttack()
{
	if (m_weapon.afterTheAttack)
	{
		m_weapon.combo++;
	}

	m_weapon.attack = true;
	m_weapon.afterTheAttack = false;
}

//�U���I��
void Bow::EndAttack()
{
	//�U�����[�V�������I��������
	if (m_weapon.flame >= 1.0f)
	{
		//�U���I������
		m_weapon.flame = 0.0f;
		m_weapon.attack = false;
		m_weapon.afterTheAttack = true;

		if (m_weapon.combo >= MAX_COMBO)
		{
			m_weapon.combo = 0;
		}
	}
}

//�U�������擾
bool Bow::GetAttackSTate() const
{
	return m_weapon.attack;
}

//Matrix��ݒ肷��
void Bow::MatrixSetting()
{
	//��]�̂��߂P��ɂ��炷
	m_weapon.matrix = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f));

	//��]����
	m_weapon.matrix *= DirectX::SimpleMath::Matrix::CreateRotationX(m_weapon.angle.x);
	m_weapon.matrix *= DirectX::SimpleMath::Matrix::CreateRotationY(m_weapon.angle.y);
	m_weapon.matrix *= DirectX::SimpleMath::Matrix::CreateRotationZ(m_weapon.angle.z);

	//�x�N�g�����o��
	DirectX::SimpleMath::Matrix vecMatrix;

	vecMatrix = DirectX::SimpleMath::Matrix::CreateRotationX(m_weapon.angle.x);
	vecMatrix *= DirectX::SimpleMath::Matrix::CreateRotationY(m_weapon.angle.y);
	vecMatrix *= DirectX::SimpleMath::Matrix::CreateRotationZ(m_weapon.angle.z);

	m_weapon.vector = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f), vecMatrix);

	//����̈ʒu�̒���
	m_weapon.matrix *= DirectX::SimpleMath::Matrix::CreateTranslation(BASE_POSITION_ATTACK);
	//�v���C���[�̈ʒu�ɒ���
	m_weapon.matrix *= m_playerMatrix;
}

void Bow::ResetWeaponData()
{
	m_weapon.combo = 0;
	m_weapon.angle = ANGLE[m_weapon.combo];
	m_weapon.flame = 0.0f;
	m_weapon.attack = false;
	m_weapon.betweenAttacksTime = 0.0f;
	m_weapon.afterTheAttack = false;
}

//�U������(��])
void Bow::Attack()
{
	//�ő�R���{�����傫���ꍇ�R���{���ŏ��ɖ߂�
	if (m_weapon.combo + 1 == MAX_COMBO)
	{
		m_weapon.angle = DirectX::SimpleMath::Vector3::Lerp(ANGLE[m_weapon.combo], ANGLE[0], m_weapon.flame);
	}
	else
	{
		m_weapon.angle = DirectX::SimpleMath::Vector3::Lerp(ANGLE[m_weapon.combo], ANGLE[m_weapon.combo + 1], m_weapon.flame);
	}

	m_weapon.flame += 1.0f / ATTACK_FLAME;

	EndAttack();
}