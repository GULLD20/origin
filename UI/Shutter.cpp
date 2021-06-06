#include "../pch.h"
#include "Shutter.h"

#include "../DeviceResources.h"
#include "../Scene/Projection.h"
#include "../Scene/GameContext.h"

//�G�t�F�N�g�p�̈ʒu�f�[�^(����)
const float Shutter::MAX_POS = 1.0f;

Shutter::Shutter()
{
}


Shutter::~Shutter()
{
	//�V�F�[�_�[�Ŏg���Ă����f�[�^�̍폜
	if (m_fade != nullptr)
	{
		m_fade->Lost();
		m_fade.reset(nullptr);
	}
}

//����
void Shutter::Create()
{
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();

	m_fade = std::make_unique<Fade>();
	m_fade->Create(deviceResources, 1);
}

//������
void Shutter::Initialize()
{
	//�t�F�[�h�C���E�A�E�g�m�F�p
	m_fadeCheck = false;
	//���Ԍv�Z�p
	m_fadeTimming = MAX_POS;
	//�t�F�[�h�C���E�A�E�g�J�n�p
	m_fadeIn = false;
	m_fadeOut = false;

	m_fade->InitializeNormal(5, DirectX::SimpleMath::Vector3::Zero);
}

//�X�V
void Shutter::Update(const DX::StepTimer &timer)
{
	//�t�F�[�h���Ȃ�
	if (m_fadeCheck)
	{
		//1�b����Ə����ɂ��邽��
		float elapsTime = static_cast<float>(timer.GetElapsedSeconds());

		//�t�F�[�h�C������
		if (m_fadeIn)
		{
			m_fadeTimming -= elapsTime;

			if (m_fadeTimming <= -MAX_POS)
			{
				m_fadeTimming = -MAX_POS;
				m_fadeIn = false;
				m_fadeCheck = false;
			}
		}
		//�t�F�[�h�A�E�g����
		else if(m_fadeOut)
		{
			m_fadeTimming += elapsTime;

			if (m_fadeTimming >= MAX_POS)
			{
				m_fadeTimming = MAX_POS;
				m_fadeOut = false;
				m_fadeCheck = false;
			}
		}
	}

	m_fade->Update(timer);
	m_fade->SetFadeTimming(m_fadeTimming);
}

//�`��
void Shutter::Draw()
{
	Projection* projection = GameContext<Projection>().Get();
	DirectX::SimpleMath::Vector3 eys = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::Identity;

	m_fade->SetRenderState(eys, view, projection->GetMatrix());
	m_fade->Render();
}

//���Z�b�g
void Shutter::Reset()
{

}


//�t�F�[�h�C��
void Shutter::FadeIn()
{
	m_fadeCheck = true;
	m_fadeIn = true;
}

//�t�F�[�h�A�E�g
void Shutter::FadeOut()
{
	m_fadeCheck = true;
	m_fadeOut = true;
}

//�t�F�[�h�C���E�A�E�g�̊m�F
bool Shutter::GetFade() const
{
	return m_fadeCheck;
}