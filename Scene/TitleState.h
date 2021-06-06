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
		TitleState();

	public:
		virtual ~TitleState();


	public:
		void Initialize() override;
		void Update(DX::StepTimer timer) override;
		void Render() override;
		void Finalize() override;

private:
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

	std::vector<std::vector<std::vector<int>>>       m_mapDataList;


	ScreenCapture*                                   m_pScreenCapture;
};
