#include "../../../pch.h"

#include "Rowling.h"

#include "../../DataManager/DataManager.h"
#include "../../List/CharacterMapList.h"

#include "../../Map/Map.h"
#include "../../MyLib/MyLib.h"

//�U������
const float Rowling::ATTACK_TIME = 3.0f;
//�U���͈�
const float Rowling::ATTACK_RANGE = 50.0f;
//�U���Ԋu
const int Rowling::ATTACK_INTERVAL = 2;
//�퓬�͈�
const float Rowling::BATTLE_RANGE = 50.0f;
//�U���̂Ƃ��̈ړ����x(�ʏ푬�x�̉��{��)
const float Rowling::ATTACK_MOVE_SPEED = 3.0f;

//�v���C���[�𐁂���΂���(1�b�Ԃ�)
const float Rowling::BLOW_AWAY_POWER_PLAYER_TARGET = 2.0f;

//�G�t�F�N�g�̌�
//�U���͈�
const int Rowling::NUMBER_OF_EFFECT_ATTACK_AREA = 1;
//�U��������������
const int Rowling::NUMBER_OF_EFFECT_HIT = 25;
//���S
const int Rowling::NUMBER_OF_EFFECT_DETH = 50;

//�X���E��Ԃ̌v�Z�p�̐��l
const float Rowling::SLOW_CAL = 0.025f;
//�ʏ��Ԃ̌v�Z�p�̐��l
const float Rowling::NORMAL_CAL = 1.0f;

Rowling::Rowling(const DirectX::SimpleMath::Vector3 &pos, float attack, float hp, float speed, float thisRange)
	:Enemy(pos, attack, hp, speed, thisRange)
{
}


Rowling::~Rowling()
{
	if (m_pAttackLine != nullptr)
	{
		m_pAttackLine->Lost();
		m_pAttackLine.reset(nullptr);
	}
}

//�G�t�F�N�g�̍쐬
void Rowling::CreateEffect()
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
void Rowling::SubInitialize()
{
	//��]�p�x�̏�����(1�b�Ԃɉ��x��]���邩)
	m_angle = 360.0f;

	//�U�����̊p�x��ۑ��̏�����
	m_attackAngle = 0.0f;

	//�U�����̈ړ��o�H�����܂���������
	m_setAttackLine = false;

	//�G�l�~�[�̐�����є���̏�����
	m_blowAway = true;
}

//�G�t�F�N�g�̏�����
void Rowling::EffectInitialize()
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
	DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(0.5f, 1.0f, (ATTACK_RANGE * 0.5f));

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
void Rowling::SubUpdate()
{

}

//�G�t�F�N�g�̍X�V����
void Rowling::EffectUpdate(const DX::StepTimer &timer)
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
			+ (DirectX::SimpleMath::Vector3::Down * 1.35f)
			+ DirectX::SimpleMath::Vector3::Transform(
				DirectX::SimpleMath::Vector3::Forward*(ATTACK_RANGE * 0.5f)
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
void Rowling::AliveDraw(const DirectX::SimpleMath::Matrix &viewMatrix, const DirectX::SimpleMath::Matrix &projection)
{
	//�`��Ɏg���f�[�^���擾
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();
	ID3D11DeviceContext1* deviceContext = deviceResources->GetD3DDeviceContext();
	DirectX::CommonStates* commonState = GameContext<DirectX::CommonStates>().Get();

	// ���f���`��
	//�e
	DirectX::SimpleMath::Matrix shadowMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_charaData.pos + DirectX::SimpleMath::Vector3(0.0f, -1.0f, 0.0f));
	m_shadow->Draw(deviceContext, *commonState, shadowMatrix, viewMatrix, projection);

	//�L�����N�^�[
	m_pModel->Draw(deviceContext, *commonState, m_charaData.matrix, viewMatrix, projection);

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
	if (m_charaData.state == State::Battle )
	{
		//�U���͈͂̕\��
		m_pAttackLine->SetRenderState(m_eyePos, viewMatrix, projection);
		m_pAttackLine->Render();
	}
}

//�퓬������
void Rowling::Battle()
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
		if (!m_setAttackLine)
		{
			SetAttackLine();
		}
		//�U���J�n�܂ł̎��Ԍv�Z
		//m_speedCal�̓X���E��Ԃ̎��ƒʏ��Ԃ̎��̎��Ԃ̌v�Z���x��ς��邽��
		m_attackInterval += (m_elapsedTime* m_speedCal);
		m_pAttackLine->SetTimming(m_attackInterval / ATTACK_INTERVAL);
	}
}

