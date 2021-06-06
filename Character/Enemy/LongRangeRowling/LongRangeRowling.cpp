#include "../../../pch.h"

#include "LongRangeRowling.h"

#include "../../DataManager/DataManager.h"
#include "../../List/CharacterMapList.h"

#include "../../Map/Map.h"

//�U������
const float LongRangeRowling::ATTACK_TIME = 0.25f;
//�U���͈�
const float LongRangeRowling::ATTACKRANGE = 20.0f;
//�U���Ԋu
const float LongRangeRowling::ATTACK_INTERVAL = 0.25f;
//�퓬�͈�
const float LongRangeRowling::BATTLE_RANGE = 20.0f;
//�e�̍U�����x(1�b�Ԃ̑��x)
const float LongRangeRowling::ATTACK_SPEED = 15.0f;

//�v���C���[�𐁂���΂���(1�b�Ԃ�)
const float LongRangeRowling::BLOW_AWAY_POWER_PLAYER_TARGET = 5.0f;

//�G�t�F�N�g�̌�
//�U���͈�
const int LongRangeRowling::NUMBER_OF_EFFECT_ATTACK_AREA = 1;
//�U��������������
const int LongRangeRowling::NUMBER_OF_EFFECT_HIT = 25;
//���S
const int LongRangeRowling::NUMBER_OF_EFFECT_DETH = 50;

//�X���E��Ԃ̌v�Z�p�̐��l
const float LongRangeRowling::SLOW_CAL = 0.025f;
//�ʏ��Ԃ̌v�Z�p�̐��l
const float LongRangeRowling::NORMAL_CAL = 1.0f;

LongRangeRowling::LongRangeRowling(const DirectX::SimpleMath::Vector3 &pos, float attack, float hp, float speed, float thisRange)
	:Enemy(pos, attack, hp, speed, thisRange)
{
}


LongRangeRowling::~LongRangeRowling()
{
	if (m_pAttackLine != nullptr)
	{
		m_pAttackLine->Lost();
		m_pAttackLine.reset(nullptr);
	}

	if (m_pLongRangeAttack != nullptr)
	{
		m_pLongRangeAttack.reset(nullptr);
	}
}

//�G�t�F�N�g�̍쐬
void LongRangeRowling::CreateEffect()
{
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();

	//HP�Q�[�W�̍쐬
	m_pHPGuage = std::make_unique<Effect3D>();
	//�摜�̐ݒ�
	m_pHPGuage->Create(deviceResources, L"Resources/Textures/HPGuage/Player/HP.png");

	//���O�̍쐬
	m_pName = std::make_unique<Effect3D>();
	//�摜�̐ݒ�
	m_pName->Create(deviceResources, L"Resources/Textures/Enemies/�S�u����.png");

	//�U���͈͂̍쐬
	m_pAttackLine = std::make_unique<AttackLine>();
	//�摜�ƃG�t�F�N�g�̍쐬���̐ݒ�
	m_pAttackLine->Create(deviceResources, NUMBER_OF_EFFECT_ATTACK_AREA);

	//�U���������������̉��o(�v���C���[�̍U�����G�l�~�[�ɓ���������)�̍쐬
	m_pEnemyHit = std::make_unique<EnemyHit>();
	m_pEnemyHit->Create(deviceResources, NUMBER_OF_EFFECT_HIT);

	//���S���o�̍쐬
	m_pEnemyDeth = std::make_unique<EnemyDeth>();
	//�摜�ƃG�t�F�N�g�̍쐬���̐ݒ�
	m_pEnemyDeth->Create(deviceResources, NUMBER_OF_EFFECT_DETH);
}

//�h���N���X�p�X�V����
void LongRangeRowling::SubInitialize()
{
	m_pLongRangeAttack = std::make_unique<LongRangeAttack>();
	//�U���̃��f���쐬
	m_pLongRangeAttack->CreatModel();
	//�U���̏���������
	m_pLongRangeAttack->Initialize(ATTACK_SPEED,static_cast<int>(BATTLE_RANGE));
	//��]�p�x�̏�����
	m_angle = 1.0f;
	//�G�l�~�[�̐�����є���̏�����
	m_blowAway = false;

	//��]�p�x
	m_anglY = 0.0f;
}

