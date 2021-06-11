#include "../../pch.h"

#include "../List/CharacterMapList.h"

#include "Weapon/Weapon.h"
#include "Weapon/WeaponType.h"

#include "Weapon/Sword/Sword.h"
#include "Weapon/Bow/Bow.h"
#include "Weapon/Spear/Spear.h"

#include "Player.h"

#include "../../DataManager/DataManager.h"
#include "../../Map/Map.h"
#include "../../DebugObj/DebugCapsule.h"
#include "../../MyLib/Collision.h"
#include "../../MyLib/MyLib.h"

#include "../../SE�EBGM/Adx2.h"
#include "../../SE�EBGM/acb/CueSheet_0.h"

//�v���C���[�̑傫���̏�Ɖ�
const DirectX::SimpleMath::Vector3 Player::PLAYER_TOP_DOWN = DirectX::SimpleMath::Vector3(0.0f, 0.5f, 0.0f);

//�������
const float Player::AVOIDANCE_TIME = 0.15f;
//����̃N�[���^�C��
const float Player::AVOIDANCE_COOL_TIME = 0.5f;
//������΂���鎞��
const float Player::BLOW_AWAY_TIME = 0.25f;
//���G����
const float Player::INVINCIBLE_TIME = 2.0f;

//�G�l�~�[�𐁂���΂���(1�b�Ԃ�)
const float Player::BLOW_AWAY_POWER_ENEMYTARGET = 40.0f;

Player::Player(const DirectX::SimpleMath::Vector3 &pos, float attack, float hp, float speed, float thisRange)
	: Character(GetNextID(),pos,attack,hp,speed,thisRange)
	,m_pWeapon(nullptr)
	, m_pModel(nullptr)
	, m_pHPGauge(nullptr)
	//, m_pDebugCapsulePlayer(nullptr)
{
}


Player::~Player()
{
	if (m_pModel != nullptr)
	{
		m_pModel.reset(nullptr);
	}

	if (m_shadow != nullptr)
	{
		m_shadow.reset(nullptr);
	}

	if (m_pHPGauge != nullptr)
	{
		m_pHPGauge->Lost();
		m_pHPGauge.reset(nullptr);
	}

	/*if (m_pDebugCapsulePlayer != nullptr)
	{
		m_pDebugCapsulePlayer.reset(nullptr);
	}*/
}

void Player::Create(DX::DeviceResources* &deviceResources, const WeaponType &type)
{
	//����̍쐬
	switch (type)
	{
	case WeaponType::Sword:
		m_pWeapon = std::make_unique<Sword>();
		break;

	case WeaponType::Bow:
		m_pWeapon = std::make_unique<Bow>();
		break;

	case WeaponType::Spear:
		m_pWeapon = std::make_unique<Spear>();
		break;

	default:
		break;
	}
	m_pWeapon->Create(deviceResources);

	// �G�t�F�N�g�t�@�N�g���̍쐬
	DirectX::EffectFactory * factory = new DirectX::EffectFactory(deviceResources->GetD3DDevice());
	// �e�N�X�`���̓ǂݍ��݃p�X�w��
	factory->SetDirectory(L"Resources/CMO");

	// �t�@�C�����w�肵�ă��f���f�[�^�ǂݍ���
	m_pModel = DirectX::Model::CreateFromCMO(
		deviceResources->GetD3DDevice(),
		L"Resources/CMO/Character.cmo",
		*factory
	);

	//�e�̃��f��
	m_shadow = DirectX::Model::CreateFromCMO(
		deviceResources->GetD3DDevice(),
		L"Resources/CMO/Shadow.cmo",
		*factory
	);

	delete factory;

	//HP�Q�[�W
	m_pHPGauge = std::make_unique<HPGuage>();
	//�G�t�F�N�g�̌��Ɖ摜���w��(�摜�͎g���Ă��Ȃ�)
	m_pHPGauge->Create(deviceResources,1);

	//m_pDebugCapsulePlayer = std::make_unique<DebugCapsule>(deviceResources->GetD3DDevice(), DirectX::SimpleMath::Vector3::Vector3::Zero, DirectX::SimpleMath::Vector3::Vector3::Zero, 0.5f);
}

