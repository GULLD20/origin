#include "../../pch.h"

#include "../../DeviceResources.h"

#include "Enemy.h"

#include "../../DataManager/DataManager.h"
#include "../List/CharacterMapList.h"

#include "../../Map/Map.h"

//�G�̑傫����Ɖ�
const DirectX::SimpleMath::Vector3 Enemy::ENEMYSIZE_TOP_DOWN = DirectX::SimpleMath::Vector3(0.0f,0.5f,0.0f);

//������΂���鎞��
const float Enemy::BLOW_AWAY_TIME = 0.25f;

//���G����
const float Enemy::INVINCIBLE_TIME = 4.0f;

//�X���E��Ԃ̌v�Z�p�̐��l
const float Enemy::SLOW_CAL = 0.025f;
//�ʏ��Ԃ̌v�Z�p�̐��l
const float Enemy::NORMAL_CAL = 1.0f;

//�X���E����
const float Enemy::SLOW_TIME = 3.0f;

Enemy::Enemy(const DirectX::SimpleMath::Vector3 &pos, float attack, float hp, float speed, float thisRange)
	:Character(GetNextID(),pos, attack, hp, speed, thisRange)
	,m_pModel(nullptr)
{
}


Enemy::~Enemy()
{
	if (m_pModel != nullptr)
	{
		m_pModel.reset(nullptr);
	}

	if (m_pHPGuage != nullptr)
	{
		m_pHPGuage->Reset();
		m_pHPGuage.reset(nullptr);
	}

	if (m_pName != nullptr)
	{
		m_pName->Reset();
		m_pName.reset(nullptr);
	}

	if (m_pEnemyHit != nullptr)
	{
		m_pEnemyHit->Lost();
		m_pEnemyHit.reset(nullptr);
	}

	if (m_pEnemyDeth != nullptr)
	{
		m_pEnemyDeth->Lost();
		m_pEnemyDeth.reset(nullptr);
	}

	if (m_pOtherEnemyList.size() > 0)
	{
		for (int i = 0; i < static_cast<int>(m_pOtherEnemyList.size()); i++)
		{
			delete m_pOtherEnemyList[i];
			m_pOtherEnemyList[i] = nullptr;
		}

		m_pOtherEnemyList.clear();
	}
}

void Enemy::CreateModel(const wchar_t* fileName)
{
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();

	// �G�t�F�N�g�t�@�N�g���̍쐬
	DirectX::EffectFactory* factory = new DirectX::EffectFactory(deviceResources->GetD3DDevice());
	// �e�N�X�`���̓ǂݍ��݃p�X�w��
	factory->SetDirectory(L"Resources/CMO");

	//�L�����N�^�[�̃��f���̓ǂݍ���
	m_pModel = DirectX::Model::CreateFromCMO(
		deviceResources->GetD3DDevice(),
		fileName,
		*factory
	);

	//�e�̃��f��
	m_shadow = DirectX::Model::CreateFromCMO(
		deviceResources->GetD3DDevice(),
		L"Resources/CMO/Shadow.cmo",
		*factory
	);

	delete factory;
}