//�G�t�F�N�g�̏�����
void LongRangeRowling::EffectInitialize()
{
	//HP�̉摜�̑傫���̐ݒ�
	m_pHPGuage->SetTextureScaleTL(DirectX::SimpleMath::Vector2(1.0f, 0.06f));
	m_pHPGuage->SetTextureScaleBR(DirectX::SimpleMath::Vector2(-1.0f, -0.06f));
	//HP�̉摜�̐؂���ʒu��ݒ�
	m_pHPGuage->SetTextureSizeTL(DirectX::SimpleMath::Vector2(0.0f, 0.0f));
	m_pHPGuage->SetTextureSizeBR(DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	//���O�̉摜�̑傫���̐ݒ�
	m_pName->SetTextureScaleTL(DirectX::SimpleMath::Vector2(0.60f, 0.15f));
	m_pName->SetTextureScaleBR(DirectX::SimpleMath::Vector2(-0.60f, -0.15f));
	//���O�̉摜�̐؂���ʒu��ݒ�
	m_pName->SetTextureSizeTL(DirectX::SimpleMath::Vector2(0.0f, 0.0f));
	m_pName->SetTextureSizeBR(DirectX::SimpleMath::Vector2(1.0f, 0.5f));

	m_pAttackLine->InitializeNormal(5, DirectX::SimpleMath::Vector3(0, 0, 0));
	DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(0.15f, 1.0f, (ATTACKRANGE * 0.5f));

	m_pAttackLine->SetScale(scale);

	//�U���������������̉��o(�v���C���[�̍U�����G�l�~�[�ɓ���������)�̏�����
	m_pEnemyHit->InitializeNormal(1, DirectX::SimpleMath::Vector3::Zero);
	m_pEnemyHit->SetScale(DirectX::SimpleMath::Vector3(0.2f, 0.2f, 0.2f));

	//���S���o�̏�����
	m_pEnemyDeth->InitializeCorn(10.0f, DirectX::SimpleMath::Vector3(0.0f, -0.25f, 0.0f), DirectX::SimpleMath::Vector3(0.0f, 0.1f, 0.0f));
	m_pEnemyDeth->SetScale(DirectX::SimpleMath::Vector3(0.2f, 0.2f, 0.2f));
}


//�eUpdate�֐�-----------------
//�h���N���X�p�X�V����
void LongRangeRowling::SubUpdate()
{
	//��]����
	m_anglY += m_angle*m_elapsedTime *m_speedCal;
	//���̃N���X���ł̂݉�]�����邽��
	m_charaData.angle.y = m_anglY;

	//�U��(�e)�̍X�V����
	m_pLongRangeAttack->SetSpeedCal(m_speedCal);
	m_pLongRangeAttack->Update(m_elapsedTime);
	//�e���G�ɓ���������
	HitAttack();
}

//�G�t�F�N�g�̍X�V����
void LongRangeRowling::EffectUpdate(const DX::StepTimer &timer)
{
	if (DethUpdate(timer))
	{
		return;
	}

	HPTextureSize();
	//HP�Q�[�W�̍X�V
	m_pHPGuage->RotationY(m_charaData.pos + DirectX::SimpleMath::Vector3(0.0f, 0.5f, 0.0f), m_eyePos);
	m_pName->RotationY(m_charaData.pos + DirectX::SimpleMath::Vector3(0.0f, 0.7f, 0.0f), m_eyePos);

	m_pAttackLine->Update(timer);

	if (m_charaData.state == State::Battle)
	{
		m_pAttackLine->SetPos(m_charaData.pos
			+ DirectX::SimpleMath::Vector3::Transform(
				DirectX::SimpleMath::Vector3::Forward*(ATTACKRANGE * 0.5f)
				, m_charaData.rotation));
	}

	m_pAttackLine->SetMainMatrix(m_charaData.rotation);

	//�����Ȃ�X�V���Ȃ�
	if (m_hitEffectAlpha > 0.0f)
	{
		m_hitEffectAlpha -= m_elapsedTime;
		m_pEnemyHit->Update(timer);
	}

	//�摜�̐؂���ʒu��ݒ�
	if (!m_discovery)
	{
		m_pName->SetTextureSizeTL(DirectX::SimpleMath::Vector2(0.0f, 0.0f));
		m_pName->SetTextureSizeBR(DirectX::SimpleMath::Vector2(1.0f, 0.5f));
	}
	else
	{
		m_pName->SetTextureSizeTL(DirectX::SimpleMath::Vector2(0.0f, 0.5f));
		m_pName->SetTextureSizeBR(DirectX::SimpleMath::Vector2(1.0f, 1.0f));
	}
}

//-----------------------------

//�e�`�揈��-------------------
//�������̕`�揈��
void LongRangeRowling::AliveDraw(const DirectX::SimpleMath::Matrix &viewMatrix, const DirectX::SimpleMath::Matrix &projection)
{
	//�`��Ɏg���f�[�^���擾
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();
	ID3D11DeviceContext1* deviceContext = deviceResources->GetD3DDeviceContext();
	DirectX::CommonStates* CommonState = GameContext<DirectX::CommonStates>().Get();

	// ���f���`��
	//�e
	DirectX::SimpleMath::Matrix shadowMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_charaData.pos + DirectX::SimpleMath::Vector3(0.0f, -1.0f, 0.0f));
	m_shadow->Draw(deviceContext, *CommonState, shadowMatrix, viewMatrix, projection);

	//�L�����N�^�[
	m_pModel->Draw(deviceContext, *CommonState, m_charaData.matrix, viewMatrix, projection);

	//�U���̕`�揈��
	m_pLongRangeAttack->Draw(viewMatrix, projection);


	//���݃������[���[�N������
	//HP�Q�[�W�̕\��
	m_pHPGuage->SetRenderState(viewMatrix.Translation(), viewMatrix, projection);
	m_pHPGuage->Render();


	//���݃������[���[�N������
	//���O�̕\��
	m_pName->SetRenderState(viewMatrix.Translation(), viewMatrix, projection);
	m_pName->Render();


	//�U���������������̃G�t�F�N�g�̕`��
	if (m_hitEffectAlpha > 0.0f)
	{
		m_pEnemyHit->SetRenderState(m_eyePos, viewMatrix, projection);
		m_pEnemyHit->Render();
	}


	//�퓬��
	if (m_charaData.state == State::Battle || m_charaData.state == State::Attack)
	{
		//�U���͈͂̕\��
		m_pAttackLine->SetRenderState(m_eyePos, viewMatrix, projection);
		m_pAttackLine->Render();
	}

}

