//----------------------------------------
// Effect3D.h
//----------------------------------------
#pragma once

#include "../../DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>

class Effect3D
{
public:
	void Create(DX::DeviceResources* &deviceResources,const wchar_t* fileName, const DirectX::SimpleMath::Vector3 &pos = DirectX::SimpleMath::Vector3::Zero);
	void Update(const DirectX::SimpleMath::Vector3 &thisPos, const DirectX::SimpleMath::Vector3 &targetPos, const DirectX::SimpleMath::Matrix &matrix);
	void Render();
	void Reset();

	void SetRenderState(const DirectX::SimpleMath::Vector3 &camera, const DirectX::SimpleMath::Matrix &view, const DirectX::SimpleMath::Matrix &proj);

	void RotationY(const DirectX::SimpleMath::Vector3 &thisPos, const DirectX::SimpleMath::Vector3 &targetPos);

	//�摜�̑傫���̐ݒ�
	void SetTextureScaleTL(const DirectX::SimpleMath::Vector2 &size);
	void SetTextureScaleBR(const DirectX::SimpleMath::Vector2 &size);
	//�摜�̐؂���ʒu��ݒ�
	void SetTextureSizeTL(const DirectX::SimpleMath::Vector2 &size);
	void SetTextureSizeBR(const DirectX::SimpleMath::Vector2 &size);
private:


	void Draw(const DirectX::SimpleMath::Matrix &world,const DirectX::SimpleMath::Matrix &view, const DirectX::SimpleMath::Matrix &proj);

private:
	DX::DeviceResources*                                                     m_pDeviceResources;
	DirectX::SimpleMath::Matrix                                              m_world;
	DirectX::SimpleMath::Matrix                                              m_view;
	DirectX::SimpleMath::Matrix                                              m_proj;
	// �G�t�F�N�g
	std::unique_ptr<DirectX::AlphaTestEffect>                                m_pBatchEffect;
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_pBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout>                                m_pInputLayout;

	std::unique_ptr<DirectX::CommonStates>                                   m_pStates;
	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                         m_pTexture;

	DirectX::SimpleMath::Vector3                                             m_position;
	DirectX::SimpleMath::Vector3                                             m_camera;

	//�摜�̑傫��
	//��ƍ�
	DirectX::SimpleMath::Vector2                                             m_textureScaleTL;
	//���ƉE
	DirectX::SimpleMath::Vector2                                             m_textureScaleBR;

	//�摜�̐؂���ʒu
	//��ƍ�
	DirectX::SimpleMath::Vector2                                             m_textureSizeTL;
	//���ƉE
	DirectX::SimpleMath::Vector2                                             m_textureSizeBR;
};