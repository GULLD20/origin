#include "../pch.h"
#include "Score.h"

#include "../DataManager/DataManager.h"

#include <CommonStates.h>
#include "../Scene/GameContext.h"
#include "../Aspect.h"

#include "../MyLib/MyLib.h"

//��ʕ\���ʒu
const DirectX::SimpleMath::Vector2 Score::F_DRAW_POS = DirectX::SimpleMath::Vector2(1160.0f, 10.0f);

//���̈ʒu���v�Z����Ƃ��Ɏg���l
const float Score::F_CAL_SIDE_1 = 70.0f;
const float Score::F_CAL_SIDE_2 = 30.0f;

//�������v�Z����Ƃ��Ɏg���l
const float Score::R_CAL_HIGH = 90.0f;

//���̈ʒu���v�Z����Ƃ��Ɏg���l
const float Score::R_CAL_SIDE_1 = 35.0f;
const float Score::R_CAL_SIDE_2 = 70.0f;
const float Score::R_CAL_SIDE_3 = 105.0f;
const float Score::R_CAL_SIDE_4 = 140.0f;

//�\�����鏇��
const int Score::DRAW_RANKING = 5;

//�摜�̔����̑傫��
const float Score::TEXTURE_HALF_SIZE_X = (504.0f * 0.5f);

//�X�R�A�̈ړ����x
const float Score::SCORE_MOVE_SPEED = 8.0f;

//�X�R�A�̏����ʒu(X)
const float Score::SCORE_START_POS_X = -1200.0f;

//�����̉摜�̐�
const int Score::NUMBER_TEXTURE_NUM = 10;

//�����L���O�O�̍ŐV�X�R�A�̕\���ʒu
const DirectX::SimpleMath::Vector2 Score::NEW_SCORE_POS = DirectX::SimpleMath::Vector2(750, 50.0f);

//�ŐV�X�R�A�̕`��Ԋu
const int Score::NEW_SCORE_DRAW_INTERVAL = 5;

Score::Score()
{

}

Score::~Score()
{
	if (m_pNumNormalList.size() > 0)
	{
		for (int i = 0; i< static_cast<int>(m_pNumNormalList.size()); i++)
		{
			m_pNumNormalList[i].Reset();
		}
		m_pNumNormalList.clear();
	}

	if (m_pNumNewList.size() > 0)
	{
		for (int i = 0;i< static_cast<int>(m_pNumNewList.size());i++)
		{
			m_pNumNewList[i].Reset();
		}
		m_pNumNewList.clear();
	}

	if (m_pColonNormal!= nullptr)
	{
		m_pColonNormal.Reset();
	}

	if (m_pColonNew != nullptr)
	{
		m_pColonNew.Reset();
	}

	if (m_pRankingTable != nullptr)
	{
		m_pRankingTable.Reset();
	}
}

//����
void Score::Create(ID3D11Device1* &device)
{
	//�������̊m��
	m_pNumNormalList.resize(NUMBER_TEXTURE_NUM);
	m_pNumNewList.resize(NUMBER_TEXTURE_NUM);

	//�摜�̓ǂݍ���
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/0.png", NULL, m_pNumNormalList[0].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/1.png", NULL, m_pNumNormalList[1].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/2.png", NULL, m_pNumNormalList[2].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/3.png", NULL, m_pNumNormalList[3].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/4.png", NULL, m_pNumNormalList[4].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/5.png", NULL, m_pNumNormalList[5].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/6.png", NULL, m_pNumNormalList[6].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/7.png", NULL, m_pNumNormalList[7].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/8.png", NULL, m_pNumNormalList[8].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/9.png", NULL, m_pNumNormalList[9].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/Normal/�R����.png", NULL, m_pColonNormal.ReleaseAndGetAddressOf());

	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/0_New.png", NULL, m_pNumNewList[0].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/1_New.png", NULL, m_pNumNewList[1].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/2_New.png", NULL, m_pNumNewList[2].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/3_New.png", NULL, m_pNumNewList[3].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/4_New.png", NULL, m_pNumNewList[4].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/5_New.png", NULL, m_pNumNewList[5].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/6_New.png", NULL, m_pNumNewList[6].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/7_New.png", NULL, m_pNumNewList[7].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/8_New.png", NULL, m_pNumNewList[8].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/9_New.png", NULL, m_pNumNewList[9].ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Number/New/�R����_New.png", NULL, m_pColonNew.ReleaseAndGetAddressOf());

	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/rankingTemporary.png", NULL, m_pRankingTable.ReleaseAndGetAddressOf());

	m_scoreDataList.resize(DRAW_RANKING);
}

