#pragma once

#include "../../../DeviceResources.h"
#include "../../StepTimer.h"
#include <SimpleMath.h>
#include "../../Myeffect.h"
#include <Mouse.h>
#include <Keyboard.h>

#include <list>

class HitWall
{
public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Time;
		DirectX::SimpleMath::Vector4	Mouse;
		DirectX::SimpleMath::Vector4	Mode;
	};
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	void Create(DX::DeviceResources* &deviceResources, const wchar_t* fileName, int count);
	void Initialize(float life, const DirectX::SimpleMath::Vector3 &pos);
	void InitializeNormal(float life, const DirectX::SimpleMath::Vector3 &pos);
	void InitializeCorn(float life, const DirectX::SimpleMath::Vector3 &pos, const DirectX::SimpleMath::Vector3 &dir);
	void Update(const DX::StepTimer &timer);
	void Render();
	void Lost();

	void SetRenderState(const DirectX::SimpleMath::Vector3 &camera, const DirectX::SimpleMath::Matrix &view, const DirectX::SimpleMath::Matrix &proj);
	void Draw(const DirectX::SimpleMath::Matrix &world, const DirectX::SimpleMath::Matrix &view, const DirectX::SimpleMath::Matrix &proj);

	void RenderModel();

	//�ʒu�̐ݒ�
	//�v���C���[�̈ʒu
	void  SetPlayerPos(DirectX::SimpleMath::Vector3 pos) { m_playerPos = pos; };
	//�ǂ̈ʒu
	void  SetWallPos(DirectX::SimpleMath::Vector3 pos) { m_wallPos = pos; };

	//�p�x�̐ݒ�
	void SetRange(float angle) { m_angle = angle; };

	//�ǂ̈ʒu
	void  SetScale(DirectX::SimpleMath::Vector3 scale) { m_scale= scale; };


private:
	//MyEffect*					                                                  m_myEffect[10];
	std::list<MyEffect*>		                                                  m_pEffectList;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                              m_pTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                              m_pTexture2;


	DX::StepTimer                                                                 m_timer;


	DX::DeviceResources*			                                              m_pDeviceResources;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	                                      m_pBuffer;
	std::unique_ptr<DirectX::CommonStates>                                        m_pStates;

	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader>                                    m_pVertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader>                                     m_pPixelShader;
	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>                                  m_pGeometryShader;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_pBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout>                                     m_pInputLayout;
	std::vector<DirectX::VertexPositionColorTexture>                              m_vertexList;

	DirectX::SimpleMath::Vector3                                                  m_centerPosition;

	DirectX::SimpleMath::Matrix                                                   m_billboardTranslation;
	DirectX::SimpleMath::Matrix                                                   m_view;
	DirectX::SimpleMath::Matrix                                                   m_proj;

	std::unique_ptr<DirectX::EffectFactory>                                       m_pFxFactory;


	Microsoft::WRL::ComPtr<ID3D11Texture2D>                                       m_pCapture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	                              m_pRtv;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                              m_pSrv;

	DirectX::Keyboard::KeyboardStateTracker                                       m_keyStateTracker;

	int                                                                           m_menuId = 0;
	int                                                                           m_colorMode = 0;

	//�v���C���[�̈ʒu
	DirectX::SimpleMath::Vector3                                                  m_playerPos;

	//�ǂ̈ʒu
	DirectX::SimpleMath::Vector3                                                  m_wallPos;

	//�p�x
	float                                                                         m_angle;

	DirectX::SimpleMath::Vector3                                                  m_scale;

	//�s��
	DirectX::SimpleMath::Matrix                                                   m_world;
};