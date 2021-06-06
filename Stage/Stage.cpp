#include "../pch.h"
#include "Stage.h"

#include "../MyLib/MyLib.h"

#include "../Character/Enemy/Goblin/Goblin.h"
#include "../Character/Enemy/LongRange/LongRange.h"
#include "../Character/Enemy/LongRangeRowling/LongRangeRowling.h"
#include "../Character/Enemy/Stamp/Stamp.h"
#include "../Character/Enemy/Rowling/Rowling.h"

#include "../DataManager/DataManager.h"

Stage::Stage()
	: m_pPlayer(nullptr)
	, m_pFollowCamera(nullptr)
{
}


Stage::~Stage()
{
	if (m_pPlayer != nullptr)
	{
		m_pPlayer.reset(nullptr);
	}

	if (m_pEnemy.size() != 0)
	{
		for (int i = 0; i < static_cast<int>(m_pEnemy.size()); i++)
		{
			m_pEnemy[i].reset(nullptr);
		}
		m_pEnemy.clear();
	}

	if (m_pFollowCamera != nullptr)
	{
		m_pFollowCamera.reset(nullptr);
	}
}

//�G�l�~�[�̍쐬
void Stage::CreatEnemy(Params::EnemyTyp typ, DirectX::SimpleMath::Vector3 pos)
{
	//�G�l�~�[���쐬����������
	bool create = false;

	//�G�l�~�[�ƃ��f���̍쐬
	switch (typ)
	{
		//�S�u����
	case Params::EnemyTyp::Goblin:
		m_pEnemy.push_back(std::make_unique<Goblin>(pos, Params::GOBLIN_ATTACK, Params::GOBLIN_HP, Params::GOBLIN_SPEED, Params::GOBLIN_RANGE));
		m_pEnemy[m_creatEnemyNum]->CreateModel(L"Resources/CMO/Goblin.cmo");
		create = true;
		break;

		//�������U���̓G
	case Params::EnemyTyp::LongRange:
		m_pEnemy.push_back(std::make_unique<LongRange>(pos, Params::LONGRANGE_ATTACK, Params::LONGRANGE_HP, Params::LONGRANGE_SPEED, Params::LONGRANGE_RANGE));
		m_pEnemy[m_creatEnemyNum]->CreateModel(L"Resources/CMO/LongRange.cmo");
		create = true;
		break;

		//��]���鉓�����U���̓G
	case Params::EnemyTyp::LongRangeRowling:
		m_pEnemy.push_back(std::make_unique<LongRangeRowling>(pos, Params::LONGRANGEROWLING_ATTACK, Params::LONGRANGEROWLING_HP, Params::LONGRANGEROWLING_SPEED, Params::LONGRANGEROWLING_RANGE));
		m_pEnemy[m_creatEnemyNum]->CreateModel(L"Resources/CMO/LongRangeRowling.cmo");
		create = true;
		break;

		//�����Ԃ��G
	case Params::EnemyTyp::Stamp:
		m_pEnemy.push_back(std::make_unique<Stamp>(pos, Params::STAMP_ATTACK, Params::STAMP_HP, Params::STAMP_SPEED, Params::STAMP_RANGE));
		m_pEnemy[m_creatEnemyNum]->CreateModel(L"Resources/CMO/Stamp.cmo");
		create = true;
		break;

		//��]�U���̓G
	case Params::EnemyTyp::Rowling:
		m_pEnemy.push_back(std::make_unique<Rowling>(pos, Params::ROWLING_ATTACK, Params::ROWLING_HP, Params::ROWLING_SPEED, Params::ROWLING_RANGE));
		m_pEnemy[m_creatEnemyNum]->CreateModel(L"Resources/CMO/Rowling.cmo");
		create = true;
		break;

	default:
		break;
	}

	if (create)
	{
		//�쐬�������v���𑝂₷
		m_creatEnemyNum++;
	}
}

//���̃G�l�~�[�̗̑͂�0�ɂȂ�����
void Stage::EnemyDethCount()
{
	//HP���[���ɂȂ����G�̐��̏�����
	m_DethEnemyNum = 0;

	//�G��HP���Ȃ��Ȃ�����
	for (int i = 0; i < static_cast<int>(m_pEnemy.size()); i++)
	{
		//�G������ł���Ƃ�
		if (m_pEnemy[i]->IsDeth())
		{
			//���S�����G�̐����v�Z
			m_DethEnemyNum++;
		}
	}
}

//�Q�[���N���A
bool Stage::GameClear()
{
	//�N���A����
	if (m_DethEnemyNum >= static_cast<int>(m_pEnemy.size()))
	{
		return true;
	}

	EnemyDethCount();
	return false;
}

//�Q�[���I�[�o�[
bool Stage::GameOver()
{
	if (m_pPlayer->GetHP() <= 0.0f)
	{
		return true;
	}

	return false;
}

//CSV��ǂݍ���
void Stage::LoadCsv(char* failName)
{
	//CSV�t�@�C����1�s���ۑ�
	std::vector<std::string> allDataStr = MyLib::LoadFail_TextForm(failName);

	//1�s���ۑ�����������𕪊����Ă�����i�[����ϐ�
	std::vector<std::vector<std::string>> separate;
	separate.resize(allDataStr.size());

	//������𕪊����Ă�����i�[����
	for (int i = 0; i < static_cast<int>(allDataStr.size()); i++)
	{
		separate[i] = MyLib::SeparateString(allDataStr[i], ',');
	}

	m_csvData = separate;
}

//�ǂݍ��񂾃f�[�^��ϐ��Ɋi�[����
void Stage::SettingCsvData()
{
	//csv�̃f�[�^���}�b�v���L�����N�^�[�̂ǂ����\���Ă��邩
	std::string typ = "";
	//�v�f�̈ʒu(�c)
	int select = 0;

	//�ǂݍ��񂾃f�[�^��int�^�ɕϊ����Ċi�[
	for (int i = 0; i < static_cast<int>(m_csvData.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(m_csvData[i].size()); j++)
		{
			//1�̗v�f���������Ă��Ȃ��Ƃ���
			//�}�b�v���L�����N�^�[��\���Ă��邩
			//�}�b�v�̃T�C�Y��\���Ă���
			if (m_csvData[i].size() == 1)
			{
				//���݂ǂ̃f�[�^�������Ă��邩
				typ = m_csvData[i][j];
				//����p�̃f�[�^���i�[���ăG���[���o�Ȃ�����
				i++;
				//�v�f�̈ʒu��������
				select = 0;
			}

			if (typ == "Stage")
			{
				m_mapDataList[select][j] = std::stoi(m_csvData[i][j]);
			}
			else if (typ == "Character")
			{
				m_characterData[select][j] = std::stoi(m_csvData[i][j]);
			}
			else if (typ == "�c")
			{
				//�������̊m��(m_MapData��m_CharacterData�̗v�f�̊m��)
				m_mapDataList.resize(std::stoi(m_csvData[i][j]));
				m_characterData.resize(std::stoi(m_csvData[i][j]));
			}
			else if (typ == "��")
			{
				//�v�f��
				int size = std::stoi(m_csvData[i][j]);
				for (int k = 0; k < size; k++)
				{
					//�������̊m��(m_MapData��m_CharacterData�̗v�f�̊m��)
					m_mapDataList[k].resize(size);
					m_characterData[k].resize(size);
				}
			}
		}
		select++;
	}
}