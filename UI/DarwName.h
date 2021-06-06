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
	//表示位置の基準
	const static DirectX::SimpleMath::Vector2 BASE_POS;

public:
	DrawName();
	~DrawName();

	//生成
	void Create(ID3D11Device1* &device);
	//初期化
	void Initialize();
	//描画
	void Draw(const std::string& name, const DirectX::SimpleMath::Vector2 &pos, const DirectX::CommonStates &state);
	// リセット
	void Reset();

	//CSVファイルの読み込み
	void CSVLoad();


private:
	//画像保存
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pHiraganaKatakana;

	//画像サイズ
	std::map<std::string, RECT>                      m_textureSizeMapList;
};