void Enemy::Initialize()
{
	//�ړI�n�����Ă�����ϐ�
	m_wanderingAssignment = 0;

	//�X�s�[�h�v�Z�p
	m_speedCal = NORMAL_CAL;
	//��������
	m_alive = true;
	//�v���C���[�𔭌�����������
	m_discovery = false;
	//�ړ����
	m_walk = false;

	//���[�v�����𒲐����邽�߂̕ϐ�
	m_lerpAdjustment = 0.0f;

	//
	m_withinRange = false;

	//�`��͈͓�������
	m_checkCamera = false;

	//1��̍U���Ɏg���t���[���̊m�F
	m_attackTime = 0.0f;

	//�U�������̔���
	m_attack_start_end = false;

	//�U���Ԋu
	m_attackInterval = 0.0f;
	//1�t���[���ɂ�����������
	m_elapsedTime = 0.0f;

	//1�t���[���O�̂Ȃ��p
	m_oldAco = 0.0f;

	//���G���
	m_invincible = false;
	//���G����
	m_invincibleTime = 0.0f;
	//1��̍U���œG��1�񂾂��_���[�W�����������邽��
	m_hitAttack = false;

	m_justAvoidance = false;
	m_justAvoidanceTime = 0.0f;
	//m_justAvoidanceFlameControl = 0;

	//�v���C���[��ID�̏�����
	m_playerID = 0;

	//�U���������������̃G�t�F�N�g�̃A���t�@�l
	m_hitEffectAlpha = 0.0f;

	//���S�G�t�F�N�g�̃A���t�@�l
	m_dethEffectAlpha = 1.0f;

	//�^�O�̐ݒ�
	m_tag = ENEMY_TAG;

	//�v���C���[�̍U���̃R���{���Ōォ
	m_playerAttackCombLast = false;

	//�G�l�~�[�̐�����є���̏�����
	m_blowAway = true;

	//������ԕ����̊p�x(�s��)
	m_blowAwayRotation = DirectX::SimpleMath::Matrix::Identity;
	//������ԗ�(1�b��)
	m_blowAwayPower = 0.0f;
	//������ю��Ԃ̌v��
	m_blowAwayTime = 0.0f;

	//�h���N���X�̏���������
	SubInitialize();
}

void Enemy::Update(const DX::StepTimer &timer)
{
	//1�t���[���ɂ����������Ԃ̐ݒ�
	m_elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	//�v���C���[�̃W���X�g������Ԃ̍X�V����
	JustAvoidanceUpdate();

	//State�ɍ��킹���X�V����
	StateUpdate();

	//�h���N���X�̍X�V����
	SubUpdate();

	//���G���Ԃ̍X�V����
	InvincibleUpdate();

	//�v���C���[�̃W���X�g��𔻒�
	PlayerJustAvoidance();

	//��~����
	Stop();

	MatrixSetting();
	//�G�t�F�N�g�̍X�V����
	EffectUpdate(timer);

	//Map�Ƃ̓����蔻��
	HitMap();

	//�f�[�^�̍X�V����
	DataSetting();

	//�����̓����蔻���ݒ�
	m_charaData.thisCapsule =
	{
		m_charaData.pos + ENEMYSIZE_TOP_DOWN,
		m_charaData.pos - ENEMYSIZE_TOP_DOWN,
		GetThisRange(),
	};
}

void Enemy::Draw(const DirectX::SimpleMath::Matrix &viewMatrix)
{
	//�`��͈͓�
	if (m_checkCamera)
	{
		//�v���W�F�N�V�����s��̎擾
		Projection* projection = GameContext<Projection>().Get();

		//���S����
		//�����Ă���Ȃ�
		if (m_alive)
		{
			AliveDraw(viewMatrix, projection->GetMatrix());
		}
		//����ł���Ȃ�
		if (!m_alive)
		{
			DethDraw(viewMatrix, projection->GetMatrix());

		}
	}
}

void Enemy::Reset()
{

}

//�_���[�W����
void Enemy::Damage(float attack, const DirectX::SimpleMath::Matrix &rotation, bool blowAway, float power)
{
	//���G��Ԃ̎��̓_���[�W���������Ȃ�
	if (m_invincible) return;

	//�̗͂��U���͕����炷
	m_charaData.hp -= attack;

	//������я����ɕK�v�ȃf�[�^��ݒ�
	m_blowAwayRotation = rotation;
	m_blowAwayPower = power;

	//�������
	m_discovery = true;

	//true�̎��͍Ō�̃R���{
	m_playerAttackCombLast = blowAway;

	//�Ō�̃R���{�̎��ɐ�����я�Ԃɂ���
	if (m_playerAttackCombLast)
	{
		m_charaData.state = State::BlowAway;
	}

	//�G�t�F�N�g�̓����x
	m_hitEffectAlpha = 1.0f;
	m_pEnemyHit->SetMainPos(m_charaData.pos);
	//�v���C���[�Ɍ������x�N�g����ݒ�
	DirectX::SimpleMath::Vector3 vec = GameContext<CharacterMapList>::Get()->GetEntityFromID(m_playerID)->GetPos() - m_charaData.pos;
	vec.Normalize();
	m_pEnemyHit->SetVector(vec);
}





