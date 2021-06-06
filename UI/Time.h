#pragma once

#include <SimpleMath.h>
#include "../DeviceResources.h"
#include <WICTextureLoader.h>

class Time
{
public:
	//�^�C�}�[�̕`��ʒu
	const static DirectX::SimpleMath::Vector2 DRAW_POS_1;
	const static DirectX::SimpleMath::Vector2 DRAW_POS_2;

public:
	Time();
	~Time();

	//����
	void Create(ID3D11Device1* device) ;
	//������
	void Initialize();
	// �X�V
	void Update();
	// �`��
	void Draw();
	// ���Z�b�g
	void Reset();

	//�v���C�V�[���̍��v���Ԃ��擾
	float GetPlayTime() const { return m_total; };

	//1�t���[���ɂ����������Ԃ�ݒ�
	void SetElapsedTime(const float &elapsedTime) { m_elapsedTime = elapsedTime; };

	//1�t���[���ɂ����������Ԃ��擾
	float GetElapsedTime() const { return m_elapsedTime; };

private:
	//�摜�ۑ�
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pNum[10];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pColon;

	DirectX::SimpleMath::Vector2                     m_basePos;

	float                                            m_total;
	int                                              m_time;
	int                                              m_second;
	int                                              m_minute;
	int                                              m_oneDigitSecond;
	int                                              m_tenDigitSecond;
	int                                              m_oneDigitMinute;
	int                                              m_tenDigitMinute;

	float                                            m_elapsedTime;
};