//����������
void Player::Initialize()
{
	m_charaData.pos = GetPos();
	m_charaData.angle = DirectX::SimpleMath::Vector3::Zero;
	m_charaData.hp = GetHP();

	//�U������
	m_attack = false;

	//���ʉ�
	m_playSE = false;

	//���x
	m_velocity = DirectX::SimpleMath::Vector3::Zero;
	//�����x
	m_acceleration = DirectX::SimpleMath::Vector3::Zero;
	//�L�[���͂��������̃x�N�g��
	m_inputKeyVec = DirectX::SimpleMath::Vector3::Zero;

	//����̏�����
	m_pWeapon->Initialize();
	//HP�Q�[�W�̏�����
	m_pHPGauge->InitializeNormal(0.0f,DirectX::SimpleMath::Vector3::Zero);
	//�ő�̂g�o���G�t�F�N�g�n��
	m_pHPGauge->SetMaxHP(m_charaData.hp);
	//�������
	m_avoidance.timer = 0.0f;
	//������̑���
	m_avoidance.spead = GetSpeed()*3.0f;
	//����̃N�[���^�C��
	m_avoidance.coolTimeCount = 0.0f;
	//���������������
	m_avoidance.Check = false;
	//�U���L�[�����
	m_attackKeyDown = false;
	//�ړ����Ă��邩����
	m_moveCheck = false;

	//���G����̏�����
	m_invincible = false;
	//���G���Ԃ̏�����
	m_invincibleTimer = 0.0f;
	//���G���Ԏ��̃t���[���v�Z�̏�����
	m_invincibleFrameCount = 0;
	//1�t���[���ɂ�����������(���v�����1�b�ɂȂ�)
	m_elapsedTime = 0.0f;

	m_hitAttack = false;

	//1�t���[���O�̃R���{
	m_oldCombo = 0;

	//�^�O�̐ݒ�
	m_tag = PLAYER_TAG;

	//������ԕ����̊p�x(�s��)
	m_blowAwayRotation = DirectX::SimpleMath::Matrix::Identity;
	//������ԗ�(1�b��)
	m_blowAwayPower = 0.0f;
	//������ю��Ԃ̌v��
	m_blowAwayTimer = 0.0f;
}

//�X�V����
void Player::Update(const DX::StepTimer &timer)
{
	//1�t���[���Ɋ|���������Ԃ�ݒ�
	m_elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	//�󒆂ɍs���Ȃ����߂�
	m_charaData.pos.y = 0.0f;

	//����̍X�V����
	//�X�V
	m_charaData.matrix *= DirectX::SimpleMath::Matrix::CreateTranslation(m_charaData.pos);

	//���݂�HP���G�t�F�N�g�ɓn��
	m_pHPGauge->SetNowHP(m_charaData.hp);
	//�g�o�G�t�F�N�g�̍X�V����
	m_pHPGauge->Update(timer);

	//m_pDebugCapsulePlayer->SetCenterSegment(m_charaData + DirectX::SimpleMath::Vector3(0.0f, 0.8f, 0.0f), m_charaData - DirectX::SimpleMath::Vector3(0.0f, 0.8f, 0.0f));

	//�L�[����
	InputKey();

	//�ړ�����
	Move();

	//�v���C���[�̉�]
	Rotation();


	//�U������
	Attack();

	//����̍X�V����
	AvoidanceUpdate();

	//������я���
	BlowAwayMove();

	//���G���Ԃ̍X�V����
	InvincibleUpdate();

	Collision::Box3D player
	{
		m_charaData.pos,
		DirectX::SimpleMath::Vector3(GetThisRange(),1.0f,GetThisRange())
	};

	//�}�b�v�Ƃ̏���
	Map* map = GameContext<Map>::Get();
	//�ǂɓ������Ă��邩
	m_charaData.pos = map->HitWallBox(player);
	//�ړ����E�܂ōs���Ă��邩
	m_charaData.pos = map->Limit(m_charaData.pos, m_charaData.thisRange);
	//�ړ����E�̃G�t�F�N�g
	map->LimitEffect(m_charaData.pos, m_charaData.thisRange);

	m_pWeapon->Update(m_elapsedTime);

	//�v���C���[�̎����̓����蔻��̐ݒ�
	m_charaData.thisCapsule =
	{
		m_charaData.pos + PLAYER_TOP_DOWN ,
		m_charaData.pos - PLAYER_TOP_DOWN ,
		m_charaData.thisRange,
	};
}

