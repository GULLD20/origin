#pragma once

#include <vector>
#include <Model.h>
#include <SimpleMath.h>

#include "../../../MyLib/Collision.h"

class LongRangeAttack
{
public:
	struct Bullet
	{
		//�e�̈ʒu
		DirectX::SimpleMath::Vector3 pos;
		//�e�̔��ˎ��̈ʒu
		DirectX::SimpleMath::Vector3 startPos;
		//���[���h�s��
		DirectX::SimpleMath::Matrix matrix;
		//��]�݂̂̍s��
		DirectX::SimpleMath::Matrix rotation;
		//�����蔻��
		Collision::Capsule3D capsule3D;
		//�g���Ă��邩�ǂ���
		bool active;
	};

public:
	//�ړ����x
	const static float                        SPEED;
	//�����蔻��̒��S�����[�̒���(Vector3)
	const static DirectX::SimpleMath::Vector3 BASE_CENTER_TOP;
	//�����蔻��̉~�̑傫��
	const static float                        CIRCLE_SIZE;

public:
	//�R���X�g�E�f�X�g
	LongRangeAttack();
	~LongRangeAttack();

	//���f���̍쐬
	void CreatModel();
	//����������
	void Initialize(float speed ,int attackRange);
	//�X�V����
	void Update(float elapsedTime);
	//�`�揈��
	void Draw(const DirectX::SimpleMath::Matrix &viewMatrix, const DirectX::SimpleMath::Matrix &projection);
	//���Z�b�g
	void Reset();

	//�U���J�n����
	void Shot(const DirectX::SimpleMath::Vector3 &pos, const DirectX::SimpleMath::Matrix &rotation);

	//�U�������������Ȃ�
	void HitAttack();

	//�����蔻���Get
	Collision::Capsule3D GetCapsuleData(const DirectX::SimpleMath::Vector3 &playerPos);

	void SetSpeedCal(float speedCal) { m_speedCal = speedCal; };

private:
	//�ړ�����
	void Move(int select, float elapsedTime);
	//�U���I������
	void AttackEnd(int select);
	//�����蔻��̐ݒ�
	void CapsuleDataSetting(int select);
	//�s��̐ݒ�
	void MatrixSetting(int select);

private:
	//�e
	std::vector<Bullet>             m_bulletList;
	//���f��
	std::unique_ptr<DirectX::Model> m_pModel;
	//���x
	float                           m_speed;
	//�U���͈�
	int                             m_attackRange;
	//�X�s�[�h�v�Z�p
	float                           m_speedCal;
	//�Ԃ����J�v�Z���f�[�^(�����蔻��̃f�[�^)�̗v�f�ʒu
	int                             m_capsuleSelect;
};