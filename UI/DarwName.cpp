#include "../pch.h"
#include "DarwName.h"

#include <SpriteBatch.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

#include "../Scene/GameContext.h"

//�\���ʒu�̊
const DirectX::SimpleMath::Vector2 DrawName::BASE_POS = DirectX::SimpleMath::Vector2(45.0f, 11.0f);

DrawName::DrawName()
{

}

DrawName::~DrawName()
{

}

//����
void DrawName::Create(ID3D11Device1* &device)
{
	//�摜�̓ǂݍ���
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/letter/HiraganaKatakana.png", NULL, m_pHiraganaKatakana.ReleaseAndGetAddressOf());
}

//������
void DrawName::Initialize()
{
	CSVLoad();
}

//�`��
void DrawName::Draw(const std::string& name, const DirectX::SimpleMath::Vector2 &pos, const DirectX::CommonStates &state)
{
	//�摜�̕\��
	for (int i = 0; i < static_cast<int>(name.size()); i++)
	{
		//name.substr(i * 2, 2)��i * 2�͂ǂ��̃f�[�^����ɂ��邩(�S�p�͓�̃f�[�^�łł��Ă��邽��2�����炷)�A2�͓�̃f�[�^���Ƃ�������(�S�p�͓�̃f�[�^�łł��Ă��邽��)
		//18�͈�̕����̑傫��
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pHiraganaKatakana.Get(), DirectX::SimpleMath::Vector2((pos.x + (i *18)+ BASE_POS.x), (pos.y+ BASE_POS.y)), &m_textureSizeMapList[name.substr(i * 2, 2)]);
		//�\���\��̕��������ׂĕ\��������
		if (i * 2 >= static_cast<int>(name.size())-2)
		{
			break;
		}
	}
}

// ���Z�b�g
void DrawName::Reset()
{
	m_pHiraganaKatakana.Reset();
}

//CSV�t�@�C���̓ǂݍ���
void DrawName::CSVLoad()
{
	// csv�t�@�C���̃f�[�^��S�ē����
	std::ifstream wifs;
	//�}�b�v�f�[�^������
	std::string key;
	std::string data;

	//�f�[�^�ۑ��p
	int stertPosX = 0;
	int stertPosY = 0;
	int endPosX = 0;
	int endPosY = 0;

	//CSV�̓ǂݍ���
	std::string failName = "Resources/CSV/letter.csv";
	wifs.open(failName);

	//�J���Ȃ�������X�L�b�v
	if (!wifs.is_open())
		return;

	//�u,�v�ŕ�������؂�
	while (getline(wifs, key, ','))
	{
		//�u,�v�ŕ�������؂�
		getline(wifs, data, ',');
		//��؂������l������
		stertPosX = std::stoi(data);

		//�u,�v�ŕ�������؂�
		getline(wifs, data, ',');
		//��؂������l������
		stertPosY = std::stoi(data);

		//�u,�v�ŕ�������؂�
		getline(wifs, data, ',');
		//��؂������l������
		endPosX = std::stoi(data);

		//�u\n�v(���s)�ŕ�������؂�
		getline(wifs, data, '\n');
		//��؂������l������
		endPosY = std::stoi(data);

		//1�̕���(�S�p)�݂̂Ȃ̂�0�̈ʒu����2�ڂ܂ł̃f�[�^���L�[�ɂ���
		m_textureSizeMapList[key.substr(0, 2)] =
		{
		//�摜�̐؂���ʒu��ۑ�
		stertPosX,
		stertPosY,
		endPosX,
		endPosY,
		};
	}
}