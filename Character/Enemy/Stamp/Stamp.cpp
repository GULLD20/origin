#include "../../../pch.h"


#include "Stamp.h"

#include "../../DataManager/DataManager.h"
#include "../../List/CharacterMapList.h"
#include "../../../MyLib/MyLib.h"

#include "../../Map/Map.h"

//�U���͈�
const float Stamp::ATTACK_RANGE = 3.0f;
//�W�����v�̎���
const float Stamp::ATTACKJUMPTIME = 0.25f ;
//�U�����̈ړ��̎���
const float Stamp::ATTACKMOVETIME = 0.5f;
//�U�����̗����̎���
const float Stamp::ATTACKFALLTIME = 0.25f;
//�U���Ԋu
const int Stamp::ATTACK_INTERVAL = 2;
//�퓬�͈�
const float Stamp::BATTLE_RANGE = 10.0f;
//�U�����̍ő�̍����̈ʒu
const float Stamp::ATTACKTOPPOS = 10.0f;

//�v���C���[�𐁂���΂���(1�b�Ԃ�)
const float Stamp::BLOW_AWAY_POWER_PLAYER_TARGET = 30.0f;

//�G�t�F�N�g�̌�
//�U���͈�
const int Stamp::NUMBER_OF_EFFECT_ATTACK_AREA = 1;
//�U��������������
const int Stamp::NUMBER_OF_EFFECT_HIT = 25;
//���S
const int Stamp::NUMBER_OF_EFFECT_DETH = 50;

//�X���E��Ԃ̌v�Z�p�̐��l
const float Stamp::SLOW_CAL = 0.025f;
//�ʏ��Ԃ̌v�Z�p�̐��l
const float Stamp::NORMAL_CAL = 1.0f;

Stamp::Stamp(const DirectX::SimpleMath::Vector3 &pos, float attack, float hp, float speed, float thisRange)
	:Enemy(pos, attack, hp, speed, thisRange)
{
}


Stamp::~Stamp()
{
	if (m_pAttackArea != nullptr)
	{
		m_pAttackArea->Lost();
		m_pAttackArea.reset(nullptr);
	}
}

//�G�t�F�N�g�̍쐬
void Stamp::CreateEffect()
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
	m_pAttackArea = std::make_unique<AttackArea>();
	//�摜�ƃG�t�F�N�g�̍쐬���̐ݒ�
	m_pAttackArea->Create(deviceResources, NUMBER_OF_EFFECT_ATTACK_AREA);

	//�U���������������̉��o(�v���C���[�̍U�����G�l�~�[�ɓ���������)�̍쐬
	m_pEnemyHit = std::make_unique<EnemyHit>();
	m_pEnemyHit->Create(deviceResources, NUMBER_OF_EFFECT_HIT);

	//���S���o�̍쐬
	m_pEnemyDeth = std::make_unique<EnemyDeth>();
	//�摜�ƃG�t�F�N�g�̍쐬���̐ݒ�
	m_pEnemyDeth->Create(deviceResources,  NUMBER_OF_EFFECT_DETH);
}

//�h���N���X�p�X�V����
void Stamp::SubInitialize()
{
	//��]�p�x�̏�����
	m_angle = 360.0f;

	//�U���J�n����
	m_startAttack = false;

	//�W�����v���̖ړI�ʒu�̏�����
	m_jumpDestination = DirectX::SimpleMath::Vector3(100.0f, 100.0f, 100.0f);

	//�G�l�~�[�̐�����є���̏�����
	m_blowAway = true;

	//�U���̃W�����v�̎��Ԍv��
	m_attackJumpTimer = 0.0f;
	//�U���̈ړ��̎��Ԍv��
	m_attackMoveTimer = 0.0f;
	//�U���̗����̎��Ԍv��
	m_attackFallTimer = 0.0f;

	//�U�����̊e����̏�����
	//�W�����v�I��
	m_attackJump = false;
	//�U���I��
	m_attackMove = false;
	//�����I��
	m_attackFall = false;

	//1�t���[���]�T����������ϐ��̏�����
	m_oneFrameWait = 0;

	//�U���I������̏�����
	m_endAttack = false;
}

