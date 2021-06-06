#include "../pch.h"

#include "MyLib.h"

//3D�p-------------------------------------------------------------------

//�ړ��p
//����
//�����̈ʒu�Fpos�Ɛi�ދ����Fvec
DirectX::SimpleMath::Vector3 MyLib::Move(const DirectX::SimpleMath::Vector3 &pos, const DirectX::SimpleMath::Vector3 &vec)
{
	DirectX::SimpleMath::Vector3 movingPos;
	movingPos = pos + vec;

	return movingPos;
}

//����𔭌�(����)
//����
//�����̋��FthisSphere,����̋��FtargetSphere,�����Ă�������̃x�N�g���FforwardVec,���o(������͈�)�Fvisual
bool MyLib::Discovery(const Collision::Sphere3D &thisSphere, const  Collision::Sphere3D &targetSphere, const  DirectX::SimpleMath::Vector3 &forwardVec, float visual)
{
	//�v���C���[���G�̍��G�͈͂ɓ�������
	if (Collision::HitCheckoxSphere3D(thisSphere, targetSphere))
	{
		//���ʂɌ������x�N�g��
		DirectX::SimpleMath::Vector3 forward = forwardVec;
		//����(this)����^�[�Q�b�g(�v���C���[)�Ɍ������x�N�g��
		DirectX::SimpleMath::Vector3 enemy_target_Vec = targetSphere.center - thisSphere.center;

		//���K�����R
		//�v�Z���y
		//�傫�����֌W�Ȃ��Ȃ�
		//���K��
		forward.Normalize();
		enemy_target_Vec.Normalize();

		//��̃x�N�g���̓��ς����߂�
		float dot = enemy_target_Vec.Dot(forwardVec);

		//���߂����ς̂Ȃ��p�����߂�
		//���K�����ĂȂ��ꍇ
		//����g�����ߏd���Ȃ�
		float angle = std::acos(dot);

		//���E�͈͓̔�������
		if (angle <= visual && angle >= -visual)
		{
			return true;
		}
	}

	return false;
}

//�^�[�Q�b�g�̂ق��Ɍ���
//����
//�����̈ʒu�FthisPos,����̈ʒu�FtargetPos,���ʂɌ������x�N�g���FfrontVec
DirectX::SimpleMath::Matrix MyLib::LockAt(const DirectX::SimpleMath::Vector3 &thisPos, const  DirectX::SimpleMath::Vector3 &targetPos, const DirectX::SimpleMath::Vector3 &frontVec)
{
	//��������ΏۂɌ������x�N�g��
	DirectX::SimpleMath::Vector3 my_target_Vector = targetPos - thisPos;

	//��������ΏۂɌ������x�N�g���̐��K��
	my_target_Vector.Normalize();

	//��̃x�N�g���̓��ς����߂�
	float dot = my_target_Vector.Dot(frontVec);
	//���߂����ς̂Ȃ��p�����߂�
	float angle = std::acos(dot);

	//��̃x�N�g���̊O�ς����߂�
	DirectX::SimpleMath::Vector3 cros = my_target_Vector.Cross(frontVec);

	//�O�ς�0�̎�(��̃x�N�g�����d�Ȃ��Ă���Ƃ�)�O�ς̒l��DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f)�ŌŒ肷��
	if (cros.Length() == 0.0f) cros = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);

	//���߂��O�ςƓ��ς�Quaternion�ɕϊ�
	DirectX::SimpleMath::Quaternion qua = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(-cros, angle);

	//�ϊ�����Quaternion���g���ĉ�]�s����쐬
	DirectX::SimpleMath::Matrix Rotation = DirectX::SimpleMath::Matrix::Transform(DirectX::SimpleMath::Matrix::Identity, qua);

	return Rotation;
}

