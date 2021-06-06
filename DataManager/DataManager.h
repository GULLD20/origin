#pragma once

#include <SimpleMath.h>
#include <vector>

#include "../UI/Score.h"

class DataManager
{
public:
	DataManager();
	~DataManager();

	void Initialize();
	//---------------------------------------------------------

	//�Q�[�����f�[�^---------------------------------
	//����
	float GetTime() const;
	void SetTime(float time);
	//�X�R�A
	std::vector<Score::ScoreData> GetScore();
	void SetScore(const std::vector<Score::ScoreData> &scoreList);
	//�N���A����
	bool GetClear() const;
	void SetClear(bool clear);
	//-----------------------------------------------

	//���Z�b�g---------------------------------------
	void Reset();
	//-----------------------------------------------

private:
	///�v���C���[�̃f�[�^-----------------------------
	//�J�����p��]�s��
	DirectX::SimpleMath::Matrix   m_playerCameraRotation;
	///-----------------------------------------------

	///�Q�[�����f�[�^---------------------------------
	//����
	float                         m_time;
	//�X�R�A
	std::vector<Score::ScoreData> m_scoreList;
	//�N���A����
	bool                          m_clear;
	///-----------------------------------------------
};

