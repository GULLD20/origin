#include "../../../pch.h"
#include "EnemyDeth.h"
#include <WICTextureLoader.h>
#include "../../BinaryFile.h"
#include "d3d11.h"
#include <Effects.h>
#include <Model.h>

using namespace DirectX::SimpleMath;
using namespace DirectX;

const std::vector<D3D11_INPUT_ELEMENT_DESC> EnemyDeth::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
void EnemyDeth::Create(DX::DeviceResources* &deviceResources, int count)
{
	m_pDeviceResources = deviceResources;
	auto device = m_pDeviceResources->GetD3DDevice();

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePSEnemyDeth.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_pInputLayout.GetAddressOf());
	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_pVertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_pGeometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pPixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}
	// �v���~�e�B�u�o�b�`�̍쐬
	m_pBatch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(m_pDeviceResources->GetD3DDeviceContext());

	m_pStates = std::make_unique<CommonStates>(device);



	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_pBuffer);
	m_pFxFactory = std::make_unique<EffectFactory>(device);
	m_pFxFactory->SetDirectory(L"Resources");

	//m_model = Model::CreateFromCMO(device, L"Resources/cup.cmo", *m_pFxFactory);

	for (int i = 0; i < count; i++) {
		MyEffect* effect = new MyEffect();
		m_pEffectList.push_back(effect);
	}

}
void EnemyDeth::Lost() {
	for (std::list<MyEffect*>::iterator itr = m_pEffectList.begin(); itr != m_pEffectList.end(); itr++)
	{
		delete (*itr);
	}

	m_pBuffer.Reset();
	m_pStates.reset();

	// ���_�V�F�[�_
	m_pVertexShader.Reset();
	// �s�N�Z���V�F�[�_
	m_pPixelShader.Reset();
	// �W�I���g���V�F�[�_
	m_pGeometryShader.Reset();

	// �v���~�e�B�u�o�b�`
	m_pBatch.reset();
	// ���̓��C�A�E�g
	m_pInputLayout.Reset();
	m_vertexList.clear();

	m_pFxFactory.reset();
	//std::unique_ptr<DirectX::Model> m_model;


	m_pCapture.Reset();
	m_pRtv.Reset();
	m_pSrv.Reset();

	m_keyStateTracker.Reset();
}
void EnemyDeth::Initialize(float life, const DirectX::SimpleMath::Vector3 &pos)
{
	int range = 100;
	for (std::list<MyEffect*>::iterator itr = m_pEffectList.begin(); itr != m_pEffectList.end(); itr++)
	{
		Vector3 vel = Vector3::Zero;
		(*itr)->Initialize(life,pos,vel);
	}
	m_centerPosition = pos;
}
void EnemyDeth::InitializeNormal(float life, const DirectX::SimpleMath::Vector3 &pos)
{
	int range = 1000;
	for (std::list<MyEffect*>::iterator itr = m_pEffectList.begin(); itr != m_pEffectList.end(); itr++)
	{
		Vector3 vel=Vector3::Zero;
		(*itr)->Initialize(life, pos, vel);
	}
	m_centerPosition = pos;
}
void EnemyDeth::InitializeCorn(float life, const DirectX::SimpleMath::Vector3 &pos, const DirectX::SimpleMath::Vector3 &dir)
{
	int range = 10;
	Vector3 vel = Vector3::Zero;
	for (std::list<MyEffect*>::iterator itr = m_pEffectList.begin(); itr != m_pEffectList.end(); itr++)
	{
		Vector3 vel = Vector3(
			(((rand() % (range * 2)) - range)*0.1f / range) + dir.x,
			(((rand() % (range * 2)) - range)*0.1f / range) + dir.y,
			(((rand() % (range * 2)) - range)*0.1f / range) + dir.z);
		vel *= 1.5f;

		(*itr)->Initialize(life, pos, vel);
	}
	m_centerPosition = pos;
}
void EnemyDeth::Update(const DX::StepTimer &timer)
{
	m_timer = timer;
	for(std::list<MyEffect*>::iterator itr = m_pEffectList.begin(); itr != m_pEffectList.end();itr++)
	{
		(*itr)->Update(timer);
	}
	m_keyStateTracker.Update(DirectX::Keyboard::Get().GetState());
	if (m_keyStateTracker.IsKeyPressed(Keyboard::Up))
		m_menuId = (m_menuId+3 - 1) % 3;
	if (m_keyStateTracker.IsKeyPressed(Keyboard::Down))
		m_menuId = (m_menuId + 1) % 3;
	if (m_keyStateTracker.IsKeyPressed(Keyboard::Left))
		m_colorMode = (m_colorMode+4 - 1) % 4;
	if (m_keyStateTracker.IsKeyPressed(Keyboard::Right))
		m_colorMode = (m_colorMode + 1) % 4;
}

