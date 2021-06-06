#pragma once

#include <SpriteBatch.h>
#include <SimpleMath.h>
#include "../DeviceResources.h"
#include <WICTextureLoader.h>

#include <vector>

class Score
{
public:
	//�X�R�A�̃f�[�^
	struct ScoreData
	{
		//�X�R�A
		int  score;
		//�ŐV�̃X�R�A������
		bool nreScore;
	};

public:
	///R_ = �����L���O�p  F_��ʕ\���p-------
	//��ʕ\���ʒu
	const static DirectX::SimpleMath::Vector2 F_DRAW_POS;

	//���̈ʒu���v�Z����Ƃ��Ɏg���l(���)
	const static float                        F_CAL_SIDE_1;
	const static float                        F_CAL_SIDE_2;

	//�������v�Z����Ƃ��Ɏg���l(�����L���O)
	const static float                        R_CAL_HIGH;

	//���̈ʒu���v�Z����Ƃ��Ɏg���l(�����L���O)
	const static float                        R_CAL_SIDE_1;
	const static float                        R_CAL_SIDE_2;
	const static float                        R_CAL_SIDE_3;
	const static float                        R_CAL_SIDE_4;
	///--------------------------------------

	//�\�����鏇��
	const static int                          DRAW_RANKING;

	//�摜�̔����̑傫��
	const static float                        TEXTURE_HALF_SIZE_X;

	//�X�R�A�̈ړ����x
	const static float                        SCORE_MOVE_SPEED;

	//�X�R�A�̏����ʒu(X)
	const static float                        SCORE_START_POS_X;

	//�����̉摜�̐�
	const static int                          NUMBER_TEXTURE_NUM;

	//�����L���O�O�̍ŐV�X�R�A�̕\���ʒu
	const static DirectX::SimpleMath::Vector2 NEW_SCORE_POS;

	//�ŐV�X�R�A�̕`��Ԋu
	const static int                          NEW_SCORE_DRAW_INTERVAL;

public:
	Score();
	~Score();

	//����
	void Create(ID3D11Device1* &device);
	//������
	void Initialize();
	//���U���g�p�̍X�V����
	void ResultUpdate();
	// ���Z�b�g
	void Reset();

	//1�ʕ\��
	void DrawFirstRank();
	//�����L���O�\��
	void DrawRanking();

	//�`�揈��
	void Draw(int select, bool ranking,bool newScore, bool OutOfRanking);

	//�X�R�A�̓ǂݍ���
	void LoadScore();
	//�\�[�g(���בւ��ƕ\���f�[�^�ȊO�폜)
	void ScoreSort();
	//�X�R�A�̕ۑ�
	void SaveScore();

	//�����L���O�̒��ɍŐV�̃X�R�A�����邩����
	void CheckNewScore();

	//���݂̃X�R�A�̎擾(�v���C����)
	void SetNowScore(int score);

	//�X�R�A�̈ړ����X�L�b�v����
	void MoveScoreSkip();

	//�ړ���Ԃ��擾
	bool GetMove() const;

private:
	//���Ԍv�Z�p
	void Calculation(const int score);
	//�X�R�A�̈ړ�
	void MoveScore();
	//�ŐV�X�R�A�̕`�攻��
	void NewScoreDrawCheck();

private:
	//�摜�ۑ�
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pNumNormalList;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pNumNewList;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>              m_pColonNormal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>              m_pColonNew;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>              m_pRankingTable;

	//�S�ẴX�R�A
	std::vector<ScoreData>                                        m_scoreDataList;
	//�ŐV�̃X�R�A
	ScoreData                                                     m_newScore;

	//�摜�̕\���̊�{�ʒu(r = �����L���O�p)
	DirectX::SimpleMath::Vector2                                  m_rBasePos;

	//�ŐV�X�R�A�̕`��Ԋu
	int                                                           m_newScoreDrawInterval;
	//�ŐV�X�R�A�̕`�攻��
	bool                                                          m_newScoreDraw;

	//���v����
	int                                                           m_time;
	//���v(�b)
	int                                                           m_second;
	//���v(��)
	int                                                           m_minute;
	//�e�����Ƃ̐��l
	//�b
	int                                                           m_oneDigitSecond;
	int                                                           m_tenDigitSecond;
	//��
	int                                                           m_oneDigitMinute;
	int                                                           m_tenDigitMinute;

	//�X�R�A�̈ړ�
	std::vector<float>                                            m_calXList;
	//�X�R�A�̈ړ�����������
	std::vector<bool>                                             m_calXCheckList;

	//�ړ���Ԃ̔���
	bool                                                          m_move;

	//�����L���O���ɍŐV�X�R�A�����邩
	bool                                                          m_newScoreCheck;
};