//�`�揈��
void Player::Draw(const DirectX::SimpleMath::Matrix &viewMatrix)
{
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();
	ID3D11DeviceContext1* deviceContext = deviceResources->GetD3DDeviceContext();
	DirectX::CommonStates* CommonState = GameContext<DirectX::CommonStates>().Get();
	Projection* projection = GameContext<Projection>().Get();

	// ���f���`��
	//�e
	DirectX::SimpleMath::Matrix shadowMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_charaData.pos + DirectX::SimpleMath::Vector3(0.0f,-1.0f,0.0f));
	m_shadow->Draw(deviceContext, *CommonState, shadowMatrix, viewMatrix, projection->GetMatrix());

	//���G���Ԃ̏�Ԃ̎��̕`�揈��
	if (m_invincible)
	{
		if (m_invincibleFrameCount % 2 == 0)
		{
			m_pModel->Draw(deviceContext, *CommonState, m_charaData.matrix, viewMatrix, projection->GetMatrix());
		}
	}
	else
	{
		//�ʏ�̏�Ԃ̕`�揈��
		m_pModel->Draw(deviceContext, *CommonState, m_charaData.matrix, viewMatrix, projection->GetMatrix());
	}

	//����� �`�揈��
	m_pWeapon->Draw(deviceContext, *CommonState, viewMatrix, projection->GetMatrix());

	//HP�Q�[�W�̕`��
	m_pHPGauge->SetRenderState(DirectX::SimpleMath::Vector3::One,viewMatrix, projection->GetMatrix());
	m_pHPGauge->Render();
}

//���Z�b�g����
void Player::Reset()
{
	//����̃��Z�b�g����
	m_pWeapon->Reset();
}

//�_���[�W����
void Player::Damage(float attack, const DirectX::SimpleMath::Matrix &rotation, bool blowAway, float power)
{
	//���G��Ԃ̎��̓_���[�W���������Ȃ�
	if (m_invincible) return;

	if (GetState() == State::Avoidance)return;

	//�̗͂��U���͕����炷
	m_charaData.hp -= attack;

	//�󂯂��_���[�W���G�t�F�N�g�ɓn��
	m_pHPGauge->HitDamage(attack);

	//���G���ԏ�ԂɕύX
	m_invincible = true;

	//������я����ɕK�v�ȃf�[�^��ݒ�
	m_blowAwayRotation = rotation;
	m_blowAwayPower = power;

	m_charaData.state = State::BlowAway;
}


//���G���Ԃ̍X�V����
void Player::InvincibleUpdate()
{
	if (m_invincible)
	{
		//���G���ԏI������
		if (m_invincibleTimer >= INVINCIBLE_TIME)
		{
			m_invincible = false;
			m_invincibleFrameCount = 0;
			m_invincibleTimer = 0.0f;
		}
		//���G���Ԃ̌v�Z
		m_invincibleTimer += m_elapsedTime;
		//���G���Ԃ̃t���[���v�Z
		m_invincibleFrameCount++;
	}
}

//�p�x�̐���
DirectX::SimpleMath::Vector3 Player::LimitsAngle(const DirectX::SimpleMath::Vector3 &angle)
{
	DirectX::SimpleMath::Vector3 retAngle = angle;

	if (retAngle.x >= DirectX::XM_PI * 2)
	{
		retAngle.x = 0.0f;
	}
	else if (retAngle.x <= DirectX::XM_PI * -2)
	{
		retAngle.x = 0.0f;
	}

	if (retAngle.y >= DirectX::XM_PI * 2)
	{
		retAngle.y = 0.0f;
	}
	else if (retAngle.y <= DirectX::XM_PI * -2)
	{
		retAngle.y = 0.0f;
	}
	if (retAngle.z >= DirectX::XM_PI * 2)
	{
		retAngle.z = 0.0f;
	}
	else if (angle.z <= DirectX::XM_PI * -2)
	{
		retAngle.z = 0.0f;
	}

	return retAngle;
}

















