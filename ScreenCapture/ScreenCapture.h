#pragma once

#include "../DeviceResources.h"
#include <d3d11.h>

#include "../StepTimer.h"

#include "../MyEffect/Effect2D/ScreenBlur/ScreenBlur.h"

class ScreenCapture
{
public:
	ScreenCapture();
	~ScreenCapture();

	//初期化
	void Initialize();
	//更新
	void Update(DX::StepTimer timer);
	//描画
	void Draw();
	//リセット
	void Reset();

	//画像の取得(画面全体をキャプチャした画像)
	void TextureSetting();

private:
	//画像データ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  m_pTexture;

	//D3D10_TEXTURE2D_DESCでレンダーターゲットビューと宣言する
	D3D11_TEXTURE2D_DESC                              m_screenData;
	//レンダーターゲットテクスチャ
	Microsoft::WRL::ComPtr<ID3D11Texture2D>           m_pRenderTexture;
	//レンダーターゲットビュー
	ID3D11RenderTargetView *                          m_pRenderTargetView;

	std::unique_ptr<ScreenBlur>                       m_pScreenBlur;
};

