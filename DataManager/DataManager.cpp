#include "../pch.h"
#include "DataManager.h"
#include "../Character/Player/Weapon/WeaponType.h"

DataManager::DataManager()
	:m_time(0.0f)
{

}


DataManager::~DataManager()
{

}

//����������
void DataManager::Initialize()
{
	m_time = 0.0f;
	m_clear = false;
}
//---------------------------------------------------------



//�Q�[�����f�[�^---------------------------------
//����
float DataManager::GetTime() const
{
	//���U���g�V�[����
	return m_time;
}

void DataManager::SetTime(float time)
{
	m_time = time;
}

//�X�R�A
std::vector<Score::ScoreData> DataManager::GetScore()
{
	//�^�C�g���V�[���ŎQ��
	return m_scoreList;
}

void DataManager::SetScore(const std::vector<Score::ScoreData>& scoreList)
{
	m_scoreList = scoreList;
}

//�N���A����
bool DataManager::GetClear() const
{
	return m_clear;
}

void DataManager::SetClear(bool clear)
{
	m_clear = clear;
}

//-----------------------------------------------


//���Z�b�g---------------------------------------
void DataManager::Reset()
{

}
//-----------------------------------------------