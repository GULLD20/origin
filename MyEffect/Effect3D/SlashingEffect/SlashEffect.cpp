#include "../../../pch.h"
#include "SlashEffect.h"

#include <CommonStates.h>
#include "../../../DeviceResources.h"

#include "../../../Scene/GameContext.h"
#include "../../../Scene/Projection.h"

//�e�[�u��
static uint16_t s_indices[] =
{
	0, 1, 2,
	2, 1, 3,
};

//���_�̐�
const int SlashEffect::VERTEX_NUM = 10;

SlashEffect::SlashEffect()
{
}


SlashEffect::~SlashEffect()
{
	// �v���~�e�B�u�o�b�`
	m_pPrimitiveBatch.reset(nullptr);
	//���_�̈ʒu�ƐF
	m_pOrbitVertex.clear();
	// �x�[�V�b�N�G�t�F�N�g
	m_pBasicEffect.reset(nullptr);
	// ���̓��C�A�E�g
	m_pInputLayout.Reset();
}

//�쐬
void SlashEffect::Creat()
{
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();

	ID3D11DeviceContext1*  context = deviceResources->GetD3DDeviceContext();
	ID3D11Device1* device = deviceResources->GetD3DDevice();

	// �v���~�e�B�u�o�b�`�쐬
	m_pPrimitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);
	// �|���S���p�G�t�F�N�g�쐬
	m_pBasicEffect = std::make_unique<DirectX::BasicEffect>(device);
	void const * shaderByteCode;
	size_t byteCodeLength;
	//���_�̐F��L���ɂ���
	m_pBasicEffect->SetVertexColorEnabled(true);
	// �V�F�[�_�[�擾
	m_pBasicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	// ���̓��C�A�E�g�̍쐬�i���_�f�[�^�̒�`����j
	device->CreateInputLayout(DirectX::VertexPositionColor::InputElements,
		DirectX::VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_pInputLayout.GetAddressOf());
}

//������
void SlashEffect::Initialzie()
{
	//���_�̐���ݒ肷��
	m_pOrbitVertex.resize(VERTEX_NUM);

	//���_���Ƃ̐F�A�����x�����߂�
	for (int i = 0; i<VERTEX_NUM; i++)
	{
		//
		m_pOrbitVertex[i].color.x = 1.0f;
		m_pOrbitVertex[i].color.y = 1.0f;
		//m_pOrbitVertex[i].color.z = 0.5f;

		//�����x�̐ݒ�
		//��[�ɋ߂��قǓ����x�������Ȃ�
		//��[���痣��Ă���قǓ����x���Ⴍ�Ȃ�
		if (i % 2 == 0)
		{
			m_pOrbitVertex[i].color.w = 1.0f - i / static_cast<float>(VERTEX_NUM);
		}
		else
		{
			m_pOrbitVertex[i].color.w = 0.25f - i* 0.25f / static_cast<float>(VERTEX_NUM);
		}
	}
}

//�X�V
void SlashEffect::Update()
{
	//�a���̐�[�Ɩ��[
	DirectX::SimpleMath::Vector3 posT = DirectX::SimpleMath::Vector3::Up * 0.75f;
	DirectX::SimpleMath::Vector3 posB = DirectX::SimpleMath::Vector3::Up * -0.5f;

	for (int i = VERTEX_NUM - 4; i >= 0; i -= 2)
	{
		m_pOrbitVertex[i + 2].position = m_pOrbitVertex[i].position;
		m_pOrbitVertex[i + 3].position = m_pOrbitVertex[i + 1].position;
	}

	m_pOrbitVertex[0].position = DirectX::SimpleMath::Vector3::Transform(posT, m_weaponMatrix);
	m_pOrbitVertex[1].position = DirectX::SimpleMath::Vector3::Transform(posB, m_weaponMatrix);
}

//�`��
void SlashEffect::Draw(const DirectX::SimpleMath::Matrix &view)
{
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();
	DirectX::CommonStates* state = GameContext<DirectX::CommonStates>::Get();

	ID3D11DeviceContext1*  context = deviceResources->GetD3DDeviceContext();


	// TODO: Add your rendering code here.
	DirectX::SimpleMath::Matrix world;

	// ���[���h�s��ݒ�
	m_pBasicEffect->SetWorld(world);
	// �r���[�s��ݒ�
	m_pBasicEffect->SetView(view);
	// �v���W�F�N�V�����s��ݒ�
	m_pBasicEffect->SetProjection(GameContext<Projection>::Get()->GetMatrix());
	// �G�t�F�N�g�̐ݒ�
	m_pBasicEffect->Apply(context);
	// �[�x�X�e���V���X�e�[�g�̐ݒ�
	context->OMSetDepthStencilState(state->DepthDefault(), 0);
	// �u�����h�X�e�[�g�̐ݒ�
	context->OMSetBlendState(state->NonPremultiplied(), nullptr, 0xffffffff);
	// ���X�^���C�U�X�e�[�g��ݒ�
	context->RSSetState(state->CullNone());
	// ���̓��C�A�E�g�̐ݒ�
	context->IASetInputLayout(m_pInputLayout.Get());

	// �`��J�n
	m_pPrimitiveBatch->Begin();
	// ���_��񂩂�`��
	for (int i = 0; i<VERTEX_NUM - 2; i += 2)
	{
		//D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		//D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
		m_pPrimitiveBatch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, s_indices, 6, &m_pOrbitVertex[i], 4);
	}
	// �`��I��
	m_pPrimitiveBatch->End();
}

//���Z�b�g
void SlashEffect::Reset()
{

}

//����̐n�̒��S�Ɛn�̒��S�����[�Ɍ���������
void SlashEffect::SetWeaponData(const DirectX::SimpleMath::Vector3 &weaponC, const DirectX::SimpleMath::Vector3 &weaponC_T)
{
	//����̒��S�ƒ��S�����[
	m_weaponC =weaponC;
	m_weaponC_T = weaponC_T;
}

//����̃}�g���b�N�X
void SlashEffect::SetWeaponMatrix(const DirectX::SimpleMath::Matrix &matrix)
{
	m_weaponMatrix = matrix;
}