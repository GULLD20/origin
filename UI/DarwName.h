#pragma once

#include <SimpleMath.h>
#include "../DeviceResources.h"
#include <WICTextureLoader.h>

#include<CommonStates.h>

#include <string>
#include <map>

class DrawName
{
private:
	//�\���ʒu�̊
	const static DirectX::SimpleMath::Vector2 BASE_POS;

public:
	DrawName();
	~DrawName();

	//����
	void Create(ID3D11Device1* &device);
	//������
	void Initialize();
	//�`��
	void Draw(const std::string& name, const DirectX::SimpleMath::Vector2 &pos, const DirectX::CommonStates &state);
	// ���Z�b�g
	void Reset();

	//CSV�t�@�C���̓ǂݍ���
	void CSVLoad();


private:
	//�摜�ۑ�
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pHiraganaKatakana;

	//�摜�T�C�Y
	std::map<std::string, RECT>                      m_textureSizeMapList;
};