//----------------------------------------
// Effect3D.cpp
//----------------------------------------

#include "../../pch.h"
#include "Effect3D.h"

#include "../../DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>

void Effect3D::Create(DX::DeviceResources* &deviceResources,const wchar_t* fileName,const DirectX::SimpleMath::Vector3 &pos)
{
	m_pDeviceResources = deviceResources;
	auto device = m_pDeviceResources->GetD3DDevice();
	// �G�t�F�N�g�̍쐬
	m_pBatchEffect = std::make_unique<DirectX::AlphaTestEffect>(device);
	m_pBatchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_pBatchEffect->SetReferenceAlpha(255);
	// ���̓��C�A�E�g����
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_pBatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_pInputLayout.GetAddressOf());


	// �v���~�e�B�u�o�b�`�̍쐬
	m_pBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(m_pDeviceResources->GetD3DDeviceContext());

	m_pStates = std::make_unique<DirectX::CommonStates>(device);
	// �e�N�X�`���̃��[�h
	DirectX::CreateWICTextureFromFile(device, fileName, nullptr,
		m_pTexture.GetAddressOf());
	m_position = pos;
}

void Effect3D::Update(const DirectX::SimpleMath::Vector3 &thisPos, const DirectX::SimpleMath::Vector3 &targetPos, const DirectX::SimpleMath::Matrix &matrix)
{
	//��������ΏۂɌ������x�N�g��
	DirectX::SimpleMath::Vector3 enemy_target_Vector;
	DirectX::SimpleMath::Vector3 baceFrontVec = DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f);

	enemy_target_Vector = targetPos - thisPos;

	//��������ΏۂɌ������x�N�g���̐��K��
	enemy_target_Vector.Normalize();

	//��̃x�N�g���̓��ς����߂�
	float dot = enemy_target_Vector.Dot(baceFrontVec);
	//���߂����ς̂Ȃ��p�����߂�
	float angle = std::acos(dot);

	//��̃x�N�g���̊O�ς����߂�
	DirectX::SimpleMath::Vector3 cros = enemy_target_Vector.Cross(baceFrontVec);

	//�O�ς�0�̎�(��̃x�N�g�����d�Ȃ��Ă���Ƃ�)�O�ς̒l��DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f)�ŌŒ肷��
	if (cros.Length() == 0.0f) cros = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);

	//���߂��O�ςƓ��ς�Quaternion�ɕϊ�
	DirectX::SimpleMath::Quaternion qua = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(-cros, angle);

	//�ϊ�����Quaternion���g���ĉ�]�s����쐬
	DirectX::SimpleMath::Matrix Rotmat = DirectX::SimpleMath::Matrix::Transform(DirectX::SimpleMath::Matrix::Identity, qua);

	//��]�s�����
	m_world = Rotmat * matrix;
	m_world *= DirectX::SimpleMath::Matrix::CreateTranslation(thisPos);
}

void Effect3D::Render()
{
	//m_world = DirectX::SimpleMath::Matrix::CreateBillboard(m_position, m_camera, DirectX::SimpleMath::Vector3::UnitY);
	Draw(m_world, m_view, m_proj);
}

void Effect3D::Reset()
{
	// ���̓��C�A�E�g
	m_pInputLayout.Reset();;

	// �e�N�X�`���n���h��
	m_pTexture.Reset();
}

void Effect3D::SetRenderState(const DirectX::SimpleMath::Vector3 &camera,const DirectX::SimpleMath::Matrix &view, const DirectX::SimpleMath::Matrix &proj)
{
	m_camera = camera;
	m_view = view;
	m_proj = proj;
}