void Player::Move()
{
	//// �r�[�N���Ɋ|���鑀�Ǘ�
	//// �r�[�N���̃��X�g���̊e���Ǎs�����獇�͂��v�Z����
	//DirectX::SimpleMath::Vector3 steeringForce = m_steering->Calculate();

	//// �����x = �� �� ����
	//DirectX::SimpleMath::Vector3 acceleration = steeringForce / GetMass();

	//// ���x���X�V����i���x�ɉ����x�𑫂��j
	//DirectX::SimpleMath::Vector3 velocity = GetVelocity() + acceleration * elapsedTime;

	//// �r�[�N�����ő呬�x�𒴂��Ȃ��悤�ɂ���
	//if (velocity.Length() > GetMaxSpeed())
	//{
	//	velocity.Normalize();
	//	velocity *= GetMaxSpeed();
	//}

	//// �X�V�������x��ݒ肷��
	//SetVelocity(velocity);

	//// �ʒu�̍X�V����
	//SetPos(GetPos() + GetVelocity() * elapsedTime);




	//�����x�����߂�
	if (m_charaData.state == State::Move)
	{
		//�����x�����߂�
		m_acceleration += (m_inputKeyVec);
		m_acceleration.Normalize();

		//���x�����߂�
		m_velocity += m_acceleration* m_elapsedTime;

		//�ő呬�x
		if (m_velocity.Length() >= 0.1f)
		{
			//
			m_velocity.Normalize();

			m_velocity *= 0.1f;
		}
	}
	else
	{
		//���x�A�����x������
		m_acceleration *= 0.5f;
		m_velocity *= 0.5f;

		//��~
		if (m_velocity.Length() <= 0.001f)
		{
			m_acceleration = DirectX::SimpleMath::Vector3::Zero;
			m_velocity = DirectX::SimpleMath::Vector3::Zero;
		}
	}

	//�ړ�����
	m_charaData.pos += m_velocity;
}

//�U������
void Player::Attack()
{
	if (m_charaData.state == State::Attack)
	{
		//�U���J�n
		m_pWeapon->StartAttack();
		//�U���������������̔����������
		m_hitAttack = false;

		//�U���̌��ʉ��̍Đ�
		PlayAttackSE();

		//�L�����N�^�[�̃f�[�^���擾
		CharacterMapList* list = GameContext<CharacterMapList>::Get();

		std::vector<int> key = list->GetAllEntityKey();

		//���ʂɌ������x�N�g��
		DirectX::SimpleMath::Vector3 frontVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, m_charaData.rotation);
		frontVec.Normalize();

		//������̈�ԋ߂��G�̈ʒu���擾
		DirectX::SimpleMath::Vector3 enemyPos =  EnemyDistanceClosePos(key,frontVec);

		//�ړ�����
		m_charaData.pos += (CalAttackMoveVel(frontVec, enemyPos)* m_elapsedTime);
	}
}

//�U���̌��ʉ��̍Đ�
void Player::PlayAttackSE()
{
	//SE���Đ�����Ă��Ȃ��Ȃ�
	if (!m_playSE)
	{
		//SE�̍Đ�
		GameContext<ADX2>::Get()->Play(CRI_CUESHEET_0_SLASH);
		//SE�Đ�����
		m_playSE = true;
	}
}

//�U�����̈ړ����x�����߂�
DirectX::SimpleMath::Vector3 Player::CalAttackMoveVel(const DirectX::SimpleMath::Vector3 &front, const DirectX::SimpleMath::Vector3 &enemyPos)
{
	//�L�����N�^�[�̃f�[�^���擾
	CharacterMapList* list = GameContext<CharacterMapList>::Get();

	//�v���C���[����G�Ɍ������x�N�g��
	DirectX::SimpleMath::Vector3 pToEVec = enemyPos - GetPos();
	//���K��
	pToEVec.Normalize();

	//�v���C���[�̌����Ă���p�x�𒲐�
	float cross = (front.x * pToEVec.z) - (front.z * pToEVec.x);
	m_charaData.angle.y = cross;

	//�G�Ɍ�������
	DirectX::SimpleMath::Matrix matrix = MyLib::LockAt(GetPos(), enemyPos, DirectX::SimpleMath::Vector3::Forward);

	//�ړ����x
	DirectX::SimpleMath::Vector3 vel= DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, matrix) * 0.1f;

	return vel;
}