//�G�t�F�N�g�̏�����
void Stamp::EffectInitialize()
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

	//�U���͈͂̏�����
	m_pAttackArea->InitializeNormal(5, DirectX::SimpleMath::Vector3(0, 0, 0));
	m_pAttackArea->SetScale(DirectX::SimpleMath::Vector3(ATTACK_RANGE, 1.0f, ATTACK_RANGE));

	//�U���������������̉��o(�v���C���[�̍U�����G�l�~�[�ɓ���������)�̏�����
	m_pEnemyHit->InitializeNormal(1, DirectX::SimpleMath::Vector3::Zero);
	m_pEnemyHit->SetScale(DirectX::SimpleMath::Vector3(0.2f, 0.2f, 0.2f));

	//���S���o�̏�����
	m_pEnemyDeth->InitializeCorn(10.0f, DirectX::SimpleMath::Vector3(0.0f, -0.25f, 0.0f), DirectX::SimpleMath::Vector3(0.0f, 0.1f, 0.0f));
	m_pEnemyDeth->SetScale(DirectX::SimpleMath::Vector3(0.2f, 0.2f, 0.2f));
}


//�eUpdate�֐�-----------------
//�h���N���X�p�X�V����
void Stamp::SubUpdate()
{

}

//�G�t�F�N�g�̍X�V����
void Stamp::EffectUpdate(const DX::StepTimer &timer)
{
	if (DethUpdate(timer))
	{
		return;
	}

	HPTextureSize();
	//HP�Q�[�W�̍X�V
	m_pHPGuage->RotationY(m_charaData.pos + DirectX::SimpleMath::Vector3(0.0f, 0.5f, 0.0f), m_eyePos);
	m_pName->RotationY(m_charaData.pos + DirectX::SimpleMath::Vector3(0.0f, 0.7f, 0.0f), m_eyePos);

	m_pAttackArea->Update(timer);

	if (m_charaData.state == State::Battle)
	{
		m_pAttackArea->SetPos(GameContext<CharacterMapList>::Get()->GetEntityFromID(m_playerID)->GetPos() + (DirectX::SimpleMath::Vector3::Down));
	}

	m_pAttackArea->SetMainMatrix(m_charaData.rotation);

	//�����Ȃ�X�V���Ȃ�
	if (m_hitEffectAlpha > 0.0f)
	{
		m_hitEffectAlpha-= m_elapsedTime;
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
void Stamp::AliveDraw(const DirectX::SimpleMath::Matrix &viewMatrix, const DirectX::SimpleMath::Matrix &projection)
{
	//�`��Ɏg���f�[�^���擾
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();
	ID3D11DeviceContext1* deviceContext = deviceResources->GetD3DDeviceContext();
	DirectX::CommonStates* CommonState = GameContext<DirectX::CommonStates>().Get();

	// ���f���`��
	m_pModel->Draw(deviceContext, *CommonState, m_charaData.matrix, viewMatrix, projection);

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
	if (m_charaData.state == State::Attack)
	{
		//�U���͈͂̕\��
		m_pAttackArea->SetRenderState(m_eyePos, viewMatrix, projection);
		m_pAttackArea->Render();
	}
}

//�퓬������
void Stamp::Battle()
{
	//�U���J�n����
	if (m_attackInterval >= ATTACK_INTERVAL)
	{
		//�U���J�n����
		m_charaData.state = State::Attack;
		m_hitAttack = false;
		m_startAttack = false;
		m_attackInterval = 0.0f;
	}
	else
	{
		//�U���J�n�܂ł̎��Ԍv�Z
		//m_speedCal�̓X���E��Ԃ̎��ƒʏ��Ԃ̎��̎��Ԃ̌v�Z���x��ς��邽��
		m_attackInterval += m_elapsedTime* m_speedCal;
	}
}

//�U������
void Stamp::Attack()
{
	//�ړI�n�̐ݒ�
	if (m_jumpDestination.y >= 50.0f)
	{
		//�L�����N�^�[�̃f�[�^��ێ����Ă��郊�X�g���擾
		CharacterMapList* list = GameContext<CharacterMapList>::Get();
		//�v���C���[�̈ʒu���擾
		m_jumpDestination = list->GetEntityFromID(m_playerID)->GetPos();
		//�v���C���[�̏�Ɉʒu���C��
		m_jumpDestination.y += ATTACKTOPPOS;
	}

	if (!m_attackJump)
	{
		//�W�����v����
		AttackJump();
	}
	else if (!m_attackMove)
	{
		AttackMove();
	}
	else if (!m_attackFall)
	{
		AttackFall();
	}

	if (!m_attackJump || !m_attackMove)
	{
		return;
	}

	if (m_oneFrameWait == 1)
	{
		//�U���I������
		EndAttack();
	}

	if (m_endAttack)
	{
		//1�t���[���]�T����������
		m_oneFrameWait++;
	}

	//�L�����N�^�[�̃f�[�^��ێ����Ă��郊�X�g���擾
	Character* player = GameContext<CharacterMapList>::Get()->GetEntityFromID(m_playerID);

	//�����蔻��
	//�v���C���[�ƓG
	if (Collision::HitCheck_Capsule2Capsule(player->GetCapsule(), m_charaData.thisCapsule))
	{
		//�G�l�~�[����v���C���[�Ɍ������x�N�g���̎擾
		DirectX::SimpleMath::Vector3 enemy_player_vec =
			player->GetPos() - m_charaData.pos;

		//�x�N�g����������ɍs���Ȃ�����
		enemy_player_vec.y = 0.0f;

		//���K��
		enemy_player_vec.Normalize();

		//�U���������U�����������Ă��Ȃ�������
		if (!m_hitAttack)
		{
			//�v���C���[�̃_���[�W����
			player->Damage(m_charaData.attack, m_charaData.rotation,true, BLOW_AWAY_POWER_PLAYER_TARGET);
			//�U����������������
			m_hitAttack = true;
		}
	}

	Collision::Capsule3D blowArea
	{
		m_charaData.thisCapsule.startPos,
		m_charaData.thisCapsule.endPos,
		ATTACK_RANGE,
	};

	float areaS = blowArea.startPos.y - (m_charaData.thisCapsule.range*0.5f);
	float areaE = blowArea.endPos.y + (m_charaData.thisCapsule.range*0.5f);

	if (areaS > player->GetPos().y && areaE < player->GetPos().y)
	{

		if (Collision::HitCheck_Capsule2Capsule(player->GetCapsule(), blowArea))
		{
			//�v���C���[���΂�����
			player->Damage(0.0f,m_charaData.rotation, true, BLOW_AWAY_POWER_PLAYER_TARGET);
		}
	}
}

//��~����
void Stamp::Stop()
{
	if (!m_alive) return;
}

//�ړ�����
void Stamp::SelectMove()
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
void Stamp::SelectBattle()
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
		//�G���[�΍�
		if (dot > 1.0f)
		{
			dot = 1.0f;
		}

		//�Ȃ��p
		float angle = std::acos(dot);

		//�G�̐��ʂ�16�x�ȓ��̂Ƀv���C���[��������퓬���
		if (angle <= (ONE_DEGREE * 16.0f))
		{
			m_charaData.state = State::Battle;
		}
	}
}