//�퓬������
void LongRangeRowling::Battle()
{
	//�U���J�n����
	if (m_attackInterval >= ATTACK_INTERVAL)
	{
		//�U���J�n����
		m_charaData.state = State::Attack;
		m_hitAttack = false;
	}
	else
	{
		//�U���J�n�܂ł̎��Ԍv�Z
		//m_speedCal�̓X���E��Ԃ̎��ƒʏ��Ԃ̎��̎��Ԃ̌v�Z���x��ς��邽��
		m_attackInterval += m_elapsedTime* m_speedCal;
		m_pAttackLine->SetTimming(m_attackInterval / ATTACK_INTERVAL);
	}
}

//�U������
void LongRangeRowling::Attack()
{
	DirectX::SimpleMath::Matrix enemyRotation = m_charaData.rotation;

	//�e�̔��ˏ���
	for (int i = 0; i < 4; i++)
	{
		enemyRotation *= DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XM_PI*0.5f);
		m_pLongRangeAttack->Shot(m_charaData.pos, enemyRotation);
	}
	EndAttack();
}

//��~����
void LongRangeRowling::Stop()
{
	if (!m_alive) return;
}

//�ړ�����
void LongRangeRowling::SelectMove()
{
	//�L�����N�^�[�̃f�[�^��ێ����Ă��郊�X�g���擾
	CharacterMapList* list = GameContext<CharacterMapList>::Get();
	//�v���C���[�ƃG�l�~�[�̊Ԃ̋���
	float length = (list->GetEntityFromID(m_playerID)->GetPos() - m_charaData.pos).Length();

	//�U���͈͊O
	if (length > BATTLE_RANGE)
	{
		if (m_charaData.state == State::Battle)
		{
			EndAttack();
		}
		m_charaData.state = State::Move;
	}
}

//�퓬�J�n����
void LongRangeRowling::SelectBattle()
{
	//�L�����N�^�[�̃f�[�^��ێ����Ă��郊�X�g���擾
	CharacterMapList* list = GameContext<CharacterMapList>::Get();
	//�v���C���[�ƃG�l�~�[�̊Ԃ̋���
	float length = (list->GetEntityFromID(m_playerID)->GetPos() - m_charaData.pos).Length();

	m_charaData.state = State::Battle;
	return;

	//�U���͈͓�
	if (length <= BATTLE_RANGE)
	{
		//�G�l�~�[����^�[�Q�b�g�Ɍ������x�N�g��
		DirectX::SimpleMath::Vector3 enemy_target_vec = list->GetEntityFromID(m_playerID)->GetPos() - m_charaData.pos;

		//�G�l�~�[�̐��ʂɌ������x�N�g��
		DirectX::SimpleMath::Vector3 frontVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, m_charaData.rotation);

		//���K��
		enemy_target_vec.Normalize();
		frontVec.Normalize();

		//���ς����߂�
		float dot = enemy_target_vec.Dot(frontVec);

		//�G���[�΍�(���ς̍ő��1�����炻��𒴂��Ȃ��悤��)
		if (dot > 1.0f)
		{
			dot = 1.0f;
		}

		//�Ȃ��p
		float angle = std::acos(dot);

		//�G�̐��ʂ�6�x�ȓ��̂Ƀv���C���[��������퓬���(���ʂ𒆐S�Ƃ���6�x�ȓ��ɂ��邽�߂�3��������)
		if (angle <= (ONE_DEGREE * 3.0f))
		{
			m_charaData.state = State::Battle;
		}
	}
}

