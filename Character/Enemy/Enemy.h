#pragma once

#include <GeometricPrimitive.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <CommonStates.h>
#include <vector>
#include "Model.h"
#include <string>
#include "../../DeviceResources.h"

#include "../../Scene/Projection.h"
#include "../../Scene/GameContext.h"

#include "../../DebugObj/DebugCapsule.h"

#include "../../MyEffect/Effect3D/Effect3D.h"

#include "../Character/Character.h"

#include "../../MyEffect/Effect2D/Enemy/EnemyDeth.h"
#include "../../MyEffect/Effect2D/Enemy/EnemyHit.h"

class DX::DeviceResources;

class Enemy: public Character
{
public:

public:
	//�G�̑傫����Ɖ�
	const static DirectX::SimpleMath::Vector3  ENEMYSIZE_TOP_DOWN;


	//������΂���鎞��
	const static float                         BLOW_AWAY_TIME;

	//���G����
	const static float                         INVINCIBLE_TIME;

	//�X���E��Ԃ̌v�Z�p�̐��l
	const static float                         SLOW_CAL;
	//�ʏ��Ԃ̌v�Z�p�̐��l
	const static float                         NORMAL_CAL;

	//�X���E����
	const static float                         SLOW_TIME;
public: //�R���X�g�@�f�X�g
	Enemy(const DirectX::SimpleMath::Vector3 &pos, float attack, float hp, float speed, float thisRange);
	virtual ~Enemy();

public: //���N���X���p�������֐�

	//���N���X�ŏ����֐�
	//���f���̍쐬
	void CreateModel(const wchar_t* fileName);
	//����������
	void Initialize()override;
	//�X�V����
	void Update(const DX::StepTimer &timer)override;
	//�`�揈��
	void Draw(const DirectX::SimpleMath::Matrix &viewMatrix)override;
	//���Z�b�g����
	void Reset()override;

	//�_���[�W����
	void Damage(float attack, const DirectX::SimpleMath::Matrix &rotation, bool blowAway, float power)override;

	//������я���
	void BlowAwayMove()override;

public:
	//�eUpdate�֐�-----------------
	//State�֘A�̍X�V����
	void StateUpdate();
	//State�֘A�̃��C����Update
	void StateMainUpdate();
	//�v���C���[�̃W���X�g������Ԃ̍X�V����
	void JustAvoidanceUpdate();
	//���G���Ԃ̍X�V����
	void InvincibleUpdate();
	//���S���̍X�V����
	bool DethUpdate(DX::StepTimer timer);
	//Map�Ƃ̓����蔻��
	void HitMap();
	//-----------------------------

	//���S���̕`�揈��
	void DethDraw(const DirectX::SimpleMath::Matrix &viewMatrix, const DirectX::SimpleMath::Matrix &projection);

	//��]����
	void Rotation(float angle);

	//���ʂɈړ�����
	void Move();

	//Matrix�̐ݒ�
	void MatrixSetting();

	//�^�[�Q�b�g�̈ʒu�̐ݒ�
	void TargetosSet();

	//�X�e�[�g�̐ݒ�
	void SelectState();

	//�f�[�^��ݒ肷��
	void DataSetting();

	//�v���C���[��ID��ݒ�
	void SetPlayerID(int id);

	//�����ȊO��Enemy��ݒ�
	void SetOtherEnemy(const std::vector<std::unique_ptr<Enemy>> &enemyList, int enemyNum);

	//�͈͂ɂ��邩(�J�����`��͈�)
	void CameraWithinRange();

	//���S����
	bool IsDeth();

	//�J�����̈ʒu�̎擾
	void SetEysPos(const DirectX::SimpleMath::Vector3 &pos);

	//HP�̉摜�T�C�Y�̐ݒ�
	void HPTextureSize();

	//�X���E��Ԃ̊J�n
	void StartSlow();

	//�X���E��Ԃ��擾
	bool GetSlow() const { return m_justAvoidance; };

public:
	//�h���N���X�ŏ����֐�
	//�G�t�F�N�g�̍쐬
	virtual void CreateEffect() = 0;
	//�h���N���X�p�X�V����
	virtual void SubInitialize() = 0;
	//�G�t�F�N�g�̏�����
	virtual void EffectInitialize() = 0;

