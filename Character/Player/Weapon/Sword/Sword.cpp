#include "../../../../pch.h"
#include "Sword.h"
#include "../../DataManager/DataManager.h"

//���̃T�C�Y
const  DirectX::SimpleMath::Vector3 Sword::SIZE = DirectX::SimpleMath::Vector3(0.2f, 1.5f, 0.2f);

//���̊�{�ʒu(�ړ���)
const DirectX::SimpleMath::Vector3 Sword::BASE_POSITION = DirectX::SimpleMath::Vector3(0.7f, 1.0f, -0.5f);

//���̊�{�ʒu(�U����)
const DirectX::SimpleMath::Vector3 Sword::BASE_POSITION_ATTACK = DirectX::SimpleMath::Vector3(0.7f, 0.0f, -0.5f);

//�R���{�̐�
const int Sword::MAX_COMBO = 4;
//�U������
const float Sword::ATTACK_FLAME =10.0f;

//����̉�](�R���{)
const std::vector<DirectX::SimpleMath::Vector3> Sword::ANGLE =
{

	//��
	DirectX::SimpleMath::Vector3(0.0f,(DirectX::XM_PI* 0.5f),-DirectX::XM_PI* 0.3f),

	DirectX::SimpleMath::Vector3(-DirectX::XM_PI,(DirectX::XM_PI* 0.5f),-DirectX::XM_PI* 0.5f),

	DirectX::SimpleMath::Vector3(0.0f,(DirectX::XM_PI* 0.5f),-(DirectX::XM_PI* 0.5f)) ,

	DirectX::SimpleMath::Vector3(-DirectX::XM_PI* 3.0f,(DirectX::XM_PI* 0.5f),(-DirectX::XM_PI* 0.5f))
};

Sword::Sword()
{
}


Sword::~Sword()
{
	if (m_pSlashEffect != nullptr)
	{
		m_pSlashEffect.reset(nullptr);
	}
}

void Sword::Create(DX::DeviceResources* &DeviceResources)
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

	m_pSlashEffect = std::make_unique<SlashEffect>();
	m_pSlashEffect->Creat();
}

//����������
void Sword::Initialize()
{
	m_weapon.attack = false;
	m_weapon.combo = 0;
	m_weapon.angle = ANGLE[m_weapon.combo];
	m_weapon.flame = 0.0f;
	m_weapon.vector = DirectX::SimpleMath::Vector3::Zero;
	m_weapon.betweenAttacksTime = 0.0f;
	m_weapon.afterTheAttack = false;

	m_pSlashEffect->Initialzie();
}

//�X�V����
void Sword::Update(float elapsedSeconds)
{
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

	m_pSlashEffect->SetWeaponMatrix(m_weapon.matrix);
	m_pSlashEffect->Update();
}

//�`�揈��
void Sword::Draw(ID3D11DeviceContext1* context, const DirectX::CommonStates& State, const DirectX::SimpleMath::Matrix &viewMatrix, const DirectX::SimpleMath::Matrix &proj)
{
	// ���f���`��
	m_pModel->Draw(context, State, m_weapon.matrix, viewMatrix, proj);

	if (m_weapon.attack)
	{
		//�G�t�F�N�g�̕`��
		m_pSlashEffect->Draw(viewMatrix);
	}
}

//���Z�b�g
void Sword::Reset()
{
}

//�U���J�n
void Sword::StartAttack()
{
	if (m_weapon.afterTheAttack)
	{
		m_weapon.combo++;

		if (m_weapon.combo >= MAX_COMBO)
		{
			m_weapon.combo = 0;
		}
	}

	m_weapon.attack = true;
	m_weapon.afterTheAttack = false;
}

//�U���I��
void Sword::EndAttack()
{
	//�U�����[�V�������I��������
	if (m_weapon.flame >= 1.0f)
	{
		//�U���I������
		m_weapon.flame = 0.0f;
		m_weapon.attack = false;
		m_weapon.afterTheAttack = true;
		m_weapon.betweenAttacksTime = 0.0f;
	}
}

//�U�������擾
bool Sword::GetAttackSTate() const
{
	return m_weapon.attack;
}

//Matrix��ݒ肷��
void Sword::MatrixSetting()
{
	m_weapon.matrix = DirectX::SimpleMath::Matrix::CreateRotationY(ANGLE[m_weapon.combo].y);

	//����̈ʒu�̒���
	m_weapon.matrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, 2.0f, 0.0f));

	//��]����
	m_weapon.matrix *= DirectX::SimpleMath::Matrix::CreateRotationX(m_weapon.angle.x);
	m_weapon.matrix *= DirectX::SimpleMath::Matrix::CreateRotationZ(ANGLE[m_weapon.combo].z);

	//�x�N�g�����o��
	DirectX::SimpleMath::Matrix vecMatrix;

	vecMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(ANGLE[m_weapon.combo].y);
	vecMatrix *= DirectX::SimpleMath::Matrix::CreateRotationX(m_weapon.angle.x);
	vecMatrix *= DirectX::SimpleMath::Matrix::CreateRotationZ(ANGLE[m_weapon.combo].z);

	m_weapon.vector= DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f), vecMatrix);


	//�v���C���[�̈ʒu�ɒ���
	m_weapon.matrix *= m_playerMatrix;
}

void Sword::ResetWeaponData()
{
	m_weapon.combo = 0;
	m_weapon.angle = ANGLE[m_weapon.combo];
	m_weapon.flame = 0.0f;
	m_weapon.attack = false;
	m_weapon.betweenAttacksTime = 0.0f;
	m_weapon.afterTheAttack = false;
}

//�U������(��])
void Sword::Attack()
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

	if (m_weapon.combo <= 1)
	{
		m_weapon.flame += 1.0f / ATTACK_FLAME;
	}
	else
	{
		m_weapon.flame += 1.0f / (ATTACK_FLAME * 2.0f);
	}

	EndAttack();
}