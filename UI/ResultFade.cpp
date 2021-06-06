#include "../pch.h"
#include "ResultFade.h"

#include "../DeviceResources.h"
#include "../Scene/Projection.h"
#include "../Scene/GameContext.h"

//�t�F�[�h�̓����x
const float ResultFade::MAX_ALPHA = 0.7f;

ResultFade::ResultFade()
{
}


ResultFade::~ResultFade()
{
	if (m_pBlack != nullptr)
	{
		m_pBlack->Lost();
		m_pBlack.reset();
	}
}

//����
void ResultFade::Create()
{
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();

	m_pBlack = std::make_unique<Black>();
	m_pBlack->Create(deviceResources, 1);
}

//������
void ResultFade::Initialize()
{
	//�t�F�[�h�m�F�p
	m_fadeCheck = false;
	//�����x�v�Z�p
	m_alpha = 0.0f;
	//�t�F�[�h�C���J�n�p
	m_fadeIn = false;

	m_pBlack->InitializeNormal(5, DirectX::SimpleMath::Vector3(0, 0, 0));
}

//�X�V
void ResultFade::Update(DX::StepTimer timer)
{
	//�t�F�[�h�����̔���
	if (m_fadeCheck)
	{
		//�t�F�[�h�C���̏���
		if (m_fadeIn)
		{
			//���񂾂�Ɠ����x���グ��
			m_alpha += static_cast<float>(timer.GetElapsedSeconds());
		}

		//�t�F�[�h���ݒ肵�������x�ɒB������
		if (m_alpha >= MAX_ALPHA)
		{
			m_alpha = MAX_ALPHA;
			//�t�F�[�h�����̏I��
			m_fadeCheck = false;
		}
	}

	//�V�F�[�_�[����
	m_pBlack->Update(timer);
	m_pBlack->SetAlpha(m_alpha);
}

//�`��
void ResultFade::Draw()
{
	Projection* projection = GameContext<Projection>().Get();
	DirectX::SimpleMath::Vector3 eys = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::Identity;

	//�t�F�[�h�̕`��
	m_pBlack->SetRenderState(eys, view, projection->GetMatrix());
	m_pBlack->Render();
}

//���Z�b�g
void ResultFade::Reset()
{

}


//�t�F�[�h�C���̊J�n
void ResultFade::FadeIn()
{
	m_fadeCheck = true;
	m_fadeIn = true;
}

//�t�F�[�h�C���̊m�F�@Get
bool ResultFade::GetFade() const
{
	return m_fadeCheck;
}