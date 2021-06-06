#pragma once

#include "../Weapon.h"

#include <SimpleMath.h>

#include <vector>

class Spear : public Weapon
{
public:
	//���̃T�C�Y
	const static DirectX::SimpleMath::Vector3              SIZE;
	//���̊�{�ʒu(�ړ���)
	const static DirectX::SimpleMath::Vector3              BASE_POSITION;
	//���̊�{�ʒu(�U����)
	const static DirectX::SimpleMath::Vector3              BASE_POSITION_ATTACK;
	//����̉�](�R���{)
	const static std::vector<DirectX::SimpleMath::Vector3> ANGLE;
	//�R���{�̐�
	const static int                                       MAX_COMBO;
	//�U������
	const static float                                     ATTACK_FLAME;

public:
	Spear();
	~Spear();

	void Create(DX::DeviceResources* &DeviceResources) override;
	//����������
	void Initialize() override;
	//�X�V����
	void Update(float elapsedSeconds) override;
	//�`�揈��
	void Draw(ID3D11DeviceContext1* context, const DirectX::CommonStates& State, const DirectX::SimpleMath::Matrix &viewMatrix, const DirectX::SimpleMath::Matrix &proj) override;
	//���Z�b�g
	void Reset() override;

	//�U���J�n
	void StartAttack()override;
	//�U�������擾
	bool GetAttackSTate()const override;

	//Matrix��ݒ肷��
	void MatrixSetting() override;
private:
	//�U������(��])
	void Attack() override;
	//�U���I��
	void EndAttack() override;

	//����̊֘A�f�[�^�̏�����
	void ResetWeaponData() override;

private:

};

