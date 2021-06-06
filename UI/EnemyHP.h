#pragma once

#include <SimpleMath.h>
#include "../DeviceResources.h"
#include <WICTextureLoader.h>
#include <map>
#include <string>
#include <CommonStates.h>

#include "DarwName.h"

class EnemyHP
{
public:
	struct Data
	{
		//���O
		std::string name;
		//�G�l�~�[��HP
		float       hp;
		//�G�l�~�[��HP�̍ő�l
		float       maxHp;
		//�G�l�~�[�������Ă��邩
		bool        alive;
		//�G�l�~�[����������Ă��邩
		bool        discovery;
		//�퓬��
		bool        battle;
		//�摜�̃T�C�Y
		RECT        textureSize;
	};

	//�摜�̑傫��
	const static int                          TEXTUER_SIZE_X;
	const static int                          TEXTUER_SIZE_Y;

	//�ʒu�̊
	const static DirectX::SimpleMath::Vector2 BASE_POS;
	//HP�Q�[�W�̈ʒu�v�Z�p
	const static DirectX::SimpleMath::Vector2 HP_POS_CAL;
	//�c�̈ʒu�̌v�Z�p
	const static float                        POS_CAL_Y;

public:
	EnemyHP();
	~EnemyHP();

	//����
	void Create(ID3D11Device1* device, ID3D11DeviceContext1* context);
	//������
	void Initialize(float maxHp, const std::string& name,int createNumber);
	// �X�V
	void Update(float hp, int createNumber);
	// �`��
	void Draw(int createNumber, DirectX::CommonStates& state);
	// ���Z�b�g
	void Reset();

	//�����Ă��邩�̃f�[�^������
	void SetAlive(bool alive, int createNumber);
	//��������Ă��邩�̃f�[�^������
	void SetDiscovery(bool discovery , int createNumber);
	//�퓬��
	void SetBattle(bool battle, int createNumber);
private:
	//�摜�ۑ�
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pHpGuage;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pBack;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pDiscovery;

	//�f�[�^�̕ۑ�
	std::map<int,Data>                               m_dataMapList;
	//�\���ʒu�����߂�
	int                                              m_setingPos;

	std::unique_ptr<DrawName>                        m_pName;
};