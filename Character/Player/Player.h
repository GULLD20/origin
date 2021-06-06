#pragma once

#include "Model.h"

#include <SimpleMath.h>
#include <Keyboard.h>
#include <Effects.h>
#include "../../DeviceResources.h"
#include <CommonStates.h>
#include "../../Scene/Projection.h"
#include "../../Scene/GameContext.h"

#include "../../MyEffect/Effect2D/HP/HPGuage.h"

#include "../Character/Character.h"

class Weapon;

class DebugCapsule;

class HPGuage;

enum class WeaponType;

class ADX2;

struct AvoidanceData
{
	//������Ԍv��
	float TimeCount;
	//������̑���
	float spead;
	//�N�[���^�C��
	float coolTimeCount;
	//�������������
	bool Check;
};

class Player:public Character
{
public:  //enum�E�萔
	//����
	enum Dir
	{
		None = 0,
		Front,
		Back,
		Left,
		Right
	};

	//�v���C���[�̑傫���̏�Ɖ�
	const static DirectX::SimpleMath::Vector3 PLAYER_TOP_DOWN;

	//�������
	const static float                        AVOIDANCETIME;
	//����̃N�[���^�C��
	const static float                        AVOIDANCECOOLTIME;
	//������΂���鎞��
	const static float                        BLOW_AWAY_TIME;
	//���G����
	const static float                        INVINCIBLE_TIME;

	//�G�l�~�[�𐁂���΂���(1�b�Ԃ�)
	const static float                        BLOWAWAYPOWER_ENEMYTARGET;

public:  //�R���X�g�E�f�X�g
	Player(const DirectX::SimpleMath::Vector3 &pos, float attack, float hp, float speed, float thisRang);
	~Player();

public:  //�p�������֐�
	//����������
	void Initialize()override;
	//�X�V����
	void Update(const DX::StepTimer &timer)override;
	//�`�揈��
	void Draw(const DirectX::SimpleMath::Matrix &viewMatrix)override;
	//���Z�b�g����
	void Reset()override;

	//�_���[�W����
	void Damage(float attack, const DirectX::SimpleMath::Matrix &rotation, bool blowAway , float power)override;

	//������я���
	virtual void BlowAwayMove()override;

public:  //���̃N���X�ō쐬���ꂽ�֐�
	//���f���ƕ���̍쐬
	void Create(DX::DeviceResources* &DeviceResources, const WeaponType &type);

	//�U����������������
	void HitAttack(int id);
	//�G�l�~�[�Ɠ������Ă���Ƃ�
	void HItEnemy(int id);
	//Matrix�̐ݒ�
	void MatrixSetting();
private:  //���̃N���X���ł̂݌Ăяo�����֐�
	//���G���Ԃ̍X�V����
	void InvincibleUpdate();
	//�p�x�̐���
	DirectX::SimpleMath::Vector3 LimitsAngle(const DirectX::SimpleMath::Vector3 &angle);


	//�ړ�����
	void Move();

	//�U������
	void Attack();
	//�U���̌��ʉ��̍Đ�
	void PlayAttackSE();
	//�U�����̈ړ����x�����߂�
	DirectX::SimpleMath::Vector3 CalAttackMoveVel(const DirectX::SimpleMath::Vector3 &front, const DirectX::SimpleMath::Vector3 &enemyPos);
	//�����̐��ʂɂ���G�̒��ň�ԋ߂��G�̈ʒu���擾
	DirectX::SimpleMath::Vector3 EnemyDistanceClosePos(std::vector<int> allKey,const DirectX::SimpleMath::Vector3 &frontVecN);

	//�����Update����(Update�ŌĂяo������	)
	void AvoidanceUpdate();
	//����̈ړ�����
	void AvoidanceMove();
	//�W���X�g���
	void JusrAvoidance();
	//�v���C���[�̉�]
	void Rotation();

	//�L�[����
	void InputKey();
private:  //�����o�ϐ�
	//����
	std::unique_ptr<Weapon>                         m_pWeapon;

	//���f��
	std::unique_ptr<DirectX::Model>                 m_pModel;

	//�e�̃��f��
	std::unique_ptr<DirectX::Model>                 m_shadow;

	//HP�Q�[�W
	std::unique_ptr <HPGuage>                       m_pHPGauge;
	//�U����������
	bool                                            m_attack;
	//�ړ����Ă��邩����
	bool                                            m_moveCheck;

	//�U���L�[�����
	bool                                            m_attackKeyDown;

	//����Ɋ֌W����f�[�^
	AvoidanceData                                   m_avoidance;

	//1�t���[���O�̃R���{
	int                                             m_oldCombo;

	//SE�̍Đ�����
	bool                                            m_playSE;

	//���G���Ԏ��̃t���[���v�Z
	int                                             m_invincibleFrameCount;

	//������ԕ����̊p�x(�s��)
	DirectX::SimpleMath::Matrix                     m_blowAwayRotation;
	//������ԗ�(1�b��)
	float                                           m_blowAwayPower;
	//������ю��Ԃ̌v��
	float                                           m_blowAwayTime;

	//1�t���[���ɂ�����������(���v�����1�b�ɂȂ�)
	float                                           m_elapsedTime;

	//�����x
	DirectX::SimpleMath::Vector3                    m_acceleration;
	//���x
	DirectX::SimpleMath::Vector3                    m_velocity;
	//�L�[���͂��������̃x�N�g��
	DirectX::SimpleMath::Vector3                    m_inputKeyVec;

	//std::unique_ptr<DebugCapsule>                 m_pDebugCapsulePlayer;
};