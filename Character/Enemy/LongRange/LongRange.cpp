#include "../../../pch.h"

#include "LongRange.h"

#include "../../DataManager/DataManager.h"
#include "../../List/CharacterMapList.h"

#include "../../Map/Map.h"

//�U������
const float LongRange::ATTACK_TIME = 1.0f;
//�U���͈�
const float LongRange::ATTACKRANGE = 20.0f;
//�U���Ԋu
const int LongRange::ATTACK_INTERVAL = 2;
//�퓬�͈�
const float LongRange::BATTLE_RANGE = 20.0f;
//�e�̍U�����x(1�b�Ԃ̑��x)
const float LongRange::ATTACK_SPEED = 36.0f;

//�v���C���[�𐁂���΂���(1�b�Ԃ�)
const float LongRange::BLOW_AWAY_POWER_PLAYER_TARGET = 5.0f;

//�G�t�F�N�g�̌�
//�U���͈�
const int LongRange::NUMBER_OF_EFFECT_ATTACK_AREA = 1;
//�U��������������
const int LongRange::NUMBER_OF_EFFECT_HIT = 25;
//���S
const int LongRange::NUMBER_OF_EFFECT_DETH = 50;

//�X���E��Ԃ̌v�Z�p�̐��l
const float LongRange::SLOW_CAL = 0.025f;
//�ʏ��Ԃ̌v�Z�p�̐��l
const float LongRange::NORMAL_CAL = 1.0f;

LongRange::LongRange(const DirectX::SimpleMath::Vector3 &pos, float attack, float hp, float speed, float thisRange)
	:Enemy(pos, attack, hp, speed, thisRange)
{
}


LongRange::~LongRange()
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
void LongRange::CreateEffect()
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
void LongRange::SubInitialize()
{
	m_pLongRangeAttack = std::make_unique<LongRangeAttack>();
	//�U���̃��f���쐬
	m_pLongRangeAttack->CreatModel();
	//�U���̏���������
	m_pLongRangeAttack->Initialize(ATTACK_SPEED,static_cast<int>(BATTLE_RANGE));
	//��]�p�x�̏�����
	m_angle = 360.0f;
	//�G�l�~�[�̐�����є���̏�����
	m_blowAway = false;
}

//�G�t�F�N�g�̏�����
void LongRange::EffectInitialize()
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
void LongRange::SubUpdate()
{
	//�U��(�e)�̍X�V����
	m_pLongRangeAttack->SetSpeedCal(m_speedCal);
	m_pLongRangeAttack->Update(m_elapsedTime);
	HitAttack();
}

//�G�t�F�N�g�̍X�V����
void LongRange::EffectUpdate(const DX::StepTimer &timer)
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
void LongRange::AliveDraw(const DirectX::SimpleMath::Matrix &viewMatrix, const DirectX::SimpleMath::Matrix &projection)
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

	//HP�Q�[�W�̕\��
	m_pHPGuage->SetRenderState(viewMatrix.Translation(), viewMatrix, projection);
	m_pHPGuage->Render();

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
void LongRange::Battle()
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
void LongRange::Attack()
{
	//�e�̔��ˏ���
	m_pLongRangeAttack->Shot(m_charaData.pos, m_charaData.rotation);
	EndAttack();
}

//��~����
void LongRange::Stop()
{
	if (!m_alive) return;
}

//�ړ�����
void LongRange::SelectMove()
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
void LongRange::SelectBattle()
{
	//�L�����N�^�[�̃f�[�^��ێ����Ă��郊�X�g���擾
	CharacterMapList* list = GameContext<CharacterMapList>::Get();
	//�v���C���[�ƃG�l�~�[�̊Ԃ̋���
	float length = (list->GetEntityFromID(m_playerID)->GetPos() - m_charaData.pos).Length();

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
void LongRange::SelectAttack()
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
void LongRange::SelectStop()
{

}

//�v���C���[�̃W���X�g��𔻒�
void LongRange::PlayerJustAvoidance()
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

void LongRange::EndAttack()
{
	m_attack_start_end = false;
	m_attackTime = 0.0f;
	m_attackInterval = 0.0f;
	m_charaData.state = State::Idle;
}

//�U���̓����蔻��
void LongRange::HitAttack()
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