//�U���J�n����
void Stamp::SelectAttack()
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
void Stamp::SelectStop()
{

}

//�v���C���[�̃W���X�g��𔻒�
void Stamp::PlayerJustAvoidance()
{
	if (!m_alive) return;

	if (m_justAvoidance) return;

	//�L�����N�^�[�̃f�[�^��ێ����Ă��郊�X�g���擾
	CharacterMapList* list = GameContext<CharacterMapList>::Get();

	//�G�l�~�[���U����Ԃ̎�
	//�v���C���[�������Ԃ̎�
	if (m_charaData.state == State::Attack &&
		list->GetEntityFromID(m_playerID)->GetState() == State::Avoidance)
	{
		//�J�v�Z���̊J�n�ʒu�ƏI���ʒu
		DirectX::SimpleMath::Vector3 startPos = DirectX::SimpleMath::Vector3(m_charaData.thisCapsule.startPos.x, m_charaData.thisCapsule.startPos.y*5.0f, m_charaData.thisCapsule.startPos.z);
		DirectX::SimpleMath::Vector3 endPos = DirectX::SimpleMath::Vector3(m_charaData.thisCapsule.endPos.x, m_charaData.thisCapsule.endPos.y*5.0f, m_charaData.thisCapsule.endPos.z);

		//�W���X�g���p�̓����蔻��(���a��2�{����)
		Collision::Capsule3D enemyAvoidanceArea =
		{
			startPos,
			endPos,
			m_charaData.thisRange * 5.0f,
		};

		//�����蔻��
		//�v���C���[�ƍU���͈�
		if (Collision::HitCheck_Capsule2Capsule(list->GetEntityFromID(m_playerID)->GetCapsule(), enemyAvoidanceArea))
		{
			//�X���E��ԂɕύX
			StartSlow();
			for (int i = 0; i <static_cast<int>(m_pOtherEnemyList.size()); i++)
			{
				//���̓G�̃X���E��Ԃ��J�n����
				m_pOtherEnemyList[i]->StartSlow();
			}
		}
	}
}