//������
void Score::Initialize()
{
	//�v�f�̊m��
	m_calXList.resize(DRAW_RANKING);
	m_calXCheckList.resize(DRAW_RANKING);

	//�X�R�A�̕`��ʒu�̏�����
	for (int i = 0; i < DRAW_RANKING; i++)
	{
		m_calXList[i] = SCORE_START_POS_X;
		m_calXCheckList[i] = false;
	}

	m_time = 0;
	m_second = 0;
	m_minute = 0;
	m_oneDigitSecond = 0;
	m_tenDigitSecond = 0;
	m_oneDigitMinute = 0;
	m_tenDigitMinute = 0;

	//�ړ����
	m_move = true;

	//�ŐV�X�R�A�̏�����
	m_newScore.score = 0;
	m_newScore.nreScore = false;

	//�����L���O���ɍŐV�X�R�A�����邩
	m_newScoreCheck = false;

	//�ŐV�X�R�A�̕`��Ԋu�̏�����
	m_newScoreDrawInterval = 0;
	//�ŐV�X�R�A�̕`�攻��
	m_newScoreDraw = false;
}

//���U���g�p�̍X�V����
void Score::ResultUpdate()
{
	//�ʒu�̏�����
	//�����L���O�̊O�g�̈ʒu�����߂�(���͉�ʂ̒��S����摜�̔����̑傫�������炷����,�c��50.0f)
	m_rBasePos = DirectX::SimpleMath::Vector2((Aspect::ASPECT_X*0.5f) - TEXTURE_HALF_SIZE_X, 50.0f);

	MoveScore();
}

//���Z�b�g
void Score::Reset()
{
	//m_Texture.Reset();
	for (int i = 0; i < static_cast<int>(m_pNumNormalList.size()); i++)
	{
		m_pNumNormalList[i].Reset();
	}

	for (int i = 0; i <static_cast<int>(m_pNumNewList.size()); i++)
	{
		m_pNumNewList[i].Reset();
	}

	m_pColonNormal.Reset();
	m_pColonNew.Reset();
}

//1�ʕ\��
void Score::DrawFirstRank()
{
	//�ǂ̉摜��\�����邩�����߂�(0�Ԗڂ���ʂ̃X�R�A������)
	Calculation(m_scoreDataList[0].score);
	//�摜�̕\��
	Draw(0, false, false, false);
}

//�����L���O�\��
void Score::DrawRanking()
{
	//�����L���O�̊O�g��\��
	GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pRankingTable.Get(), m_rBasePos);

	//DRAW_RANKING�̐����\��
	for (int i = 0; i < DRAW_RANKING; i++)
	{
		//�摜��\������ʒu�̍����𒲐�
		m_rBasePos += DirectX::SimpleMath::Vector2(0.0f, R_CAL_HIGH);

		//�\�����鐔�l�����߂�
		Calculation(m_scoreDataList[i].score);

		if (m_scoreDataList[i].nreScore)
		{
			NewScoreDrawCheck();

			//�ŐV�X�R�A��_�ł�����
			if (m_newScoreDraw)
			{
				Draw(i, true, true, false);
			}
		}
		else
		{
			Draw(i, true, false,false);
		}
	}

	//�ŐV�̃X�R�A�̕\��(�����L���O�O)
	if (!m_newScoreCheck)
	{
		//�\�����鐔�l�����߂�
		Calculation(m_newScore.score);

		NewScoreDrawCheck();

		//�ŐV�X�R�A��_�ł�����
		if (m_newScoreDraw)
		{
			//����0�͊֌W�Ȃ�
			Draw(0, false, true, true);
		}
	}
}

