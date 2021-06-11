#pragma once

#include "../Enemy.h"

#include "../../MyEffect/Effect2D/Enemy/AttackArea.h"


///======================================================
///���e
///�W�����v���ăv���C���[�̏ォ�痎�����čU������G
///======================================================
class Stamp : public Enemy
{
public:

public:
	//�U���͈�
	const static float ATTACK_RANGE;
	//�U�����̃W�����v�̎���
	const static float ATTACKJUMPTIME;
	//�U�����̈ړ��̎���
	const static float ATTACKMOVETIME;
	//�U�����̗����̎���
	const static float ATTACKFALLTIME;
	//�U���Ԋu
	const static int   ATTACK_INTERVAL;
	//�퓬�͈�
	const static float BATTLE_RANGE;
	//�U�����̍ő�̍����̈ʒu
	const static float ATTACKTOPPOS;

	//�v���C���[�𐁂���΂���(1�b�Ԃ�)
	const static float BLOW_AWAY_POWER_PLAYER_TARGET;

	//�G�t�F�N�g�̌�
	//�U���͈�
	const static int   NUMBER_OF_EFFECT_ATTACK_AREA;
	//�U��������������
	const static int   NUMBER_OF_EFFECT_HIT;
	//���S
	const static int   NUMBER_OF_EFFECT_DETH;

	//�X���E��Ԃ̌v�Z�p�̐��l
	const static float SLOW_CAL;
	//�ʏ��Ԃ̌v�Z�p�̐��l
	const static float NORMAL_CAL;

public:
	Stamp(const DirectX::SimpleMath::Vector3 &pos, float attack, float hp, float speed, float thisRange);
	~Stamp();

	//�G�t�F�N�g�̍쐬
	void CreateEffect() override;

	//�h���N���X�p�X�V����
	void SubInitialize() override;
	//�G�t�F�N�g�̏�����
	void EffectInitialize() override;

	//�h���N���X�p�X�V����
	void SubUpdate()override;
	//�G�t�F�N�g�̍X�V����
	void EffectUpdate(const DX::StepTimer &timer)override;

	//�������̕`�揈��
	void AliveDraw(const DirectX::SimpleMath::Matrix &viewMatrix, const DirectX::SimpleMath::Matrix &projection)override;

	//�퓬������
	void Battle()override;
	//�U������
	void Attack()override;
	//��~����
	void Stop()override;


	//�ړ�����
	void SelectMove()override;
	//�퓬�J�n����
	void SelectBattle()override;
	//�U���J�n����
	void SelectAttack()override;
	//��~����
	void SelectStop()override;

	//�v���C���[�̃W���X�g��𔻒�
	void PlayerJustAvoidance()override;

	//�U���I��
	void EndAttack()override;

private:
	//�U�����̏���
	//�W�����v
	void AttackJump();
	//�W�����v��̈ړ�
	void AttackMove();
	//����
	void AttackFall();
private:
	//�U���͈�
	std::unique_ptr<AttackArea>  m_pAttackArea;

	//�U���J�n����
	bool                         m_startAttack;
	//�W�����v���̖ړI�ʒu
	DirectX::SimpleMath::Vector3 m_jumpDestination;
	//�U�����̈ړ��J�n�ʒu
	DirectX::SimpleMath::Vector3 m_startMovePos;

	//�U���̃W�����v�̎��Ԍv��
	float                        m_attackJumpTimer;
	//�U���̈ړ��̎��Ԍv��
	float                        m_attackMoveTimer;
	//�U���̗����̎��Ԍv��
	float                        m_attackFallTimer;

	//�U�����̊e����
	//�W�����v�I��
	bool                         m_attackJump;
	//�U���I��
	bool                         m_attackMove;
	//�����I��
	bool                         m_attackFall;

	//�U�����Ƀf�[�^�̏�������1�t���[���]�T���������邽��
	int                          m_oneFrameWait;

	//�U���I������
	bool                         m_endAttack;
};