//�J�����̕`��͈͓�(�l�p)
//����
//�J�����𒆐S�Ƃ���(�������̓v���C���[�L����)�l�p�FthisBox,�Ώۂ̎l�p�FtargetBox
bool MyLib::CameraWithInDrawRange(const Collision::Box3D &thisBox, const Collision::Box3D &targetBox)
{
	//�`��͈͓�
	if (Collision::HitCheckoxBox3D(thisBox, targetBox))
	{
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------

//2D---------------------------------------------------------------------

//-----------------------------------------------------------------------

//���̑�-----------------------------------------------------------------
//���[�v�֐�
float MyLib::Lerp(float start, float end, float t)
{
	float c = (end - start) * t + start;
	return c;
}

//�������擾����֐�
//�����̍ŏ��l�FminimumNum�A�����̍ő�l�FmaxNum
//int�^�p
int MyLib::MersenneTwisterI(const int &minimumNum, const int &maxNum)
{
	//������Ԃ��֐�(�����Z���k�E�c�C�X�^�@)

	//�����_���Ȓl�����(���ꂾ�����Ǝ��s���x���x��)
	std::random_device rd;
	std::mt19937 mt(rd());
	//����
	std::uniform_int_distribution<int> mtRand(minimumNum, maxNum);

	return mtRand(mt);
}

//float�^�p
float MyLib::MersenneTwisterF(const float &minimumNum, const float &maxNum)
{
	//������Ԃ��֐�(�����Z���k�E�c�C�X�^�@)

	//�����_���Ȓl�����(���ꂾ�����Ǝ��s���x���x��)
	std::random_device rd;
	std::mt19937 mt(rd());
	//�����_
	std::uniform_real_distribution<float> mtRand(minimumNum, maxNum);

	return mtRand(mt);
}

//�e�L�X�g�`���̃t�@�C���̓ǂݍ���
//����
//�t�@�C���̈ʒu + �t�@�C���̖��O�FfileName
std::vector<std::string> MyLib::LoadFail_TextForm(const char* fileName)
{
	//�e�L�X�g�n�̃t�@�C���̃f�[�^���i�[����
	std::vector<std::string> csvData;
	//�������ꎞ�I�Ɋi�[����
	std::string temporary;
	//�e�L�X�g�n�̃t�@�C�����̂��i�[
	std::ifstream ifs;

	//�t�@�C���̓ǂݍ���
	ifs.open(fileName);

	//�J���Ă��Ȃ��Ƃ�
	if (!ifs.is_open())
		return csvData;

	//�ǂݍ��񂾃t�@�C���̃f�[�^��1�s���ۑ�
	while (getline(ifs, temporary))
	{
		//�f�[�^�������Ă��邩�m�F
		if (!temporary.empty())
		{
			csvData.push_back(temporary);
		}
	}

	return csvData;
}

//�e�L�X�g�n�̃f�[�^�̕ۑ�
//����
//�t�@�C���̈ʒu + �t�@�C���̖��O�FfileName , �ۑ�����f�[�^�Fdata
template<typename T>
void MyLib::SaveData(const char* fileName, const std::vector<T> &dataList)
{
	std::ofstream ofs(fileName);

	for (int i = 0; i < dataList.size(); i++)
	{
		ofs << dataList[i] << std::endl;
	}
}

//�f�[�^�𕡐��̃t�@�C���ɕۑ�����(�Ăяo�����тɃt�@�C����������)
//����
//�t�@�C���̈ʒu + �t�@�C���̖��O�FfileName , �t�@�C���`���FfileFormat , �ۑ�����f�[�^�Fdata
template<typename T>
void MyLib::SaveDataMultipleFiles(const std::string &fileName, const std::string &fileFormat, const std::vector<T> &dataList)
{
	std::string dot = ".";
	//���ڂ̃t�@�C�����w�肷��f�[�^������
	std::string num;
	//�t�@�C���𑀍삷�邽�߂ɕK�v
	std::ofstream ofs;
	//���ڂ̃t�@�C�����w�肷��
	int i = 0;
	//�t�@�C�������邩���f����
	bool open = true;

	while(open)
	{
		//int�^�̃f�[�^��std::string�^�ɕϊ�����
		num = std::to_string(i);
		//��������t�@�C���������߂�
		std::string name1 = fileName + num + dot + fileFormat;
		//�t�@�C�����J��
		std::ifstream ifs(name1);
		//�t�@�C�����J���Ă��邩�m�F
		open = ifs.is_open();
		//�t�@�C�����J���Ă��Ȃ�������(���݂��Ă��Ȃ�������)
		if(!open)
		{
			//�t�@�C���̍쐬
			ofs.open(name1);
			break;
		}
		i++;
	}

	//�f�[�^�̕ۑ�
	for(int i = 0; i < dataList.size(); i++)
	{
		ofs << dataList[i] << std::endl;
	}
}

//�����̕���
//����
//�������i�[����Ă���ϐ�(std::string�^)�Fstr,��؂镶���Fdelimiter
std::vector<std::string> MyLib::SeparateString(const std::string &str, const char delimiter)
{
	//���������������i�[����ϐ�
	std::vector<std::string> elems;
	//�󂯎�����������i�[����ϐ�(getline�Ŏg������)
	std::stringstream ss(str);
	//���������������ꎞ�I�Ɋi�[����ϐ�
	std::string temporary;

	//�����ł��Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (getline(ss, temporary, delimiter))
	{
		//�f�[�^�������Ă���Ȃ�
		if (!temporary.empty())
		{
			elems.push_back(temporary);
		}
	}
	return elems;
}