//�eUpdate�֐�-----------------
//State�֘A�̃��C����Update
void Enemy::StateMainUpdate()
{
	switch (m_charaData.state)
	{
	case State::Idle:
		break;

	case State::Move:
		//���ʂɈړ�����
		Move();
		break;

	case State::Battle:
		Battle();
		break;

	case State::Attack:
		//�U������
		Attack();
		break;

	case State::BlowAway:
		//�Ԃ���я���
		BlowAwayMove();
	default:
		break;
	}
}

//State�֘A�̍X�V����
void Enemy::StateUpdate()
{
	if (!m_alive) return;

	if (m_charaData.state != State::Battle &&
		m_charaData.state != State::Attack)
	{
		//��Ԃ̐ݒ�
		SelectState();

		//�^�[�Q�b�g�̈ʒu�̐ݒ�
		TargetosSet();

		//��]����
		Rotation(m_angle);
	}

	StateMainUpdate();
}

//�v���C���[�̃W���X�g������Ԃ̍X�V����
void Enemy::JustAvoidanceUpdate()
{
	if (!m_alive) return;

	//�W���X�g���
	if (m_justAvoidance)
	{
		m_justAvoidanceTime += m_elapsedTime;

		//3�b��������
		if (m_justAvoidanceTime >= SLOW_TIME)
		{
			//�ʏ��Ԃɖ߂�
			m_justAvoidance = false;
			m_justAvoidanceTime = 0.0f;
			m_speedCal = NORMAL_CAL;
		}
	}
}

//���G���Ԃ̍X�V����
void Enemy::InvincibleUpdate()
{
	if (!m_alive) return;

	if (m_invincible)
	{
		//���G���Ԃ̔���
		if (m_invincibleTime >= INVINCIBLE_TIME)
		{
			m_invincible = false;
			m_invincibleTime = 0.0f;
		}
		else
		{
			m_invincibleTime += m_elapsedTime;
		}
	}
}

//Map�Ƃ̓����蔻��
void Enemy::HitMap()
{
	if (!m_alive) return;

	//�G�l�~�[�̓����蔻��̐ݒ�
	Collision::Box3D enemy =
	{
		m_charaData.pos,
		DirectX::SimpleMath::Vector3(0.5f,1.0f,0.5f),
	};

	Map* map = GameContext<Map>::Get();
	//�ǂƂ̓����蔻��
	m_charaData.pos = map->HitWallBox(enemy);
	//�ړ����E
	m_charaData.pos = map->Limit(m_charaData.pos, m_charaData.thisRange);
}

//���S���̍X�V����
bool Enemy::DethUpdate(DX::StepTimer timer)
{
	//���S��ԂȂ�
	if (!m_alive)
	{
		//������Ȃ�
		if (m_dethEffectAlpha > 0.0f)
		{
			//�����x�̒���
			m_dethEffectAlpha -= m_elapsedTime;
			//�����x��n��
			m_pEnemyDeth->SetAlpha(m_dethEffectAlpha);
			//���S�G�t�F�N�g�̍X�V����
			m_pEnemyDeth->Update(timer);
		}
		return true;
	}

	return false;
}
//-----------------------------

//�e�`�揈��-------------------

//���S���̕`�揈��
void Enemy::DethDraw(const DirectX::SimpleMath::Matrix &viewMatrix, const DirectX::SimpleMath::Matrix &projection)
{
	//�����Ȃ��Ȃ�
	if (m_dethEffectAlpha < 0)
	{
		return;
	}

	m_pEnemyDeth->SetRenderState(m_eyePos, viewMatrix, projection);
	m_pEnemyDeth->Render();
}
//-----------------------------