//�`�揈��
void Score::Draw(int select, bool ranking, bool newScore, bool OutOfRanking)
{
	DirectX::SimpleMath::Vector2 pos1 = F_DRAW_POS - DirectX::SimpleMath::Vector2(F_CAL_SIDE_1, 0.0f);
	DirectX::SimpleMath::Vector2 pos2 = F_DRAW_POS - DirectX::SimpleMath::Vector2(F_CAL_SIDE_2, 0.0f);
	DirectX::SimpleMath::Vector2 pos3 = F_DRAW_POS;
	DirectX::SimpleMath::Vector2 pos4 = F_DRAW_POS + DirectX::SimpleMath::Vector2(F_CAL_SIDE_2, 0.0f);
	DirectX::SimpleMath::Vector2 pos5 = F_DRAW_POS + DirectX::SimpleMath::Vector2(F_CAL_SIDE_1, 0.0f);

	if (ranking)
	{
		//�X�R�A�̕\��(m_rBasePos + Vector2�̒l�͉E�ɉ摜�������炷����)
		pos1 = m_rBasePos + DirectX::SimpleMath::Vector2(TEXTURE_HALF_SIZE_X + m_calXList[select], 0.0f);
		pos2 = m_rBasePos + DirectX::SimpleMath::Vector2(R_CAL_SIDE_1 + TEXTURE_HALF_SIZE_X + m_calXList[select], 0.0f);
		pos3 = m_rBasePos + DirectX::SimpleMath::Vector2(R_CAL_SIDE_2 + TEXTURE_HALF_SIZE_X + m_calXList[select], 0.0f);
		pos4 = m_rBasePos + DirectX::SimpleMath::Vector2(R_CAL_SIDE_3 + TEXTURE_HALF_SIZE_X + m_calXList[select], 0.0f);
		pos5 = m_rBasePos + DirectX::SimpleMath::Vector2(R_CAL_SIDE_4 + TEXTURE_HALF_SIZE_X + m_calXList[select], 0.0f);
	}

	if (OutOfRanking)
	{
		//�X�R�A�̕\��(m_rBasePos + Vector2�̒l�͉E�ɉ摜�������炷����)
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNewList[m_tenDigitMinute].Get(), NEW_SCORE_POS + DirectX::SimpleMath::Vector2(TEXTURE_HALF_SIZE_X, R_CAL_HIGH));
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNewList[m_oneDigitMinute].Get(), NEW_SCORE_POS + DirectX::SimpleMath::Vector2(R_CAL_SIDE_1 + TEXTURE_HALF_SIZE_X, R_CAL_HIGH));
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pColonNew.Get(), NEW_SCORE_POS + DirectX::SimpleMath::Vector2(R_CAL_SIDE_2 + TEXTURE_HALF_SIZE_X, R_CAL_HIGH));
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNewList[m_tenDigitSecond].Get(), NEW_SCORE_POS + DirectX::SimpleMath::Vector2(R_CAL_SIDE_3 + TEXTURE_HALF_SIZE_X, R_CAL_HIGH));
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNewList[m_oneDigitSecond].Get(), NEW_SCORE_POS + DirectX::SimpleMath::Vector2(R_CAL_SIDE_4 + TEXTURE_HALF_SIZE_X, R_CAL_HIGH));

		//�X�R�A�̕\��(m_rBasePos + Vector2�̒l�͉E�ɉ摜�������炷����)
		pos1 = NEW_SCORE_POS + DirectX::SimpleMath::Vector2(TEXTURE_HALF_SIZE_X, R_CAL_HIGH);
		pos2 = NEW_SCORE_POS + DirectX::SimpleMath::Vector2(R_CAL_SIDE_1 + TEXTURE_HALF_SIZE_X, R_CAL_HIGH);
		pos3 = NEW_SCORE_POS + DirectX::SimpleMath::Vector2(R_CAL_SIDE_2 + TEXTURE_HALF_SIZE_X, R_CAL_HIGH);
		pos4 = NEW_SCORE_POS + DirectX::SimpleMath::Vector2(R_CAL_SIDE_3 + TEXTURE_HALF_SIZE_X, R_CAL_HIGH);
		pos5 = NEW_SCORE_POS + DirectX::SimpleMath::Vector2(R_CAL_SIDE_4 + TEXTURE_HALF_SIZE_X, R_CAL_HIGH);
	}

	if (!newScore)
	{
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNormalList[m_tenDigitMinute].Get(), pos1);
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNormalList[m_oneDigitMinute].Get(), pos2);
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pColonNormal.Get(), pos3);
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNormalList[m_tenDigitSecond].Get(), pos4);
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNormalList[m_oneDigitSecond].Get(), pos5);
	}
	else
	{
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNewList[m_tenDigitMinute].Get(), pos1);
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNewList[m_oneDigitMinute].Get(), pos2);
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pColonNew.Get(), pos3);
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNewList[m_tenDigitSecond].Get(), pos4);
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pNumNewList[m_oneDigitSecond].Get(), pos5);
	}
}

void Score::LoadScore()
{
	//�e�L�X�g�̓ǂݍ���
	std::vector<std::string> score = MyLib::LoadFail_TextForm("Resources/Data/Score.csv");

	//�e�L�X�g�ɏ�����Ă�����̂�int�^�ɕϊ����ĕۑ�����
	for (int i = 0; i < DRAW_RANKING; i++)
	{
		m_scoreDataList[i].score = std::stoi(score[i]);
		m_scoreDataList[i].nreScore = false;
	}
}

//�\�[�g(���בւ��ƕ\���f�[�^�ȊO�폜)
void Score::ScoreSort()
{
	//�\�[�g����
	ScoreData t;

	for (int i = 0; i < static_cast<int>(m_scoreDataList.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(m_scoreDataList.size()); j++)
		{
			if (m_scoreDataList[i].score < m_scoreDataList[j].score)
			{
				t = m_scoreDataList[i];
				m_scoreDataList[i] = m_scoreDataList[j];
				m_scoreDataList[j] = t;
			}
		}
	}

	//�]���ȃf�[�^�̍폜
	int restNum = static_cast<int>(m_scoreDataList.size()) - DRAW_RANKING;
	for (int i = 0; i < restNum; i++)
	{
		m_scoreDataList.pop_back();
	}
}