//�����̐��ʂɂ���G�̒��ň�ԋ߂��G�̈ʒu���擾
DirectX::SimpleMath::Vector3 Player::EnemyDistanceClosePos(std::vector<int> allKey, const DirectX::SimpleMath::Vector3 &frontVecN)
{
	//�L�����N�^�[�̃f�[�^���擾
	CharacterMapList* list = GameContext<CharacterMapList>::Get();

	//������̈�ԋ߂��G�̈ʒu
	DirectX::SimpleMath::Vector3 retPos = frontVecN;

	for (int i = 0; i < static_cast<int>(allKey.size()); i++)
	{
		//�����̃f�[�^�͕K�v�Ȃ�����
		if (allKey[i] == GetID())
		{
			continue;
		}

		//��������G�Ɍ������x�N�g��
		DirectX::SimpleMath::Vector3 player_enemy_vec = list->GetEntityFromID(i)->GetPos() - m_charaData.pos;

		//���K��
		player_enemy_vec.Normalize();

		//�v���C���[�̎��E���ɓG�����邩�m�F
		float dot = frontVecN.Dot(player_enemy_vec);
		//1.0f�𒴂��Ȃ����
		if (dot >= 1.0f)
		{
			dot = 1.0f;
		}

		//�Ȃ��p�̎擾
		float angle = std::acos(dot);

		//������ɂ��邩�ǂ����i����p = 30�x�j
		if (angle <= ONE_DEGREE* 30.0f)
		{
			retPos = list->GetEntityFromID(i)->GetPos();
		}
	}

	//������ɓG�����Ȃ��ꍇ���ʂ�Ԃ�
	return retPos;
}

//�����Update����(Update�ŌĂяo������	)
void Player::AvoidanceUpdate()
{
	if (m_charaData.state == State::Avoidance)
	{
		//������̈ړ�����
		AvoidanceMove();
		//������
		m_avoidance.Check = true;
		//������Ԃ̌v��
		m_avoidance.timer += m_elapsedTime;

		//����̏I������
		if (m_avoidance.timer >= AVOIDANCE_TIME)
		{
			//������x������ł���܂ł̎���
			m_avoidance.coolTimeCount = AVOIDANCE_COOL_TIME;
			//��Ԃ̏�����
			m_charaData.state = State::Idle;
			//������Ԃ��v������ϐ��̏�����
			m_avoidance.timer = 0.0f;
			//����I������
			m_avoidance.Check = false;
		}
	}

	//������Ă��Ȃ��Ȃ�
	if (!m_avoidance.Check)
	{
		//�����������x�g���邩����
		m_avoidance.coolTimeCount -= m_elapsedTime;
		if (m_avoidance.coolTimeCount < 0.0f)
		{
			m_avoidance.coolTimeCount = 0.0f;
		}
	}
}

//����̈ړ�����
void Player::AvoidanceMove()
{
	//�ړ������Ƒ��x�����߂�
	DirectX::SimpleMath::Vector3 vec;

	if (m_moveCheck)
	{
		vec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, (m_charaData.rotation)) * m_avoidance.spead;
	}
	else
	{
		vec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, (m_charaData.rotation)) *m_avoidance.spead;
	}

	//�ړ�����
	m_charaData.pos += (vec*m_elapsedTime);
}

//�W���X�g���
void Player::JusrAvoidance()
{

}