void EnemyDeth::Render()
{
	auto context = m_pDeviceResources->GetD3DDeviceContext();



	m_vertexList.clear();
	//�}�l�[�W���ŊǗ����Ă���G�t�F�N�g���C�e���[�^����
	for (auto itr = m_pEffectList.begin(); itr != m_pEffectList.end(); itr++)
	{
		//�G�t�F�N�g�̒��_�̍��W�Ƒ��x���擾����
		Vector3 pos = (*itr)->GetPosition();
		//���S�_����pos�̒l�������(Z���͔������Ȃ�)
		Vector3 vel = (*itr)->GetVelocity();

		{
			//�擾�������W��o�^����
			VertexPositionColorTexture vertex;
			vertex = VertexPositionColorTexture(pos, Vector4(vel.x,vel.y,vel.z,1), Vector2(0.0f,3.0f));
			//vertex = VertexPositionColorTexture(pos, Vector4::Zero, Vector2::Zero);
			m_vertexList.push_back(vertex);
		}
	}

	//��]���郂�f����\��
	static float rot = 0.0f;
	rot += 0.1f;
	//m_model->Draw(m_pDeviceResources->GetD3DDeviceContext(), *m_pStates, Matrix::CreateRotationZ(rot), m_view, m_proj);

	//�S��ʃG�t�F�N�g
	Matrix  mat = Matrix::Identity;
	//Draw(mat, mat, mat);

	//�|���S���G�t�F�N�g
	for (int i = 0; i < 4; i++)
	{
		//���C���̃f�[�^��ݒ�
		m_billboardTranslation = Matrix::CreateScale(m_scale);

		float angle = (3.141592f * 0.5f) * i;

		m_billboardTranslation *= Matrix::CreateRotationY(angle);

		m_billboardTranslation *= Matrix::CreateTranslation(m_mainPos);

		Draw(m_billboardTranslation, m_view, m_proj);
	}
}

void EnemyDeth::SetRenderState(const DirectX::SimpleMath::Vector3 &camera, const DirectX::SimpleMath::Matrix &view, const DirectX::SimpleMath::Matrix &proj)
{
	m_view = view;
	m_proj = proj;

	//m_billboardTranslation = Matrix::CreateBillboard(m_centerPosition, camera, Vector3::UnitY);

	//�r���{�[�h�̌v�Z�ŗ��Ԃ�̂ŕ␳
	//Y����180�x��]����s��
	Matrix rev = Matrix::Identity;
	rev._11 = -1;
	rev._33 = -1;

	//�␳�s����ɂ����đ��ɉe�����Ȃ��悤�ɂ���
	//m_billboardTranslation = rev * m_billboardTranslation;
}

