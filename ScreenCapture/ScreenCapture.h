#pragma once

#include "../DeviceResources.h"
#include <d3d11.h>

#include "../StepTimer.h"

#include "../MyEffect/Effect2D/ScreenBlur/ScreenBlur.h"

class ScreenCapture
{
public:
	ScreenCapture();
	~ScreenCapture();

	//������
	void Initialize();
	//�X�V
	void Update(DX::StepTimer timer);
	//�`��
	void Draw();
	//���Z�b�g
	void Reset();

	//�摜�̎擾(��ʑS�̂��L���v�`�������摜)
	void TextureSetting();

private:
	//�摜�f�[�^
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  m_pTexture;

	//D3D10_TEXTURE2D_DESC�Ń����_�[�^�[�Q�b�g�r���[�Ɛ錾����
	D3D11_TEXTURE2D_DESC                              m_screenData;
	//�����_�[�^�[�Q�b�g�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11Texture2D>           m_pRenderTexture;
	//�����_�[�^�[�Q�b�g�r���[
	ID3D11RenderTargetView *                          m_pRenderTargetView;

	std::unique_ptr<ScreenBlur>                       m_pScreenBlur;
};