//�U����������������
void Player::HitAttack(int id)
{
	CharacterMapList* list = GameContext<CharacterMapList>::Get();
	//�G��HP���[���̂Ƃ��͏��������Ȃ�
	if (list->GetEntityFromID(id)->GetState() == State::Deth)
	{
		return;
	}

	//�U����Ԃ���Ȃ��Ƃ�
	if (!m_attack)
	{
		m_hitAttack = false;
		return;
	}

	//�R���{�̏C��
	if (m_oldCombo != m_pWeapon->GetCombo())
	{
		m_oldCombo = m_pWeapon->GetCombo();
		m_hitAttack = false;
	}

	//�U���㐔�t���[���̊�
	if (!m_pWeapon->GetAttackSTate())
	{
		//SE�̍Đ����~����
		GameContext<ADX2>::Get()->Stop(CRI_CUESHEET_0_SLASH);
		m_playSE = false;
		return;
	}

	//�㉺�̈ʒu�����p��y��0.5f�𑫂���������
	Collision::Capsule3D enemy =
	{
		list->GetEntityFromID(id)->GetPos() + DirectX::SimpleMath::Vector3(0.0f,0.5f,0.0f),
		list->GetEntityFromID(id)->GetPos() - DirectX::SimpleMath::Vector3(0.0f,0.5f,0.0f),
		list->GetEntityFromID(id)->GetThisRange(),
	};

	//����̈ʒu���擾
	DirectX::SimpleMath::Vector3 weaponPos = m_pWeapon->GetPos();
	//����̃x�N�g��(���_�����[�Ɍ�����)���擾
	DirectX::SimpleMath::Vector3 weaponVector = m_pWeapon->GetVector();
	//����̃x�N�g���Ƀv���C���[�̉�]��������
	weaponVector = weaponVector.Transform(weaponVector, m_charaData.rotation);

	//����̓����蔻��
	Collision::Capsule3D weapon =
	{
		weaponPos + weaponVector ,
		weaponPos - weaponVector ,
		0.1f
	};

	//�U���ɓ������Ă��邩
	if (Collision::HitCheck_Capsule2Capsule(enemy, weapon))
	{
		//�R���{���U�����������Ă��Ȃ��Ȃ�
		if (!m_hitAttack)
		{
			//������є���
			bool blowAway = false;
			//�ő�R���{����3�̂���
			if (m_pWeapon->GetCombo() == 3)
			{
				blowAway = true;
			}
			//�e�X�g�p��10�{
			list->GetEntityFromID(id)->Damage(m_charaData.attack*10, m_charaData.rotation, blowAway, BLOW_AWAY_POWER_ENEMYTARGET);
			m_hitAttack = true;
		}
	}
}


//�G�l�~�[�Ɠ������Ă���Ƃ�
void Player::HItEnemy(int id)
{

	CharacterMapList* list = GameContext<CharacterMapList>::Get();

	//�G��HP���[���̂Ƃ��͏��������Ȃ�
	if (list->GetEntityFromID(id)->GetState() == State::Deth)
	{
		return;
	}

	float player_enemy_length = (list->GetEntityFromID(id)->GetPos() - m_charaData.pos).Length();

	float range = list->GetEntityFromID(id)->GetThisRange() + GetThisRange();

	//�G�ɓ������Ă��邩
	if (player_enemy_length <= range)
	{
		//�G�����苗������
		DirectX::SimpleMath::Vector3 vec =
			DirectX::SimpleMath::Vector3::Transform(
				DirectX::SimpleMath::Vector3::Forward,
				m_charaData.rotation)*
				(GetThisRange() + list->GetEntityFromID(id)->GetThisRange() + (GetSpeed()*m_elapsedTime));

		m_charaData.pos = list->GetEntityFromID(id)->GetPos() - vec;
	}

}

//������я���
void Player::BlowAwayMove()
{
	if (m_charaData.state == State::BlowAway)
	{
		//�m�b�N�o�b�N(m_blowAwayPower�̐��l��1�b���Z�Ői��)
		DirectX::SimpleMath::Vector3 vec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, m_blowAwayRotation) * m_blowAwayPower;

		//�ړ�����
		m_charaData.pos += (vec*m_elapsedTime);

		//������я�Ԃ̏I��
		if (m_blowAwayTimer >= BLOW_AWAY_TIME)
		{
			m_charaData.state = State::Idle;
			m_blowAwayRotation = DirectX::SimpleMath::Matrix::Identity;
			m_blowAwayPower = 0.0f;
			m_blowAwayTimer = 0.0f;
		}
		else
		{
			//������яI���܂ł̎��Ԍv�Z
			m_blowAwayTimer += m_elapsedTime;
		}
	}
}