//�X�R�A�̕ۑ�
void Score::SaveScore()
{
	//�e�L�X�g���J��(�X�R�A�̕ۑ���)
	std::ofstream ofs("Resources/Data/Score.csv");

	//�e�L�X�g�ɃX�R�A��ۑ�
	for (int i = 0; i < static_cast<int>(m_scoreDataList.size()); i++)
	{
		ofs << m_scoreDataList[i].score << std::endl;
	}
}

//�����L���O�̒��ɍŐV�̃X�R�A�����邩����
void Score::CheckNewScore()
{
	for (int i = 0; i < static_cast<int>(m_scoreDataList.size()); i++)
	{
		if (m_scoreDataList[i].nreScore)
		{
			m_newScoreCheck = true;
			break;
		}
	}
}

//���݂̃X�R�A�̎擾(�v���C����)
void Score::SetNowScore(int score)
{
	//�S�Ẵf�[�^��ۑ�����
	std::vector<ScoreData> all;

	//�V�����f�[�^�����邽�߂��Ƃ��Ƃ̗v�f��+1�Ń��������m�ۂ���
	all.resize(m_scoreDataList.size() + 1);

	//�f�[�^�̊i�[
	for (int i = 0; i < static_cast<int>(all.size()); i++)
	{
		//�v�f����0���珇�ԂȂ���-1
		if (i == static_cast<int>(all.size()-1))
		{
			//�ŐV�f�[�^������
			all[i].score = score;
			all[i].nreScore = true;
			break;
		}

		//����܂ł̃f�[�^������
		all[i] = m_scoreDataList[i];
	}

	m_scoreDataList = all;

	//�v�f����0���珇�ԂȂ���-1
	//�ŐV�f�[�^������
	m_newScore = all[static_cast<int>(all.size() - 1)];
}

//�X�R�A�̈ړ����X�L�b�v����
void Score::MoveScoreSkip()
{
	//�X�R�A�̕`��ʒu�̏�����
	for (int i = 0; i < DRAW_RANKING; i++)
	{
		m_calXList[i] = 0.0f;
		m_calXCheckList[i] = true;
	}

	m_move = false;
}


//�ړ���Ԃ��擾
bool Score::GetMove() const
{
	return m_move;
}

//���Ԍv�Z�p
void Score::Calculation(const int score)
{
	//�������߂邽�߂�60�Ŋ���
	m_minute = score / 60;

	//�������߂�(�\�������߂邽�߂�10�Ŋ���)
	m_tenDigitMinute = m_minute / 10;

	//�������߂�(�ꌅ�����߂邽�߂ɏ\��(�\���ɂ��邽�߂�10��������)�̐��l������)
	m_oneDigitMinute = m_minute - (m_tenDigitMinute * 10);;

	//�b�����߂邽�߂ɕ�������(���̐��l��60�������ĕb�ɕϊ�����)
	m_second = score - (60 * m_minute);

	//�b�����߂�(�\�������߂邽�߂�10�Ŋ���)
	m_tenDigitSecond = m_second / 10;

	//�b�����߂�(�ꌅ�����߂邽�߂ɏ\��(�\���ɂ��邽�߂�10��������)�̐��l������)
	m_oneDigitSecond = m_second - (m_tenDigitSecond * 10);
}

//�X�R�A�̈ړ�
void Score::MoveScore()
{
	for (int i = 0; i < DRAW_RANKING; i++)
	{
		//��ڂ̈ړ�
		if (i - 1 < 0)
		{
			m_calXList[i] += SCORE_MOVE_SPEED;
			if (m_calXList[i] >= 0)
			{
				//�ړ�����
				m_calXList[i] = 0;
				m_calXCheckList[i] = true;
			}
			continue;
		}

		//����ȊO�̈ړ�
		if (m_calXCheckList[i - 1])
		{
			m_calXList[i] += SCORE_MOVE_SPEED;

			if (m_calXList[i] >= 0)
			{
				//�ړ�����
				m_calXList[i] = 0.0f;
				m_calXCheckList[i] = true;

				//�v�f��0���珇�ԂȂ���-1
				if (i == DRAW_RANKING - 1)
				{
					//�S�Ă̈ړ�������
					m_move = false;
				}
			}
		}
	}
}

//�ŐV�X�R�A�̕`�攻��
void Score::NewScoreDrawCheck()
{
	m_newScoreDrawInterval++;

	//�ŐV�X�R�A�̕`��Ԋu���v�Z����
	if (m_newScoreDrawInterval >= NEW_SCORE_DRAW_INTERVAL)
	{
		m_newScoreDraw = !m_newScoreDraw;
		m_newScoreDrawInterval = 0;
	}
}
