#include "../pch.h"
#include "ScreenCapture.h"

#include "../DeviceResources.h"
#include <SpriteBatch.h>

#include "../Scene/GameContext.h"
#include "../Aspect.h"

ScreenCapture::ScreenCapture()
{
}


ScreenCapture::~ScreenCapture()
{
	//レンダーターゲットテクスチャ
	if (m_pRenderTexture != nullptr)
	{
		m_pRenderTexture.Reset();
		m_pRenderTexture = nullptr;
	}

	//レンダーターゲットビュー
	if (m_pRenderTargetView != nullptr)
	{
		m_pRenderTargetView->Release();
		m_pRenderTargetView = nullptr;
	}

	if (m_pScreenBlur != nullptr)
	{
		m_pScreenBlur->Lost();
		m_pScreenBlur.reset(nullptr);
	}

	if (m_pTexture != nullptr)
	{
		m_pTexture.Reset();
		m_pTexture = nullptr;
	}

}

//初期化
void ScreenCapture::Initialize()
{
	//DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>().Get();
	//ID3D11Device1* device = deviceResources->GetD3DDevice();

	////レンダーターゲットテクスチャを作成
	//memset(&m_screenData, 0, sizeof(m_screenData));
	//m_screenData.Width = static_cast<UINT>(Aspect::ASPECT_X);
	//m_screenData.Height = static_cast<UINT>(Aspect::ASPECT_Y);
	//m_screenData.MipLevels = 1;
	//m_screenData.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//m_screenData.SampleDesc.Count = 1;
	//m_screenData.Usage = D3D11_USAGE_DEFAULT;
	//m_screenData.ArraySize = 1;
	//m_screenData.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	//m_screenData.CPUAccessFlags = 0;
	//
	//m_pScreenBlur = std::make_unique<ScreenBlur>();
	//m_pScreenBlur->Create(deviceResources,1);
	//m_pScreenBlur->InitializeNormal(1,DirectX::SimpleMath::Vector3::Zero);
}

//更新
void ScreenCapture::Update(DX::StepTimer timer)
{
	TextureSetting();
	//m_pScreenBlur->Update(timer);
}

//描画
void ScreenCapture::Draw()
{
	//m_pScreenBlur->SetScreenTexture(m_pTexture);
	//m_pScreenBlur->SetRenderState(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Matrix::Identity, DirectX::SimpleMath::Matrix::Identity);
	//m_pScreenBlur->Render();

	GameContext<DirectX::SpriteBatch>::Get()->Draw(m_pTexture.Get(), RECT{0,0,1000,1000});
}

//リセット
void ScreenCapture::Reset()
{
}

//画像の取得(画面全体をキャプチャした画像)
void ScreenCapture::TextureSetting()
{
	//DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>().Get();
	//ID3D11Device1* device = deviceResources->GetD3DDevice();
	
	
	//D3D11_TEXTURE2D_DESC texDesc;
	//deviceResources->GetRenderTarget()->GetDesc(&texDesc);
	//texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	//texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//deviceResources->GetD3DDevice()->CreateTexture2D(&texDesc, NULL, m_pRenderTexture.ReleaseAndGetAddressOf());
	//
	//// レンダーターゲットビューの設定
	//D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	//memset(&rtvDesc, 0, sizeof(rtvDesc));
	//rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//// レンダーターゲットビューの生成
	//deviceResources->GetD3DDevice()->CreateRenderTargetView(m_pRenderTexture.Get(), &rtvDesc, &m_pRenderTargetView);
	//
	////レンダーターゲットビュー,深度ビューを取得（後で元に戻すため）
	//ID3D11RenderTargetView* defaultRTV = deviceResources->GetRenderTargetView();
	//ID3D11DepthStencilView* pDSV = deviceResources->GetDepthStencilView();
	//
	//
	//
	////レンダーターゲットテクスチャを登録するシェーダリソースビューを作成
	//ID3D11ShaderResourceView *rtShaderResView = nullptr;
	//device->CreateShaderResourceView(m_pRenderTexture.Get(), 0, &rtShaderResView);
	//
	////画像の登録
	//m_pTexture = rtShaderResView;
	//
	////シェーダリソースビューを削除
	//rtShaderResView->Release();





	
	////バックバッファのフォーマットを取得//
	//ID3D11Texture2D* pBackBuffer = nullptr;
	//HRESULT hr = deviceResources->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	//if (FAILED(hr)) return;
	//
	//D3D11_TEXTURE2D_DESC descBackBuffer;
	//pBackBuffer->GetDesc(&descBackBuffer);
	//pBackBuffer->Release();


	////CPU読み出し可能なバッファをGPU上に作成//
	//D3D11_TEXTURE2D_DESC Texture2DDesc;
	//Texture2DDesc.ArraySize = 1;
	//Texture2DDesc.BindFlags = 0;
	//Texture2DDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	//Texture2DDesc.Format = descBackBuffer.Format;
	//Texture2DDesc.Height = descBackBuffer.Height;
	//Texture2DDesc.Width = descBackBuffer.Width;
	//Texture2DDesc.MipLevels = 1;
	//Texture2DDesc.MiscFlags = 0;
	//Texture2DDesc.SampleDesc.Count = 1;
	//Texture2DDesc.SampleDesc.Quality = 0;
	//Texture2DDesc.Usage = D3D11_USAGE_STAGING;

	//deviceResources->GetRenderTarget()->GetDesc(&Texture2DDesc);
	//
	//ID3D11Texture2D *hCaptureTexture;
	//device->CreateTexture2D(&Texture2DDesc, 0, &hCaptureTexture);
	//
	//
	////作成したCPU読み込み可能バッファにGPU上でデータをコピー//
	//ID3D11Resource *hResource;
	//ID3D11RenderTargetView* pRenderTargetView;
	//device->CreateRenderTargetView(hCaptureTexture, NULL, &pRenderTargetView);
	//pRenderTargetView->GetResource(&hResource);
	//deviceResources->GetD3DDeviceContext()->CopyResource(hCaptureTexture, hResource);
	//hResource->Release();
	//
	////GPU上の読み込み可能バッファのメモリアドレスのマップを開く//
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//deviceResources->GetD3DDeviceContext()->Map(hCaptureTexture, 0, D3D11_MAP_READ, 0, &mappedResource);
	//
	////CPU上のメモリにバッファを確保//
	//double width = descBackBuffer.Width;
	//double height = descBackBuffer.Height;
	//double src_stride = mappedResource.RowPitch;    //(注)descBackBuffer.Width * 4とは必ずしも一致しない
	//size_t buffer_size = src_stride * height;
	//BYTE *bmp_buffer = new BYTE[buffer_size];
	//
	////GPU上の読み込み可能バッファからCPU上のバッファへ転送//
	//CopyMemory(bmp_buffer, mappedResource.pData, buffer_size);
	//deviceResources->GetD3DDeviceContext()->Unmap(hCaptureTexture, 0);
	//hCaptureTexture->Release();
	//
	////レンダーターゲットテクスチャを登録するシェーダリソースビューを作成
	//ID3D11ShaderResourceView *rtShaderResView = nullptr;
	//device->CreateShaderResourceView(hCaptureTexture, 0, &rtShaderResView);
	//
	////画像の登録
	//m_pTexture = rtShaderResView;
	//
	//delete[] bmp_buffer;

}