//�v���C���[�̉�]
void Player::Rotation()
{
	if (GetState() == State::Move)
	{
		DirectX::SimpleMath::Vector3 vector = m_velocity;

		vector.Normalize();
		float dot = vector.Dot(DirectX::SimpleMath::Vector3::Forward);
		if (dot > 1.0f)
		{
			dot = 1.0f;
		}

		//�p�x�擾 0�`180�x
		float angle = std::acos(dot);

		//���E�L�[�̂ǂ������͂��Ă��邩
		if (m_inputKeyVec.x > 0.0f)
		{
			m_charaData.angle.y = -angle;
		}
		else if (m_inputKeyVec.x <= 0.0f)
		{
			m_charaData.angle.y = angle;
		}
	}
}

//Matrix�̐ݒ�
void Player::MatrixSetting()
{
	m_charaData.matrix = DirectX::SimpleMath::Matrix::CreateRotationY(m_charaData.angle.y);
	m_charaData.matrix *= DirectX::SimpleMath::Matrix::CreateTranslation(m_charaData.pos);

	m_charaData.rotation = DirectX::SimpleMath::Matrix::CreateRotationY(m_charaData.angle.y);

	m_pWeapon->SetPlayerMatrix(m_charaData.matrix);
	m_pWeapon->MatrixSetting();
}

//�L�[����
void Player::InputKey()
{
	//�L�[���̖͂���
	if (m_charaData.state == State::Avoidance)
	{
		return;
	}

	if (m_charaData.state == State::BlowAway)
	{
		return;
	}

	if (m_pWeapon->GetAttackSTate())
	{
		return;
	}

	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();

	//��Ԃ̏�����
	m_charaData.state = State::Idle;
	//�L�[�̓��̓x�N�g���̏�����
	m_inputKeyVec = DirectX::SimpleMath::Vector3::Zero;

	//�ړ��p�̃L�[�������Ă��Ȃ��Ƃ�
	if (!keyState.W && !keyState.A && !keyState.S && !keyState.D)
	{
		//�ړ���~����
		m_moveCheck = false;
	}

	//�L�[����  �ړ�
	//�O�Ɉړ�
	if (keyState.W)
	{
		//�ړ�����
		m_moveCheck = true;
		//���͂��������ւ̃x�N�g��
		m_inputKeyVec += DirectX::SimpleMath::Vector3::Forward;
		//�ړ����
		m_charaData.state = State::Move;
	}

	//�E�Ɉړ�
	if (keyState.D)
	{
		//�ړ�����
		m_moveCheck = true;
		//���͂��������ւ̃x�N�g��
		m_inputKeyVec += DirectX::SimpleMath::Vector3::Right;
		//�ړ����
		m_charaData.state = State::Move;
	}

	//���Ɉړ�
	if (keyState.A)
	{
		//�ړ�����
		m_moveCheck = true;
		//���͂��������ւ̃x�N�g��
		m_inputKeyVec += DirectX::SimpleMath::Vector3::Left;
		//�ړ����
		m_charaData.state = State::Move;
	}

	//���Ɉړ�
	if (keyState.S)
	{
		//�ړ�����
		m_moveCheck = true;
		//���͂��������ւ̃x�N�g��
		m_inputKeyVec += DirectX::SimpleMath::Vector3::Backward;
		//�ړ����
		m_charaData.state = State::Move;
	}

	//�U��
	if (mouseState.leftButton && !m_attackKeyDown)
	{
		m_attack = true;
		m_charaData.state = State::Attack;
		m_attackKeyDown = true;
	}
	else if(!mouseState.leftButton)
	{
		m_attackKeyDown = false;
	}

	//���
	if (keyState.Space)
	{
		//����\������
		if (m_avoidance.coolTimeCount <= 0.0f)
		{
			m_charaData.state = State::Avoidance;
		}
	}
}