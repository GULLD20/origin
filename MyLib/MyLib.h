///�l���C�u�����[------------------------------
//�K�{---------
//��
//DirectXTK
//�C���N���[�h
//<SimpleMath>
//"Colition.h"
//<string>
//<sstream>
//<fstream>
//<vector>
//<random>
//-------------
//���e
//3D�p
//�ړ��FMove
//�ʒu�Ɛi�ޕ����Ɍ������x�N�g�����󂯎��
//�x�N�g���ɍ��킹�Ĉړ�
//����𔭌�(����)�FDiscovery
//���͈͓̔��ɑ��肪����Ƃ��Ɏ���(this)�̎��E�̒�(-visual�`visual�̊�)�ɂ���Δ���(true��Ԃ�)
//
//2D�p
//
//���̑�
//���[�v�֐��FLerp
//���[�v����
//�����̎擾 int�^�FMersenneTwisterI
//�����̎擾 float�^�FMersenneTwisterF
//�����Z���k�c�C�X�^�[���ŗ����擾
//�����̐��l�̊Ԃ𗐐��Ŏ擾����
//�e�L�X�g�n�̃t�@�C��(�m�F�ς݁Ftext,csv)�̓ǂݍ��݁FLoadFail_TextForm
//�󂯎�����t�@�C���������Ƀe�L�X�g�n�t�@�C���̓ǂݍ��݂��s����
//�f�[�^�̕ۑ��FSaveData
//�w�肵���e�L�X�g�n�t�@�C���Ƀf�[�^��ۑ�
//�����̕����FSeparateString
//�󂯎�����X�g�����O�^�̃f�[�^���󂯎����char�^�̃f�[�^�ŋ�؂�
///----------------------------------------------

#pragma once

//�C���N���[�h
#include <SimpleMath.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <random>

//�����蔻��
#include "Collision.h"
class MyLib
{
public:
	//3D�p-------------------------------------------------------------------

	//�ړ��p
	//����
	//�����̈ʒu�Fpos�Ɛi�ދ����Fvec
	static DirectX::SimpleMath::Vector3 Move(const DirectX::SimpleMath::Vector3 &pos, const DirectX::SimpleMath::Vector3 &vec);

	//����𔭌�(����)
	//����
	//�����̋��FthisSphere,����̋��FtargetSphere,�����Ă�������̃x�N�g���FforwardVec,���o(������͈�)�Fvisual
	static bool Discovery(const Collision::Sphere3D &thisSphere, const  Collision::Sphere3D &targetSphere, const  DirectX::SimpleMath::Vector3 &forwardVec, float visual);

	//�^�[�Q�b�g�̂ق��Ɍ���
	//����
	//�����̈ʒu�FthisPos,����̈ʒu�FtargetPos,���ʂɌ������x�N�g���FfrontVec
	static DirectX::SimpleMath::Matrix LockAt(const DirectX::SimpleMath::Vector3 &thisPos, const  DirectX::SimpleMath::Vector3 &targetPos, const DirectX::SimpleMath::Vector3 &frontVec);

	//�J�����̕`��͈͓�(�l�p)
	//����
	//�J�����𒆐S�Ƃ���(�������̓v���C���[�L����)�l�p�FthisBox,�Ώۂ̎l�p�FtargetBox
	static bool CameraWithInDrawRange(const Collision::Box3D &thisBox, const Collision::Box3D &targetBox);
	//-----------------------------------------------------------------------

	//2D---------------------------------------------------------------------

	//-----------------------------------------------------------------------

	//���̑�-----------------------------------------------------------------
	//���[�v�֐�
	static float Lerp(float start, float end, float t);

	//�������擾����֐�
	//�����̍ŏ��l�FminimumNum�A�����̍ő�l�FmaxNum
	//int�^�p
	static int MersenneTwisterI(const int &minimumNum, const int &maxNum);

	//float�^�p
	static float MersenneTwisterF(const float &minimumNum, const float &maxNum);

	//�e�L�X�g�`���̃t�@�C���̓ǂݍ���
	//����
	//�t�@�C���̈ʒu + �t�@�C���̖��O�FfileName
	static std::vector<std::string> LoadFail_TextForm(const char* fileName);

	//�e�L�X�g�n�̃f�[�^�̕ۑ�
	//����
	//�t�@�C���̈ʒu + �t�@�C���̖��O�FfileName , �ۑ�����f�[�^�Fdata
	template<typename T>
	static void SaveData(const char* fileName, const std::vector<T> &dataList);

	//�f�[�^�𕡐��̃t�@�C���ɕۑ�����(�Ăяo�����тɃt�@�C����������)
	//����
	//�t�@�C���̈ʒu + �t�@�C���̖��O�FfileName , �t�@�C���`���FfileFormat , �ۑ�����f�[�^�Fdata
	template<typename T>
	static void SaveDataMultipleFiles(const std::string &fileName, const std::string &fileFormat, const std::vector<T> &dataList);

	//�����̕���
	//����
	//�������i�[����Ă���ϐ�(std::string�^)�Fstr,��؂镶���Fdelimiter
	static std::vector<std::string> SeparateString(const std::string &str, const char delimiter);
	//-----------------------------------------------------------------------
};