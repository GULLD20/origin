#pragma once

#include <SpriteBatch.h>
#include <SimpleMath.h>
#include "../DeviceResources.h"
#include <WICTextureLoader.h>

#include "../StepTimer.h"
#include "../MyEffect/Effect2D/Fade/Fade.h"

class Shutter
{
public:
	//�G�t�F�N�g�p�̈ʒu�f�[�^(����)
	const static float MAX_POS;
public:
	Shutter();
	~Shutter();

	//����
	void Create() ;
	//������
	void Initialize();
	//�X�V
	void Update(const DX::StepTimer &timer);
	// �`��
	void Draw();
	// ���Z�b�g
	void Reset();

	//�t�F�[�h�C��
	void FadeIn();
	//�t�F�[�h�A�E�g
	void FadeOut();
	//�t�F�[�h�C���E�A�E�g�̊m�F
	bool GetFade() const;

private:
	//�t�F�[�h�C���E�A�E�g�̊m�F
	bool                  m_fadeCheck;
	//�t�F�[�h�C���E�A�E�g�J�n�p
	bool                  m_fadeIn;
	bool                  m_fadeOut;

	//�t�F�[�h�G�t�F�N�g
	std::unique_ptr<Fade> m_fade;

	//���Ԍv�Z�p
	float                 m_fadeTimming;
};