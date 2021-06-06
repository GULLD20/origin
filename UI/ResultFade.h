#pragma once

#include <SpriteBatch.h>
#include <SimpleMath.h>
#include "../DeviceResources.h"
#include <WICTextureLoader.h>

#include "../StepTimer.h"
#include "../MyEffect/Effect2D/Fade/Black.h"

class ResultFade
{
public:
	//�t�F�[�h�̓����x
	const static float MAX_ALPHA;
public:
	ResultFade();
	~ResultFade();

	//����
	void Create() ;
	//������
	void Initialize();
	//�X�V
	void Update(DX::StepTimer timer);
	// �`��
	void Draw();
	// ���Z�b�g
	void Reset();

	//�t�F�[�h�C���̊J�n
	void FadeIn();
	//�t�F�[�h�C���̊m�F�@Get
	bool GetFade() const;

private:
	//�t�F�[�h�m�F
	bool                   m_fadeCheck;
	//�t�F�[�h�C���J�n�p
	bool                   m_fadeIn;

	//�t�F�[�h�G�t�F�N�g
	std::unique_ptr<Black> m_pBlack;

	//�����x�v�Z�p
	float                  m_alpha;
};