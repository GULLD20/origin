#include "../pch.h"
#include "ScreenCapture.h"

#include "../DeviceResources.h"
#include <SpriteBatch.h>

#include "../Scene/GameContext.h"
#include "../Aspect.h"

ScreenCapture::ScreenCapture()
{
}


ScreenCapture::~ScreenCapture()
{
	//�����_�[�^�[�Q�b�g�e�N�X�`��
	if (m_pRenderTexture != nullptr)
	{
		m_pRenderTexture->Release();
		m_pRenderTexture = nullptr;
	}

	//�����_�[�^�[�Q�b�g�r���[
	if (m_pRenderTargetView != nullptr)
	{
		m_pRenderTargetView->Release();
		m_pRenderTargetView = nullptr;
	}

	if (m_pScreenBlur != nullptr)
	{
		m_pScreenBlur->Lost();
		m_pScreenBlur.reset(nullptr);
	}

	if (m_pTexture != nullptr)
	{
		m_pTexture.Reset();
		m_pTexture = nullptr;
	}

}

//������
void ScreenCapture::Initialize()
{
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>().Get();
	ID3D11Device1* device = deviceResources->GetD3DDevice();

	//�����_�[�^�[�Q�b�g�e�N�X�`�����쐬
	memset(&m_screenData, 0, sizeof(m_screenData));
	m_screenData.Width = static_cast<UINT>(Aspect::ASPECT_X);
	m_screenData.Height = static_cast<UINT>(Aspect::ASPECT_Y);
	m_screenData.MipLevels = 1;
	m_screenData.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_screenData.SampleDesc.Count = 1;
	m_screenData.Usage = D3D11_USAGE_DEFAULT;
	m_screenData.ArraySize = 1;
	m_screenData.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	m_screenData.CPUAccessFlags = 0;

	m_pRenderTexture = 0;
	device->CreateTexture2D(&m_screenData, 0, &m_pRenderTexture);

	//�����_�[�^�[�Q�b�g�e�N�X�`���������_�[�^�[�Q�b�g�r���[�ɓo�^
	m_pRenderTargetView = 0;
	device->CreateRenderTargetView(m_pRenderTexture, 0, &m_pRenderTargetView);

	m_pScreenBlur = std::make_unique<ScreenBlur>();
	m_pScreenBlur->Create(deviceResources,1);
	m_pScreenBlur->InitializeNormal(1,DirectX::SimpleMath::Vector3::Zero);
}

//�X�V
void ScreenCapture::Update(DX::StepTimer timer)
{
	TextureSetting();
	m_pScreenBlur->Update(timer);
}

//�`��
void ScreenCapture::Draw()
{
	//m_pScreenBlur->SetScreenTexture(m_pTexture);
	//m_pScreenBlur->SetRenderState(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Matrix::Identity, DirectX::SimpleMath::Matrix::Identity);
	//m_pScreenBlur->Render();

	//GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pTexture.Get(), RECT{0,0,100,100});
}

//���Z�b�g
void ScreenCapture::Reset()
{
}

//�摜�̎擾(��ʑS�̂��L���v�`�������摜)
void ScreenCapture::TextureSetting()
{
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>().Get();
	ID3D11Device1* device = deviceResources->GetD3DDevice();

	//�����_�[�^�[�Q�b�g�e�N�X�`����o�^����V�F�[�_���\�[�X�r���[���쐬
	ID3D11ShaderResourceView *rtShaderResView = 0;
	device->CreateShaderResourceView(m_pRenderTexture, 0, &rtShaderResView);
	//�摜�̓o�^
	m_pTexture = rtShaderResView;
	//�V�F�[�_���\�[�X�r���[���폜
	rtShaderResView->Release();
}