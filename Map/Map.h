#pragma once

#include "Model.h"

#include <Effects.h>
#include "../../DeviceResources.h"
#include <CommonStates.h>
#include "../../Scene/Projection.h"
#include "../../Scene/GameContext.h"

#include <GeometricPrimitive.h>
#include <SimpleMath.h>

#include <vector>

#include "../MyLib/Collision.h"

#include "../MyEffect/Effect2D/HitWall/HitWall.h"

class CollisionMesh;

class Map
{
public:
	enum CheckLimitEffect
	{
		CheckRight = 0,
		CheckLeft,
		CheckZ
	};

	//�}�b�v�̃f�[�^(��ށA�ʒu�A�����蔻��̑傫��)
	struct MapData
	{
		int selectData;
		DirectX::SimpleMath::Vector3 pos;
		DirectX::SimpleMath::Vector3 size;
	};

	//��̊p�̈ʒu
	struct TwoCornerPos
	{
		DirectX::SimpleMath::Vector3 pos1;
		DirectX::SimpleMath::Vector3 pos2;
	};

	//�ǂ̓�����͔���̑傫��
	const static DirectX::SimpleMath::Vector3 WALL_BOX_SIZE;
	//�ړ����E���̃G�t�F�N�g�̐�
	const static int                          LIMIT_EFFECT_NUM;
	//�G�t�F�N�g�̊p�x
	//�E
	const static float                        EFFECT_ANGLE_R;
	//��
	const static float                        EFFECT_ANGLE_L;

public:
	Map();
	~Map();

	//�쐬
	void Create();
	//����������
	void Initialize();
	//�`�揈��
	void Draw(const DirectX::SimpleMath::Matrix &viewMatrix);
	//�^�C�g����ʂł̃}�b�v�̕\��
	void TitleDraw(const DirectX::SimpleMath::Matrix &viewMatrix);
	//���Z�b�g����
	void Reset();
	//�ǂ̓����蔻��
	DirectX::SimpleMath::Vector3 HitWallBox(const Collision::Box3D& target);
	//�Ԃɕǂ����邩
	DirectX::SimpleMath::Vector3 LineOfSight(const DirectX::SimpleMath::Vector3 &pos, const DirectX::SimpleMath::Vector3 &baseFront, const DirectX::SimpleMath::Matrix &rotation);
	//�ړ����E�̕�
	DirectX::SimpleMath::Vector3 Limit(const DirectX::SimpleMath::Vector3 &pos, float size);
	//�ړ����E�̕ǂ̃G�t�F�N�g(�v���C���[�����Ăяo���Ȃ�)
	void LimitEffect(const DirectX::SimpleMath::Vector3 &pos, float size);
	//�G�t�F�N�g�p�̈ʒu��ݒ�
	void LimitEffectSetPos(const DirectX::SimpleMath::Vector3 &playerPos, const DirectX::SimpleMath::Vector3 &wallPos, const CheckLimitEffect &check);
	//�������������̕ǂ̓�̊p�̈ʒu��Ԃ�
	TwoCornerPos GetHitWallCorner (const DirectX::SimpleMath::Vector3 &pos) const;

	//�}�b�v�f�[�^�̐ݒ�
	void SetMapData(std::vector<std::vector<int>> mapDataList);
private:
	//�n�ʂ̍쐬
	void CreatGround();
	//�ǂ̍쐬
	void CreatWall();
private:
	//�}�b�v�f�[�^
	std::vector<std::vector<int>>                   m_MapDataList;

	///�n��
	//���f��
	std::unique_ptr<DirectX::Model>                 m_pGroundModel;
	//���[���h�s��
	DirectX::SimpleMath::Matrix                     m_groundWorld;
	///��
	//�����蔻��
	std::vector<Collision::Box3D>                   m_wallBoxList;
	//���f��
	std::unique_ptr<DirectX::Model>                 m_pWallModel;

	//�}�b�v�̏c���̍L���̔���
	float                                           m_halfHightSize;
	float                                           m_halfWidthSize;

	//�I�����ꂽ�}�b�v
	int                                             m_selectMap;

	///�V��
	//���f��
	std::unique_ptr<DirectX::Model>                 m_pSpace;
	//���[���h�s��
	DirectX::SimpleMath::Matrix                     m_spaceWorld;
	//y���̉�]�p�x
	float                                           m_spaceAngleY;

	//�v���C���[�ǂɓ����������̃G�t�F�N�g(0�Ԗڂɂ͉�(X��)�̈ʒu��1�Ԗڂɂ͏c(Z��)�̈ʒu������)
	std::vector<std::unique_ptr<HitWall>>           m_pHitWallList;
};

