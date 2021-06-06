#pragma once

#include <vector>
#include <SimpleMath.h>
#include <CommonStates.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <VertexTypes.h>

class SlashEffect
{
public:
	//���_�̐�
	const static int VERTEX_NUM;

public:
	SlashEffect();
	~SlashEffect();

	//�쐬
	void Creat();
	//������
	void Initialzie();
	//�X�V
	void Update();
	//�`��
	void Draw(const DirectX::SimpleMath::Matrix &view);
	//���Z�b�g
	void Reset();

	//����̐n�̒��S�Ɛn�̒��S�����[�Ɍ���������
	void SetWeaponData(const DirectX::SimpleMath::Vector3 &weaponC, const DirectX::SimpleMath::Vector3 &weaponC_T);
	//����̃}�g���b�N�X
	void SetWeaponMatrix(const DirectX::SimpleMath::Matrix &matrix);

private:
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_pPrimitiveBatch;
	//���_�̈ʒu�ƐF
	std::vector<DirectX::VertexPositionColor>                              m_pOrbitVertex;
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect>                                  m_pBasicEffect;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout>                              m_pInputLayout;

	//����̃��[���h�s��
	DirectX::SimpleMath::Matrix	                                           m_weaponMatrix;

	//����̒��S�ƒ��S�����[
	DirectX::SimpleMath::Vector3                                           m_weaponC;
	DirectX::SimpleMath::Vector3                                           m_weaponC_T;
};

