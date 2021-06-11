#pragma once

#include "Model.h"

#include <Effects.h>
#include "../../../DeviceResources.h"
#include <CommonStates.h>

#include <SimpleMath.h>

enum class WeaponType;

class Weapon
{
public:
	struct WeaponData
	{
		//����̃��[���h�s��
		DirectX::SimpleMath::Matrix		 matrix;
		//����̌��_�����[�܂ł̃x�N�g��
		DirectX::SimpleMath::Vector3     vector;
		//�U�����
		bool                             attack;
		//����̌��݂̊p�x
		DirectX::SimpleMath::Vector3     angle;
		//�R���{
		int                              combo;
		//�t���[��
		float                            flame;
		//�U����̎��̃R���{�܂ł̎���
		float                            betweenAttacksTimer;
		//�U������
		bool                             afterTheAttack;
	};

	const static float BETWEENATTACKSTIME;
public:
	Weapon();
	virtual ~Weapon();

	virtual void Create(DX::DeviceResources* &DeviceResources) = 0;
	//����������
	virtual void Initialize() = 0;
	//�X�V����
	virtual void Update(float elapsedSeconds) = 0;
	//�`�揈��
	virtual void Draw(ID3D11DeviceContext1* context, const DirectX::CommonStates& State, const DirectX::SimpleMath::Matrix &viewMatrix, const DirectX::SimpleMath::Matrix &proj) = 0;
	//���Z�b�g
	virtual void Reset() = 0;

	//�U���J�n
	virtual void StartAttack() = 0;

	//�U�������擾
	virtual bool GetAttackSTate() const = 0;

	//�R���{�擾
	virtual int GetCombo()const { return m_weapon.combo; };

	//Matrix��ݒ肷��
	virtual void MatrixSetting() = 0;

	void SetPlayerMatrix(const DirectX::SimpleMath::Matrix &playerMatrix) { m_playerMatrix = playerMatrix; };

	//���S�����[�Ɍ������x�N�g���̎擾
	DirectX::SimpleMath::Vector3 GetVector()const { return m_weapon.vector; };
	//���S�ʒu���擾
	DirectX::SimpleMath::Vector3 GetPos()const { return m_weapon.matrix.Translation(); };


private:
	//�U������(��])
	virtual void Attack() = 0;
	//�U���I��
	virtual void EndAttack() = 0;


	//����̊֘A�f�[�^�̏�����
	virtual void ResetWeaponData() = 0;

public:
	//����̊֘A�f�[�^
	WeaponData                       m_weapon;
	//���f��
	std::unique_ptr<DirectX::Model>  m_pModel;
	//�v���C���[�̍s��
	DirectX::SimpleMath::Matrix      m_playerMatrix;
};