//��]����
void Enemy::Rotation(float angle)
{
	//�G�l�~�[����^�[�Q�b�g�Ɍ������x�N�g��
	DirectX::SimpleMath::Vector3 enemy_target_vec = m_targetPos - m_charaData.pos;

	//�G�l�~�[�̐��ʂɌ������x�N�g��
	DirectX::SimpleMath::Vector3 frontVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, m_charaData.rotation);

	//���K��
	enemy_target_vec.Normalize();
	frontVec.Normalize();

	//�O��
	float cross = enemy_target_vec.x * frontVec.z - enemy_target_vec.z * frontVec.x;

	//��]�p�x
	//ONE_DEGREE * angle��1�b�Ԃɉ�]����l�����߂�
	//m_elapsedTime�������ĉ�]���x�̒���(1�b�ɍ��킹�邽�߂�)
	//m_speedCal�̓X���E��Ԃ̎��ƒʏ��Ԃ̎��̑��x��ς��邽��
	float rot = (((ONE_DEGREE * angle)*m_speedCal)* m_elapsedTime);

	//���E�ǂ��瑤�ɂ��邩�ŉ�]����������w��
	if (cross < 0)
	{
		m_charaData.angle.y -= rot;
	}
	else if (cross > 0)
	{
		m_charaData.angle.y += rot;
	}
}

//���ʂɈړ�����
void Enemy::Move()
{
	//�ړ����x�����߂�
	//�G�l�~�[�̐��ʂɌ������Ĉړ�
	DirectX::SimpleMath::Vector3 moveVec =
		DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, m_charaData.rotation)
		*m_charaData.speed;

	//m_speedCal�̓X���E��Ԃ̎��ƒʏ��Ԃ̎��̑��x��ς��邽��
	m_charaData.pos += ((moveVec * m_speedCal)* m_elapsedTime);
}

//Matrix�̐ݒ�
void Enemy::MatrixSetting()
{
	m_charaData.matrix = DirectX::SimpleMath::Matrix::CreateRotationY(m_charaData.angle.y);
	m_charaData.matrix *= DirectX::SimpleMath::Matrix::CreateRotationX(m_charaData.angle.x);
	m_charaData.matrix *= DirectX::SimpleMath::Matrix::CreateRotationZ(m_charaData.angle.z);

	m_charaData.rotation = DirectX::SimpleMath::Matrix::CreateRotationY(m_charaData.angle.y);
	m_charaData.rotation *= DirectX::SimpleMath::Matrix::CreateRotationX(m_charaData.angle.x);
	m_charaData.rotation *= DirectX::SimpleMath::Matrix::CreateRotationZ(m_charaData.angle.z);

	m_charaData.matrix *= DirectX::SimpleMath::Matrix::CreateTranslation(m_charaData.pos);
}

//�^�[�Q�b�g�̈ʒu�̐ݒ�
void Enemy::TargetosSet()
{
	m_targetPos = GameContext<CharacterMapList>::Get()->GetEntityFromID(m_playerID)->GetPos();
}

//�X�e�[�g�̐ݒ�
void Enemy::SelectState()
{
	if (m_charaData.state == State::BlowAway)
	{
		return;
	}

	//�X�e�[�g�̏�����
	m_charaData.state = State::Idle;

	//�ړ�����
	SelectMove();
	//�퓬�J�n����
	SelectBattle();
	//�U���J�n����
	SelectAttack();
	//��~����
	SelectStop();
}

//�f�[�^��ݒ肷��
void Enemy::DataSetting()
{
	SetPos(m_charaData.pos);
	SetHP(m_charaData.hp);
}

//�v���C���[��ID��ݒ�
void Enemy::SetPlayerID(int id)
{
	m_playerID = id;
}