//�U������
void Rowling::Attack()
{
	//�ړ����x�����߂�
	DirectX::SimpleMath::Vector3 vec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, m_charaData.rotation)
		* (m_charaData.speed * ATTACK_MOVE_SPEED);

	//�ړ�����(m_elapsedTime��������1�b�Ԃ̑��x�����߂�)
	m_charaData.pos += ((vec * m_speedCal)*m_elapsedTime);

	//�X���E��Ԃ̎��͔��ˏ��������Ȃ�
	if (!m_justAvoidance)
	{
		Reflect();
	}
	//�ʒu��ۑ�
	m_oldPos = m_charaData.pos;

	//�U�����Ԃ̌v�Z(�X���E��Ԃ̂Ƃ��͍��킹�Čv�Z���x�点��)
	m_attackTimer += m_elapsedTime * m_speedCal;

	//�L�����N�^�[�̃f�[�^��ێ����Ă��郊�X�g���擾
	CharacterMapList* list = GameContext<CharacterMapList>::Get();

	//�����蔻��
	//�v���C���[�ƓG
	if (Collision::HitCheck_Capsule2Capsule(list->GetEntityFromID(m_playerID)->GetCapsule(), m_charaData.thisCapsule))
	{
		//�U���������U�����������Ă��Ȃ�������
		if (!m_hitAttack)
		{
			//�v���C���[�̃_���[�W����
			list->GetEntityFromID(m_playerID)->Damage(m_charaData.attack, m_charaData.rotation,true, BLOW_AWAY_POWER_PLAYER_TARGET);
			//�U����������������
			m_hitAttack = true;
		}
	}
}

//��~����
void Rowling::Stop()
{
	if (!m_alive) return;
}

//�ړ�����
void Rowling::SelectMove()
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
			//�U���I������
			EndAttack();
		}
		m_charaData.state = State::Move;
	}
}

//�퓬�J�n����
void Rowling::SelectBattle()
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
		if (angle <= (ONE_DEGREE * 8.0f))
		{
			m_charaData.state = State::Battle;
		}
	}
}

//�U���J�n����
void Rowling::SelectAttack()
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
void Rowling::SelectStop()
{

}

//�v���C���[�̃W���X�g��𔻒�
void Rowling::PlayerJustAvoidance()
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
		Collision::Capsule3D enemyAvodanceArea =
		{
			m_charaData.thisCapsule.startPos,
			m_charaData.thisCapsule.endPos,
			m_charaData.thisRange * 10.0f,
		};

		//�����蔻��
		//�v���C���[�ƍU���͈�
		if (Collision::HitCheck_Capsule2Capsule(list->GetEntityFromID(m_playerID)->GetCapsule(), enemyAvodanceArea))
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

void Rowling::EndAttack()
{
	m_attack_start_end = false;
	m_attackTimer = 0.0f;
	m_attackInterval = 0.0f;
	m_charaData.state = State::Idle;
	m_setAttackLine = false;
}

void Rowling::SetAttackLine()
{
	//�ړ������𗐐��Ŏ擾����(0�x�`360�x)
	m_attackAngle = ONE_DEGREE * static_cast<float>(MyLib::MersenneTwisterI(0, 360));
	//�p�x�̐ݒ�
	if (m_setAttackLine)
	{
		m_charaData.angle.y = m_attackAngle;
	}
}

//���ˎ��̊p�x�����߂�
void Rowling::Reflect()
{
	if (ReflectNearlyEqual_Old_Now_Pos())
	{
		m_setAttackLine = true;
		return;
	}

	//�}�b�v�̎擾
	Map* map = GameContext<Map>::Get();
	//�ǂɂ������Ă��邩����
	if (map->GetHitWallCorner(m_charaData.pos).pos1 != DirectX::SimpleMath::Vector3::Zero)
	{
		if (EndReflect())
		{
			return;
		}
		//�ǂɓ����������ɍU�����v���C���[�ɂ������邩�̔����߂�
		m_hitAttack = false;
		//�ŏ��ȊO�͗����ŕ��������߂邽��
		m_setAttackLine = true;
		//���Ɍ��������������߂�
		SetAttackLine();
	}
}

//1�t���[���O�̈ʒu�ƌ��݂̈ʒu���قړ�������
bool  Rowling::ReflectNearlyEqual_Old_Now_Pos()
{
	//�p�ɂ���Ƃ��ɕǂƓ������Ă��Ȃ���ԂɂȂ�Ƃ������邽��
	//�l�p�̓����蔻��ŋ߂��ʒu�ɂ��邩����
	Collision::Box3D old =
	{
		m_oldPos,
		DirectX::SimpleMath::Vector3(0.1f,0.1f,0.1f),
	};
	Collision::Box3D now =
	{
		m_charaData.pos,
		DirectX::SimpleMath::Vector3(0.1f,0.1f,0.1f),
	};

	if (Collision::HitCheckoxBox3D(now, old))
	{
		if (EndReflect())
		{
			return true;
		}
		//�ǂɓ����������ɍU�����v���C���[�ɂ������邩�̔����߂�
		m_hitAttack = false;
		//���Ɍ��������������߂�
		SetAttackLine();
		return true;
	}

	return false;
}

//���˂̏I������
bool Rowling::EndReflect()
{
	if (m_attackTimer >= ATTACK_TIME)
	{
		//�U���I������
		EndAttack();
		return true;
	}
	return false;
}