#pragma once

#include <SpriteBatch.h>
#include <SimpleMath.h>
#include "../DeviceResources.h"
#include <WICTextureLoader.h>

const static int TEXTUERSIZEX = 400;
const static int TEXTUERSIZEY = 54;

class HPGuage
{
public:
	HPGuage();
	~HPGuage();

	//生成
	void Create(ID3D11Device1* device, ID3D11DeviceContext1* context) ;
	//初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// リセット
	void Reset();
	
private:
	//画像保存
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Guage[3];

	std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;

	DirectX::SimpleMath::Vector2 m_Pos;

	float m_HP;

	RECT m_TextureSize;
};

