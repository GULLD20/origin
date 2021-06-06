#include "../pch.h"
#include "Time.h"

#include <SpriteBatch.h>

#include "../Aspect.h"

#include "../Scene/GameContext.h"

//�^�C�}�[�̕`��ʒu
const DirectX::SimpleMath::Vector2 Time::DRAW_POS_1 = DirectX::SimpleMath::Vector2(70.0f,0.0f);
const DirectX::SimpleMath::Vector2 Time::DRAW_POS_2 = DirectX::SimpleMath::Vector2(30.0f,0.0f);

Time::Time()
{
}


Time::~Time()
{

}

//����
void Time::Create(ID3D11Device1* device)
{
	//�摜�ǂݍ���
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/0.png", NULL, m_pNum[0].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/1.png", NULL, m_pNum[1].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/2.png", NULL, m_pNum[2].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/3.png", NULL, m_pNum[3].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/4.png", NULL, m_pNum[4].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/5.png", NULL, m_pNum[5].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/6.png", NULL, m_pNum[6].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/7.png", NULL, m_pNum[7].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/8.png", NULL, m_pNum[8].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/9.png", NULL, m_pNum[9].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/�R����.png", NULL, m_pColon.ReleaseAndGetAddressOf());
}

//������
void Time::Initialize()
{
	m_basePos = DirectX::SimpleMath::Vector2(Aspect::ASPECT_X*0.5f, 10.0f);

	m_time = 0;
	m_second = 0;
	m_minute = 0;
	m_oneDigitSecond = 0;
	m_tenDigitSecond = 0;
	m_oneDigitMinute = 0;
	m_tenDigitMinute = 0;
	m_total = 0.0f;
	m_elapsedTime = 0.0f;
}

//�X�V
void Time::Update()
{
	//���݂̎���
	m_time = static_cast<int>(m_total);

	//�������߂�
	m_minute = m_time / 60;

	//�������߂�(�\��)
	m_tenDigitMinute = m_minute / 10;

	//�������߂�(�ꌅ)
	m_oneDigitMinute = m_minute - (m_tenDigitMinute * 10);;

	//�b�����߂�
	m_second = m_time - (60 * m_minute);

	//�b�����߂�(�\��)
	m_tenDigitSecond = m_second / 10;

	//�b�����߂�(�ꌅ)
	m_oneDigitSecond = m_second - (m_tenDigitSecond * 10);


	//���Ԃ̌o��
	m_total += m_elapsedTime;
}

//�`��
void Time::Draw()
{
	//�o�ߎ��Ԃ̕`��
	GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNum[m_tenDigitMinute].Get(), m_basePos - DRAW_POS_1);
	GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNum[m_oneDigitMinute].Get(), m_basePos - DRAW_POS_2);
	GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pColon.Get(), m_basePos);
	GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNum[m_tenDigitSecond].Get(), m_basePos+ DRAW_POS_2);
	GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNum[m_oneDigitSecond].Get(), m_basePos + DRAW_POS_1);
}

//���Z�b�g
void Time::Reset()
{
	for (int i = 0; i < 10; i++)
	{
		m_pNum[i].Reset();
	}
	m_pColon.Reset();
}