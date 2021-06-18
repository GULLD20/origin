#include "../pch.h"
#include "FollowCamera.h"

//�h���͈�(�ő�)
const int FollowCamera::MAX_SHAKE_RANGE = 1;

//�͈͂̎��k����l
const float FollowCamera::CONTRACTION_RANGE = 0.025f;
//�h��镝
const float FollowCamera::SHAPED_WIDTH = 0.45f;

FollowCamera::FollowCamera()
{
	m_eyePos = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 10.0f);
	m_targetPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	m_up = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);

	m_viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(m_eyePos, m_targetPos, m_up);

	m_refEyePos = DirectX::SimpleMath::Vector3(5.0f, 5.0f, 5.0f);
	m_refTargetPos= DirectX::SimpleMath::Vector3(5.0f, 5.0f, 5.0f);
}


FollowCamera::~FollowCamera()
{
}

//������
void FollowCamera::Initialize()
{
	//�h������
	m_shapedWidth = DirectX::SimpleMath::Vector3::Zero;
	//�h��鎞�̃J�����̈ʒu
	m_shakeCameraPos = DirectX::SimpleMath::Vector3::Zero;;
	//�h�ꔻ��
	m_shake = false;
	//���ۂ̃J�����̗h��
	m_shakeRange = 0.0f;
	//�h��̔��]
	m_shakeReverseCheck = false;
	//�h��͈̔�
	m_rangeContraction = 0.0f;
}

//�X�V����
void FollowCamera::Update()
{
	//�J�����̗h�ꂽ�Ƃ��̈ʒu�̏�����
	m_shakeCameraPos = DirectX::SimpleMath::Vector3::Zero;

	//�h��鏈�����s��������
	if(m_shake)
	{
		//�h��鏈��
		Shake();
	}

	//�}�g���b�N�X�̐ݒ�
	m_viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(m_eyePos + m_shakeCameraPos, m_targetPos + m_shakeCameraPos, m_up);
}

//�h��̊J�n(�����͗h������)
void FollowCamera::StartShake(const DirectX::SimpleMath::Vector3 &vec)
{
	m_shapedWidth = vec;
	m_shake = true;
}

//�h�ꏈ��
void FollowCamera::Shake()
{
	//�J�����̈ʒu�̒���
	if (m_shakeReverseCheck)
	{
		m_shakeRange += SHAPED_WIDTH;
	}
	else
	{
		m_shakeRange -= SHAPED_WIDTH;
	}

	//0�𒆐S�Ƃ��ĕ�����
	if (m_shakeRange >= 0.0f)
	{
		m_shakeCameraPos = DirectX::SimpleMath::Vector3::Lerp(DirectX::SimpleMath::Vector3::Zero, m_shapedWidth, m_shakeRange);
	}
	else
	{
		m_shakeCameraPos = DirectX::SimpleMath::Vector3::Lerp(-m_shapedWidth, DirectX::SimpleMath::Vector3::Zero, MAX_SHAKE_RANGE + m_shakeRange);
	}

	//���]�����Ɨh��͈̔͂����߂�
	if (MAX_SHAKE_RANGE - m_rangeContraction < m_shakeRange)
	{
		//���]
		m_shakeReverseCheck = !m_shakeReverseCheck;
		//�͈͂̎��k
		m_rangeContraction += CONTRACTION_RANGE;

		//�ʒu�̏C��
		m_shakeRange = MAX_SHAKE_RANGE - m_rangeContraction;
	}
	//���]����
	else if(-MAX_SHAKE_RANGE + m_rangeContraction > m_shakeRange)
	{
		//���]
		m_shakeReverseCheck = !m_shakeReverseCheck;
		//�͈͂̎��k
		m_rangeContraction += CONTRACTION_RANGE;
		//�h�ꕝ�̒���
		m_shakeRange = -MAX_SHAKE_RANGE + m_rangeContraction;
	}

	//�h��I��
	if (m_rangeContraction >= MAX_SHAKE_RANGE)
	{
		m_shake = false;
	}
}

//�r���[�s��̎擾
DirectX::SimpleMath::Matrix FollowCamera::GetView() const
{
	return m_viewMatrix;
}

//�r���[�s��̐ݒ�
void FollowCamera::SetView(const DirectX::SimpleMath::Matrix &view)
{
	m_viewMatrix = view;
}


//�v���W�F�N�V�����s��̎擾
DirectX::SimpleMath::Matrix FollowCamera::GetProjection() const
{
	return m_pProjectionMatrix;
}

//�v���W�F�N�V�����s��̐ݒ�
void FollowCamera::SetProjection(const DirectX::SimpleMath::Matrix &proj)
{
	m_pProjectionMatrix = proj;
}


//�J�����̌���Ώۂ�ݒ�
void FollowCamera::SetTargetPosition(const DirectX::SimpleMath::Vector3 &targetPos)
{
	m_targetPos = targetPos;
}

//�J�����̈ʒu�̎擾
DirectX::SimpleMath::Vector3 FollowCamera::GetEysPos() const
{
	return m_eyePos;
}

//�J�����̈ʒu�̐ݒ�
void FollowCamera::SetEyePosition(const DirectX::SimpleMath::Vector3 &eyePos)
{
	m_eyePos = eyePos;
}