//�����ȊO��Enemy��ݒ�
void Enemy::SetOtherEnemy(const std::vector<std::unique_ptr<Enemy>> &enemyList, int enemyNum)
{
	//-1����͎̂������܂߂Ȃ�����
	m_pOtherEnemyList.resize(enemyNum-1);

	int j = 0;
	for (int i = 0; i < enemyNum;i++)
	{
		//�����ȊO�̓G���i�[)
		if (enemyList[i]->GetID() != GetID())
		{
			m_pOtherEnemyList[j] = enemyList[i].get();
			j++;
		}
	}
}

//�͈͂ɂ��邩(�J�����`��͈�)
void Enemy::CameraWithinRange()
{
	Collision::Box3D player
	{
		GameContext<CharacterMapList>::Get()->GetEntityFromID(m_playerID)->GetPos(),
		//�`��͈͂̐ݒ�
		DirectX::SimpleMath::Vector3(60.0f,60.0f,60.0f),
	};

	Collision::Box3D enemy
	{
		m_charaData.pos,
		DirectX::SimpleMath::Vector3::One,
	};

	if (Collision::HitCheckoxBox3D(player,enemy))
	{
		m_checkCamera = true;
	}
	else
	{
		m_checkCamera = true;
	}
}

//���S����
bool Enemy::IsDeth()
{
	//HP��0�ɂȂ�����
	if (m_charaData.hp <= 0.0f)
	{
		//���S��ԂɕύX
		m_alive = false;
		m_charaData.state = State::Deth;
	}

	//���S��Ԃ̏���
	if (!m_alive)
	{
		//���S�G�t�F�N�g�̃f�[�^��ݒ�
		//�ʒu��ݒ�
		m_pEnemyDeth->SetMainPos(m_charaData.pos);
	}

	//���S�G�t�F�N�g���I��������
	if (m_dethEffectAlpha <= 0.0f)
	{

		return true;
	}

	return false;
}

//�J�����̈ʒu�̎擾
void Enemy::SetEysPos(const DirectX::SimpleMath::Vector3 &pos)
{
	m_eyePos = pos;
}

//HP�̉摜�T�C�Y�̐ݒ�
void Enemy::HPTextureSize()
{
	//�ő��HP�̂P���̒l�����߂�
	float hpOnePercent = m_charaData.maxHp * 0.01f;

	//���݂�HP�������c���Ă��邩���߂�
	float hpRemainingGuage = m_charaData.hp / hpOnePercent;

	//�摜�̉��̑傫���̂P�������߂�
	float textuerSizeOnePercent = 2.0f * 0.01f;

	//���߂�textuerSizeOnePercent��RemainingGuage���������摜���ǂꂭ�炢�\�����邩���߂�(X)
	float sizeX = textuerSizeOnePercent * hpRemainingGuage;

	m_pHPGuage->SetTextureScaleTL(DirectX::SimpleMath::Vector2(1.0f, 0.06f));
	m_pHPGuage->SetTextureScaleBR(DirectX::SimpleMath::Vector2(1.0f - sizeX, -0.06f));
}

void Enemy::BlowAwayMove()
{
	//�m�b�N�o�b�N(power�̐��l��1�b���Z�Ői��)
	DirectX::SimpleMath::Vector3 vec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, m_blowAwayRotation) * m_blowAwayPower;

	if (m_playerAttackCombLast&& m_blowAway)
	{
		//�ړ�����
		m_charaData.pos += (vec*m_elapsedTime);
		//���G���
		m_invincible = true;
	}

	//������ю��ԏI��
	if (m_blowAwayTime >= BLOW_AWAY_TIME)
	{
		m_charaData.state = State::Idle;
		m_blowAwayRotation = DirectX::SimpleMath::Matrix::Identity;
		m_blowAwayPower = 0.0f;
		m_blowAwayTime = 0.0f;
		m_playerAttackCombLast = false;
	}
	else
	{
		m_blowAwayTime += m_elapsedTime;
	}
}

//�X���E��Ԃ̊J�n
void Enemy::StartSlow()
{
	//�X���E��ԂɕύX
	m_justAvoidance = true;
	m_justAvoidanceTime = 0.0f;
	m_speedCal = SLOW_CAL;
}