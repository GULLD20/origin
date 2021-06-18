#pragma once

#include "../Enemy.h"

#include "../../MyEffect/Effect2D/Enemy/AttackLine.h"

///======================================================
///���e
///�v���C���[�Ɍ������ēːi�𑱂���
///======================================================
class Goblin : public Enemy
{
public:   //�萔
	//�U������
	const static float ATTACK_TIME;
	//�U���͈�
	const static float ATTACK_RANGE;
	//�U���Ԋu
	const static int   ATTACK_INTERVAL;
	//�퓬�͈�
	const static float BATTLE_RANGE;

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

public:   //�R���X�g�E�f�X�g
	Goblin(const DirectX::SimpleMath::Vector3 &pos, float attack, float hp, float speed, float thisRange);
	~Goblin();

public:   //�֐��ioverride�j
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

private:   //�����o�ϐ�
	//�U���͈�
	std::unique_ptr<AttackLine> m_pAttackLine;
};