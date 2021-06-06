#pragma once

#include "SimpleMath.h"

class FollowCamera
{
public:
	//�h���͈�(�ő�)
	const static int MAX_SHAKE_RANGE;
	//�͈͂̎��k����l
	const static float CONTRACTION_RANGE;
	//�h��镝
	const static float SHAPED_WIDTH;
public:
	FollowCamera();
	~FollowCamera();

	void Initialize();
	void Update();

	//�J�n�h��(�����͗h������)
	void StartShake(const DirectX::SimpleMath::Vector3 &vec);
	//�h�ꏈ��
	void Shake();

	//�r���[�s��̎擾
	DirectX::SimpleMath::Matrix GetView()const;
	//�r���[�s��̐ݒ�
	void SetView(const DirectX::SimpleMath::Matrix &view);

	//�v���W�F�N�V�����s��̎擾
	DirectX::SimpleMath::Matrix GetProjection()const;
	//�v���W�F�N�V�����s��̐ݒ�
	void SetProjection(const DirectX::SimpleMath::Matrix &proj);

	//�J�����̌���Ώۂ�ݒ�
	void SetTargetPosition(const DirectX::SimpleMath::Vector3 &targetPos);

	//�J�����̈ʒu�̎擾
	DirectX::SimpleMath::Vector3 GetEysPos()const;
	//�J�����̈ʒu�̐ݒ�
	void SetEyePosition(const DirectX::SimpleMath::Vector3 &eyePos);

private:
	//�r���[�s��
	DirectX::SimpleMath::Matrix  m_viewMatrix;
	//�ˉe�s��
	DirectX::SimpleMath::Matrix  m_pProjectionMatrix;

	//���q���_
	DirectX::SimpleMath::Vector3 m_targetPos;

	//���_�̋���
	DirectX::SimpleMath::Vector3 m_eyePos;

	DirectX::SimpleMath::Vector3 m_up;

	// �Q�Ǝ��_
	DirectX::SimpleMath::Vector3 m_refEyePos;

	// �Q�ƒ����_
	DirectX::SimpleMath::Vector3 m_refTargetPos;

	//�h������(��)
	DirectX::SimpleMath::Vector3 m_shapedWidth;
	//�h��鎞�̃J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_shakeCameraPos;
	//�h�ꔻ��
	bool                         m_shake;
	//���ۂ̃J�����̗h��
	float                        m_shakeRange;
	//�h��̔��]
	bool                         m_shakeReverseCheck;
	//�h��͈̔�
	float                        m_rangeContraction;
};