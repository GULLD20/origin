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

void FollowCamera::Initialize()
{
	//�h������
	m_shapedWidth = DirectX::SimpleMath::Vector3::Zero;
	//�h��鎞�̃J�����̈ʒu
	m_shakeCameraPos = DirectX::SimpleMath::Vector3::Zero;;
	//�h�ꔻ��
	m_shake = false;
	m_shakeRange = 0.0f;
	m_shakeReverseCheck = false;
	m_rangeContraction = 0.0f;
}

void FollowCamera::Update()
{
	DirectX::SimpleMath::Vector3 up(0.0f, 1.0f, 0.0f);

	//m_EysPos += (m_refEyePos - m_EysPos) * 0.05f;
	//m_TargetPos += (m_refTargetPos - m_TargetPos) * 0.05f;

	m_shakeCameraPos = DirectX::SimpleMath::Vector3::Zero;

	if(m_shake)
	{
		Shake();
	}

	m_viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(m_eyePos + m_shakeCameraPos, m_targetPos + m_shakeCameraPos, up);
}

//�J�n�h��(�����͗h������)
void FollowCamera::StartShake(const DirectX::SimpleMath::Vector3 &vec)
{
	m_shapedWidth = vec;
	m_shake = true;
}

//�h�ꏈ��
void FollowCamera::Shake()
{
	//�h���͈͂�
	if (m_shakeReverseCheck)
	{
		m_shakeRange += SHAPED_WIDTH;
	}
	else
	{
		m_shakeRange -= SHAPED_WIDTH;
	}

	//
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