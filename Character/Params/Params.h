#pragma once
class Params
{
public:
	//�G�̎��
	enum EnemyTyp
	{
		Goblin = 0,
		LongRange,
		LongRangeRowling,
		Stamp,
		Rowling
	};
public:
	///�v���C���[�̃X�e�[�^�X;
	//�U����
	const static float PLAYER_ATTACK;
	//�̗�
	const static float PLAYER_HP;
	//���x(1�b�Ԃɐi�ޑ��x)
	const static float PLAYER_SPEED;
	//�����蔻��̔��a
	const static float PLAYER_RANGE;

public:
	///�S�u�����̃X�e�[�^�X;
	//�U����
	const static float GOBLIN_ATTACK;
	//�̗�
	const static float GOBLIN_HP;
	//���x(1�b�Ԃɐi�ޑ��x)
	const static float GOBLIN_SPEED;
	//�����蔻��̔��a
	const static float GOBLIN_RANGE;

public:
	///�������U���̓G�̃X�e�[�^�X
	//�U����
	const static float LONGRANGE_ATTACK;
	//�̗�
	const static float LONGRANGE_HP;
	//���x(1�b�Ԃɐi�ޑ��x)
	const static float LONGRANGE_SPEED;
	//�����蔻��̔��a
	const static float LONGRANGE_RANGE;
public:
	///��]���鉓�����U���̓G�̃X�e�[�^�X
	//�U����
	const static float LONGRANGEROWLING_ATTACK;
	//�̗�
	const static float LONGRANGEROWLING_HP;
	//���x(1�b�Ԃɐi�ޑ��x)
	const static float LONGRANGEROWLING_SPEED;
	//�����蔻��̔��a
	const static float LONGRANGEROWLING_RANGE;

public:
	///�����Ԃ��G�̃X�e�[�^�X
	//�U����
	const static float STAMP_ATTACK;
	//�̗�
	const static float STAMP_HP;
	//���x(1�b�Ԃɐi�ޑ��x)
	const static float STAMP_SPEED;
	//�����蔻��̔��a
	const static float STAMP_RANGE;

public:
	///��]�U���̓G�̃X�e�[�^�X
	//�U����
	const static float ROWLING_ATTACK;
	//�̗�
	const static float ROWLING_HP;
	//���x(1�b�Ԃɐi�ޑ��x)
	const static float ROWLING_SPEED;
	//�����蔻��̔��a
	const static float ROWLING_RANGE;
};