//�U���J�n����
void LongRangeRowling::SelectAttack()
{
	if (m_charaData.state == State::Battle)
	{
		if (m_attackInterval >= ATTACK_INTERVAL)
		{
			m_charaData.state = State::Attack;
		}
		else
		{
			m_attackInterval += m_elapsedTime;
		}
	}
}

//��~����
void LongRangeRowling::SelectStop()
{

}

//�v���C���[�̃W���X�g��𔻒�
void LongRangeRowling::PlayerJustAvoidance()
{
	if (!m_alive) return;

	if (m_justAvoidance) return;

	//�L�����N�^�[�̃f�[�^��ێ����Ă��郊�X�g���擾
	CharacterMapList* list = GameContext<CharacterMapList>::Get();

	//�G�l�~�[���U����Ԃ̎�
	//�v���C���[�������Ԃ̎�
	if (list->GetEntityFromID(m_playerID)->GetState() == State::Avoidance)
	{
		//�v���C���[�̓����蔻��̐ݒ�
		Collision::Box3D player =
		{
 			list->GetEntityFromID(m_playerID)->GetPos(),
			DirectX::SimpleMath::Vector3(0.5f,1.0f,0.5f),
		};

		//�e�̒��ň�ԃv���C���[�ɋ߂��e�̓����蔻��̃f�[�^���擾
		Collision::Capsule3D enemyCapsuleData = m_pLongRangeAttack->GetCapsuleData(list->GetEntityFromID(m_playerID)->GetPos());
		//�擾�����e�̈ʒu���擾
		DirectX::SimpleMath::Vector3 enemyCapsulePos = enemyCapsuleData.startPos + ((enemyCapsuleData.startPos - enemyCapsuleData.endPos)*0.5f);

		//�U���͈͂̓����蔻��̐ݒ�(�W���X�g����͈̔�)
		//�{���̑傫�����傫�����邽��5�{����
		Collision::Box3D enemyAttack =
		{
			enemyCapsulePos,
			DirectX::SimpleMath::Vector3(
				std::abs(enemyCapsuleData.startPos.x - enemyCapsuleData.endPos.x),
				std::abs(enemyCapsuleData.range),
				std::abs(enemyCapsuleData.startPos.z - enemyCapsuleData.endPos.z)
			)*5.0f,
		};

		//�����蔻��̑傫����0��������
		if (enemyAttack.size.x <= 0.1f)
		{
			enemyAttack.size.x = 0.5f;
		}

		if (enemyAttack.size.z <= 0.1f)
		{
			enemyAttack.size.z = 0.5f;
		}

		//�����蔻��
		//�v���C���[�ƍU���͈�
		if (Collision::HitCheckoxBox3D(player, enemyAttack))
		{
			//�X���E��ԂɕύX
			StartSlow();
			for (int i = 0; i < static_cast<int>(m_pOtherEnemyList.size()); i++)
			{
				//���̓G�̃X���E��Ԃ��J�n����
				m_pOtherEnemyList[i]->StartSlow();
			}
		}
	}
}

void LongRangeRowling::EndAttack()
{
	m_attack_start_end = false;
	m_attackTime = 0.0f;
	m_attackInterval = 0.0f;
	m_charaData.state = State::Idle;
}

//�U���̓����蔻��
void LongRangeRowling::HitAttack()
{
	//�L�����N�^�[�̃f�[�^��ێ����Ă��郊�X�g���擾
	CharacterMapList* list = GameContext<CharacterMapList>::Get();

	//�����蔻��
	//�v���C���[�ƒe(�v���C���[�ɍł��߂��e)
	if (Collision::HitCheck_Capsule2Capsule(
		list->GetEntityFromID(m_playerID)->GetCapsule(),
		m_pLongRangeAttack->GetCapsuleData(list->GetEntityFromID(m_playerID)->GetPos())
	))
	{
		//�U���������U�����������Ă��Ȃ�������
		//if (!m_hitAttack)
		{
			//�e�̒�~
			m_pLongRangeAttack->HitAttack();
			//�v���C���[�̃_���[�W����
			list->GetEntityFromID(m_playerID)->Damage(m_charaData.attack, m_charaData.rotation, true, BLOW_AWAY_POWER_PLAYER_TARGET);
			//�U����������������
			m_hitAttack = true;
		}
	}
}