#pragma once


#include "GameState.h"

#include <Keyboard.h>
#include "../StepTimer.h"

#include <SpriteBatch.h>

#include <vector>

#include "../ScreenCapture/ScreenCapture.h"

class Shutter;

class TitleState: public GameState
{
public:
	//マップの数(現在は1つだがテストで表示するために3にする)
	const static int MAX_MAP_SELECT;
	//マップの描画切り替え時間
	const static float MAP_DRAW_CHANGE_TIME;

	public:
		TitleState();

	public:
		virtual ~TitleState();


	public:
		void Initialize() override;
		void Update(DX::StepTimer timer) override;
		void Render() override;
		void Finalize() override;

private:
	//マップデータを読み込む
	void LoadMapData();
	//csvファイルの読み込み
	std::vector<std::vector<std::string>> LoadCSV();
	//要素数の取得
	int GettingElementCount(const std::vector<std::vector<std::string>> &allData);
	//文字列の分割
	std::vector<std::vector<std::vector<std::string>>> SeparateString(int size, const std::vector<std::vector<std::string>> &allData);
private:
	//キーボードの入力判定
	std::unique_ptr<DirectX::Keyboard>               m_pKeyBord;

	//シーン遷移の開始判定
	bool                                             m_checge;

	//PUSHSPACEの画像をする時間
	int                                              m_pushspaceDrawCount;
	//PUSHSPACEの画像の描画判定
	bool                                             m_pushspaceDraw;
	//PUSHSPACEの描画間隔
	int                                              m_pushspaceDrawInterval;

	//シーン遷移可能か判定
	bool                                             m_sceneTrandition;

	//タイトルの画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureTitle;
	//Pushspaceの画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexturePushspace;

	//マップデータのリスト（マップデータ自体が2次元配列で複数のマップを格納するため）
	std::vector<std::vector<std::vector<int>>>       m_mapDataList;

	//マップの描画切り替え用
	float m_mapDrawChengeTimer;
	//描画するマップの種類
	int m_mapSelect;

	//スクリーン用エフェクト（未実装）
	ScreenCapture*                                   m_pScreenCapture;
};