void EnemyDeth::Draw(const DirectX::SimpleMath::Matrix &world, const DirectX::SimpleMath::Matrix &view, const DirectX::SimpleMath::Matrix &proj)
{
	auto context = m_pDeviceResources->GetD3DDeviceContext();

	//�萔�o�b�t�@�œn���l�̐ݒ�
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = world.Transpose();
	//Time		x:�o�ߎ���(�g�[�^���b)	y:1F�̌o�ߎ���(�b�j	z:�����i�T�C���J�[�u�j w:���g�p�i�b��łP�j
	cbuff.Time = Vector4(float(m_timer.GetTotalSeconds()), float(m_timer.GetElapsedSeconds()), sinf(float(m_timer.GetTotalSeconds())), m_alpha);
	DirectX::Mouse::State ms = DirectX::Mouse::Get().GetState();
	cbuff.Mouse = Vector4(ms.x/800.0f, ms.y/600.0f, ms.leftButton, ms.rightButton);
	cbuff.Mode = Vector4(static_cast<float>(m_menuId), static_cast<float>(m_colorMode), static_cast<float>(1), static_cast<float>(1));


	//�萔�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_pBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	ID3D11BlendState* blendstate = m_pStates->NonPremultiplied();
	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�͎Q�Ƃ̂�
	context->OMSetDepthStencilState(m_pStates->DepthRead(), 0);
	// �J�����O�͔����v���
	context->RSSetState(m_pStates->CullCounterClockwise());

	//�萔�o�b�t�@���V�F�[�_�ɓn���i�Ƃ肠����PS�͗v��Ȃ��̂ŃR�����g�A�E�g�j
	ID3D11Buffer* cb[1] = { m_pBuffer.Get() };
	//context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);


	//�T���v���[�A�V�F�[�_�A�摜�����ꂼ��o�^
	ID3D11SamplerState* sampler[1] = { m_pStates->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	context->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_pGeometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

	//���̓��C�A�E�g�𔽉f
	context->IASetInputLayout(m_pInputLayout.Get());

	//�o�b�`�ɒ��_����n��
	m_pBatch->Begin();
	//m_pBatch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_pBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertexList[0], m_vertexList.size());
	m_pBatch->End();


	//���̃��f���ɉe�����o��\��������̂Ŏg���I�������V�F�[�_���O��
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);



}

//���f�������e�N�X�`��(m_pSrv)�ɕ`�悷�鏈��
void EnemyDeth::RenderModel()
{
	auto context = m_pDeviceResources->GetD3DDeviceContext();

	D3D11_TEXTURE2D_DESC texDesc;
	m_pDeviceResources->GetRenderTarget()->GetDesc(&texDesc);
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_pDeviceResources->GetD3DDevice()->CreateTexture2D(&texDesc, NULL, m_pCapture.ReleaseAndGetAddressOf());

	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	// �����_�[�^�[�Q�b�g�r���[�̐���
	m_pDeviceResources->GetD3DDevice()->CreateRenderTargetView(m_pCapture.Get(), &rtvDesc, &m_pRtv);

	//�V�F�[�_���\�[�X�r���[�̐ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	//�����_�[�^�[�Q�b�g�r���[,�[�x�r���[���擾�i��Ō��ɖ߂����߁j
	ID3D11RenderTargetView* defaultRTV = m_pDeviceResources->GetRenderTargetView();
	ID3D11DepthStencilView* pDSV = m_pDeviceResources->GetDepthStencilView();

	//�w�i�F�̐ݒ�i�A���t�@���O�ɂ���ƃI�u�W�F�N�g�̂ݕ\���j
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 0.0f };

	//�����_�[�^�[�Q�b�g�r���[���Z�b�g���A����������
	context->OMSetRenderTargets(1, m_pRtv.GetAddressOf(), pDSV);
	context->ClearRenderTargetView(m_pRtv.Get(), clearColor);
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//----------------------------------------------------------------------------
	//�Ƃ肠�����̓����̂��߂ɉ�]
	static float rot = 0.0f;
	rot += 0.1f;

	//���f����`��
	//m_model->Draw(m_pDeviceResources->GetD3DDeviceContext(), *m_pStates, Matrix::CreateRotationZ(rot), m_view, m_proj);

	//�`�悵����ʂ�m_pSrv�ɕۑ�
	auto hr = m_pDeviceResources->GetD3DDevice()->CreateShaderResourceView(
		m_pCapture.Get(), &srvDesc, m_pSrv.ReleaseAndGetAddressOf());

	//------------------------------------------------------------------
	//�ݒ�����Ƃɖ߂�
	clearColor[0] = 0.3f;
	clearColor[1] = 0.3f;
	context->OMSetRenderTargets(1, &defaultRTV, pDSV);
	context->ClearRenderTargetView(defaultRTV, clearColor);
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);


}