	//�G�t�F�N�g�̍X�V����
	virtual void EffectUpdate(const DX::StepTimer &timer) =0;
	//�h���N���X�p�X�V����
	virtual void SubUpdate() = 0;

	//�������̕`�揈��
	virtual void AliveDraw(const DirectX::SimpleMath::Matrix &viewMatrix, const DirectX::SimpleMath::Matrix &projection) = 0;

	//�퓬������
	virtual void Battle() = 0;
	//�U������
	virtual void Attack() = 0;
	//��~����
	virtual void Stop() = 0;

	//�ړ�����
	virtual void SelectMove() = 0;
	//�퓬�J�n����
	virtual void SelectBattle() = 0;
	//�U���J�n����
	virtual void SelectAttack() = 0;
	//��~����
	virtual void SelectStop() = 0;

	//�v���C���[�̃W���X�g��𔻒�
	virtual void PlayerJustAvoidance() = 0;

	//�U���I��
	virtual void EndAttack() = 0;

public:
	//����𔭌�
	bool                                            m_discovery;
	//�G�l�~�[�������Ă��邩����
	bool                                            m_alive;

	//���f��
	std::unique_ptr<DirectX::Model>                 m_pModel;

	//�e�̃��f��
	std::unique_ptr<DirectX::Model>                 m_shadow;
	//�X�s�[�h�v�Z�p
	float                                           m_speedCal;
	//�p�j�ʒu���w�肷��
	int                                             m_wanderingAssignment;
	//�͈͓��ɂ��邩�m�F
	bool                                            m_withinRange;
	//�����Ă��邩����
	bool                                            m_walk;
	//�ҋ@�t���[��
	int                                             m_waitFrameCount;
	//�ҋ@���̊p�x�ύX�p(Lerp����p)
	float                                           m_lerpAdjustment;
	//�v���C���[�͈͓̔�(�`�攻��)
	bool                                            m_checkCamera;
	//1�b�Ԃ̉�]�p�x
	float                                           m_angle;

	//1�t���[���O�̂Ȃ��p
	float                                           m_oldAco;

	//HP�\��
	std::unique_ptr <Effect3D>                      m_pHPGuage;
	//���O�\��
	std::unique_ptr <Effect3D>                      m_pName;
	//�U���������������̉��o(�v���C���[�̍U�����G�l�~�[�ɓ���������)
	std::unique_ptr<EnemyHit>                       m_pEnemyHit;
	//���S���o
	std::unique_ptr<EnemyDeth>                      m_pEnemyDeth;

	//�U���������������̃G�t�F�N�g�̃A���t�@�l
	float                                           m_hitEffectAlpha;
	//���S�G�t�F�N�g�̃A���t�@�l
	float                                           m_dethEffectAlpha;

	//�J�����̈ʒu
	DirectX::SimpleMath::Vector3                    m_eyePos;

	//�^�[�Q�b�g�̈ʒu
	DirectX::SimpleMath::Vector3                    m_targetPos;

	//�U���̊J�n�E�I������
	bool                                            m_attack_start_end;

	float                                           m_attackTimer;

	//�U���̊Ԋu
	float                                           m_attackInterval;
	//1�t���[���ɂ�����������
	float                                           m_elapsedTime;

	//�W���X�g���
	//�X���E��Ԃ�
	bool                                            m_justAvoidance;
	//�X���E��Ԃ��I������܂ł̎���
	float                                           m_justAvoidanceTimer;

	//�v���C���[��ID
	int                                             m_playerID;
	//�S�Ă�Enemy
	std::vector<Enemy*>                             m_pOtherEnemyList;

	//�G�l�~�[�̐�����є���(true�Ȃ琁����ԁEfalse�Ȃ琁����΂Ȃ�   �G�l�~�[�ɂ���ĕς���)
	bool                                            m_blowAway;

	//�v���C���[�̍U���̃R���{���Ōォ
	bool                                            m_playerAttackCombLast;

	//������ԕ����̊p�x(�s��)
	DirectX::SimpleMath::Matrix                     m_blowAwayRotation;
	//������ԗ�(1�b��)
	float                                           m_blowAwayPower;
	//������ю��Ԃ̌v��
	float                                           m_blowAwayTimer;
};