//�U���I��
void Stamp::EndAttack()
{
	//�U�����菉����
	m_attack_start_end = false;
	m_attackTimer = 0.0f;
	m_attackInterval = 0.0f;
	m_charaData.state = State::Idle;

	//�W�����v���̖ړI�n��K�����B�ł��Ȃ��ʒu�ɐݒ�
	m_jumpDestination.y = 55.0f;
	//�G�̍����𒲐�
	m_charaData.pos.y = 0.0f;

	//�����̓����蔻���ݒ�
	m_charaData.thisCapsule =
	{
		m_charaData.pos + ENEMYSIZE_TOP_DOWN,
		m_charaData.pos - ENEMYSIZE_TOP_DOWN,
		GetThisRange(),
	};

	//�e���Ԃ̏�����
	m_attackJumpTimer = 0.0f;
	m_attackMoveTimer = 0.0f;
	m_attackFallTimer = 0.0f;

	//�e����̏�����
	m_attackJump = false;
	m_attackMove = false;
	m_attackFall = false;

	//1�t���[���]�T����������ϐ��̏�����
	m_oneFrameWait = 0;

	//�U���I������̏�����
	m_endAttack = false;

	//�F��߂�
	m_pAttackArea->SetTimming(0.0f);
}

void Stamp::AttackJump()
{
	//�㏸����
	m_charaData.pos.y = MyLib::Lerp(0.0f, ATTACKTOPPOS, m_attackJumpTimer / static_cast<float>(ATTACKJUMPTIME));
	m_attackJumpTimer += m_elapsedTime;

	//�W�����v�I��
	if (m_attackJumpTimer >= ATTACKJUMPTIME)
	{
		m_startMovePos = m_charaData.pos;

		m_attackJump = true;
	}
}

void Stamp::AttackMove()
{
	//�W�����v�I�����̃v���C���[�̈ʒu�̏�Ɉړ�
	m_charaData.pos = DirectX::SimpleMath::Vector3::Lerp(m_startMovePos, m_jumpDestination, (m_attackMoveTimer / ATTACKMOVETIME));
	m_attackMoveTimer+= m_elapsedTime;

	//�ړ��̏I��
	if (m_attackMoveTimer >= ATTACKMOVETIME)
	{
		m_attackMove = true;
	}
}

void Stamp::AttackFall()
{
	//��������(�^���Ɉړ�)
	m_charaData.pos.y = MyLib::Lerp(ATTACKTOPPOS, 0.0f, (m_attackFallTimer / ATTACKFALLTIME));
	m_attackFallTimer += m_elapsedTime;

	//�n�ʂɓ�����^�C�~���O(���a��0.5f�Ȃ���0.5f��������)
	m_pAttackArea->SetTimming((m_attackFallTimer / ATTACKFALLTIME)*0.5f);

	//�U�����Ԃ��I��������
	if (m_attackFallTimer >= ATTACKFALLTIME)
	{
		m_endAttack = true;
	}
}