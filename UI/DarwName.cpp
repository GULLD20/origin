#include "../pch.h"
#include "DarwName.h"

#include <SpriteBatch.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

#include "../Scene/GameContext.h"

//表示位置の基準
const DirectX::SimpleMath::Vector2 DrawName::BASE_POS = DirectX::SimpleMath::Vector2(45.0f, 11.0f);

DrawName::DrawName()
{

}

DrawName::~DrawName()
{

}

//生成
void DrawName::Create(ID3D11Device1* &device)
{
	//画像の読み込み
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/letter/HiraganaKatakana.png", NULL, m_pHiraganaKatakana.ReleaseAndGetAddressOf());
}

//初期化
void DrawName::Initialize()
{
	CSVLoad();
}

//描画
void DrawName::Draw(const std::string& name, const DirectX::SimpleMath::Vector2 &pos, const DirectX::CommonStates &state)
{
	//画像の表示
	for (int i = 0; i < static_cast<int>(name.size()); i++)
	{
		//name.substr(i * 2, 2)のi * 2はどこのデータを基準にするか(全角は二つのデータでできているため2ずつずらす)、2は二つのデータずつということ(全角は二つのデータでできているため)
		//18は一つの文字の大きさ
		GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pHiraganaKatakana.Get(), DirectX::SimpleMath::Vector2((pos.x + (i *18)+ BASE_POS.x), (pos.y+ BASE_POS.y)), &m_textureSizeMapList[name.substr(i * 2, 2)]);
		//表示予定の文字をすべて表示したら
		if (i * 2 >= static_cast<int>(name.size())-2)
		{
			break;
		}
	}
}

// リセット
void DrawName::Reset()
{
	m_pHiraganaKatakana.Reset();
}

//CSVファイルの読み込み
void DrawName::CSVLoad()
{
	// csvファイルのデータを全て入れる
	std::ifstream wifs;
	//マップデータを入れる
	std::string key;
	std::string data;

	//データ保存用
	int stertPosX = 0;
	int stertPosY = 0;
	int endPosX = 0;
	int endPosY = 0;

	//CSVの読み込み
	std::string failName = "Resources/CSV/letter.csv";
	wifs.open(failName);

	//開けなかったらスキップ
	if (!wifs.is_open())
		return;

	//「,」で文字を区切る
	while (getline(wifs, key, ','))
	{
		//「,」で文字を区切る
		getline(wifs, data, ',');
		//区切った数値を入れる
		stertPosX = std::stoi(data);

		//「,」で文字を区切る
		getline(wifs, data, ',');
		//区切った数値を入れる
		stertPosY = std::stoi(data);

		//「,」で文字を区切る
		getline(wifs, data, ',');
		//区切った数値を入れる
		endPosX = std::stoi(data);

		//「\n」(改行)で文字を区切る
		getline(wifs, data, '\n');
		//区切った数値を入れる
		endPosY = std::stoi(data);

		//1つの文字(全角)のみなので0の位置から2つ目までのデータをキーにする
		m_textureSizeMapList[key.substr(0, 2)] =
		{
		//画像の切り取り位置を保存
		stertPosX,
		stertPosY,
		endPosX,
		endPosY,
		};
	}
}