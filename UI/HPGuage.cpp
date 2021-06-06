#include "../pch.h"
#include "HPGuage.h"

#include "../DataManager/DataManager.h"

HPGuage::HPGuage()
{
}


HPGuage::~HPGuage()
{
	
}

//����
void HPGuage::Create(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	//�摜�̓ǂݍ���
	m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/HPGuage/Player/HPBack.png", NULL, m_Guage[0].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/HPGuage/Player/HP.png", NULL, m_Guage[1].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/HPGuage/Player/HPFrame.png", NULL, m_Guage[2].ReleaseAndGetAddressOf());
}

//������
void HPGuage::Initialize()
{
	//�ʒu�̏�����
	m_Pos = DirectX::SimpleMath::Vector2(10.0f,10.0f);
	//�v���C���[��HP������
	m_HP = 0.0f;

	m_TextureSize =
	{
		0,
		0,
		TEXTUERSIZEX,
		TEXTUERSIZEY,
	};
}

// �X�V
void HPGuage::Update()
{
	DataManager*DataManager = DataManager::CreateInstance();
	//�v���C���[��HP�̎擾
	m_HP = DataManager->GetPlayerHP();

	//�v���C���[��HP�̂P���̒l�����߂�
	float onePercent = DataManager->GetPlayerMaxHP() * 0.01f;

	//���݂̃v���C���[��HP�������c���Ă��邩���߂�
	float RemainingGuage = m_HP / onePercent;

	//�摜�̉��̑傫���̂P�������߂�
	float textuerSizeOnePercent = TEXTUERSIZEX * 0.01f;

	//���߂�textuerSizeOnePercent��RemainingGuage���������摜���ǂꂭ�炢�\�����邩���߂�(X)
	int sizeX = static_cast<int>(textuerSizeOnePercent * RemainingGuage);

	m_TextureSize =
	{
		0,
		0,
		sizeX,
		TEXTUERSIZEY,
	};
}

//�`��
void HPGuage::Draw()
{
	//�摜�̕\��
	m_SpriteBatch->Begin();

	m_SpriteBatch->Draw(m_Guage[0].Get(), m_Pos);
	m_SpriteBatch->Draw(m_Guage[1].Get(), m_Pos,&m_TextureSize);
	m_SpriteBatch->Draw(m_Guage[2].Get(), m_Pos);

	m_SpriteBatch->End();
}

//���Z�b�g
void HPGuage::Reset()
{
	//���Z�b�g
	//m_Texture.Reset();
	for (int i = 0; i < 3; i++)
	{
		m_Guage[i].Reset();
	}
	m_SpriteBatch.reset();
}