void Effect3D::Draw(const DirectX::SimpleMath::Matrix &world, const  DirectX::SimpleMath::Matrix &view, const DirectX::SimpleMath::Matrix &proj)
{
	auto context = m_pDeviceResources->GetD3DDeviceContext();

	// ���_���i�|���S���̒��_�j
	DirectX::VertexPositionTexture vertex[4] =
	{
		DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3(m_textureScaleTL.x, m_textureScaleTL.y, 0.0f), DirectX::SimpleMath::Vector2(m_textureSizeTL.x, m_textureSizeTL.y)),
		DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3(m_textureScaleBR.x,  m_textureScaleTL.y, 0.0f),DirectX::SimpleMath::Vector2(m_textureSizeBR.x, m_textureSizeTL.y)),
		DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3(m_textureScaleBR.x, m_textureScaleBR.y, 0.0f), DirectX::SimpleMath::Vector2(m_textureSizeBR.x,m_textureSizeBR.y)),
		DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3(m_textureScaleTL.x,m_textureScaleBR.y, 0.0f), DirectX::SimpleMath::Vector2(m_textureSizeTL.x, m_textureSizeBR.y)),
	};
	// �e�N�X�`���T���v���[�̐ݒ�i�N�����v�e�N�X�`���A�h���b�V���O���[�h�j
	ID3D11SamplerState* samplers[1] = { m_pStates->LinearClamp() };
	context->PSSetSamplers(0, 1, samplers);
	ID3D11BlendState* blendstate = m_pStates->NonPremultiplied();

	CD3D11_DEFAULT default_state;

	CD3D11_BLEND_DESC bddesc(default_state);

	blendstate->GetDesc(&bddesc);
	bddesc.AlphaToCoverageEnable = TRUE;
	m_pDeviceResources->GetD3DDevice()->CreateBlendState(&bddesc, &blendstate);

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_pStates->DepthDefault(), 0);
	// �J�����O�͍�����i�����v���j
	//context->RSSetState(m_pStates->CullCounterClockwise());
	context->RSSetState(m_pStates->CullNone());
	// �s�����̂ݕ`�悷��ݒ�
	m_pBatchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_pBatchEffect->SetReferenceAlpha(0);
	m_pBatchEffect->SetWorld(m_world);
	m_pBatchEffect->SetView(m_view);
	m_pBatchEffect->SetProjection(m_proj);
	m_pBatchEffect->SetTexture(m_pTexture.Get());
	m_pBatchEffect->Apply(context);
	context->IASetInputLayout(m_pInputLayout.Get());

	// ������������`��
	m_pBatch->Begin();
	m_pBatch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_pBatch->End();

	//�쐬�����f�[�^���폜
	blendstate->Release();
}

void Effect3D::RotationY(const DirectX::SimpleMath::Vector3 &thisPos, const DirectX::SimpleMath::Vector3 &targetPos)
{
	m_position = thisPos;

	DirectX::SimpleMath::Vector3 targetPos_thisPos_vec = targetPos - thisPos;

	targetPos_thisPos_vec.Normalize();

	float dot = targetPos_thisPos_vec.Dot(DirectX::SimpleMath::Vector3::Forward);

	if (dot >= 1.0f)
	{
	dot = 1.0f;
	}

	float angle = std::acos(dot);
	float cross = targetPos_thisPos_vec.x*	DirectX::SimpleMath::Vector3::Forward.z - targetPos_thisPos_vec.z*	DirectX::SimpleMath::Vector3::Forward.x;

	float AngleY;

	if (cross <= 0)
	{
	AngleY = -angle;
	}
	else if(cross >= 0)
	{
	AngleY = angle;
	}

	m_world = DirectX::SimpleMath::Matrix::CreateRotationY(AngleY);
	m_world *= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
}

//�摜�̑傫���̐ݒ�
void Effect3D::SetTextureScaleTL(const DirectX::SimpleMath::Vector2 &size)
{
	m_textureScaleTL = size;
}

void Effect3D::SetTextureScaleBR(const DirectX::SimpleMath::Vector2 &size)
{
	m_textureScaleBR = size;
}

//�摜�̐؂���ʒu��ݒ�
void Effect3D::SetTextureSizeTL(const DirectX::SimpleMath::Vector2 &size)
{
	m_textureSizeTL = size;
}
void Effect3D::SetTextureSizeBR(const DirectX::SimpleMath::Vector2 &size)
{
	m_textureSizeBR = size;
}