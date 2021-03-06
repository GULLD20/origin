//--------------------------------------------------------------------------------------
// File: DebugCapsule.h
//
// デバッグカプセルを描画するクラス
//
// Date: 2018.6.19
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include <SimpleMath.h>
#include <CommonStates.h>

class DebugCapsule
{
private:
	struct ConstantBuffer
	{
		DirectX::XMMATRIX worldViewProjection;
	};

public:
	// コンストラクタ
	DebugCapsule(ID3D11Device* device, const DirectX::SimpleMath::Vector3 &a, const DirectX::SimpleMath::Vector3 &b, float radius);

	// デバッグカプセルの描画関数
	void Draw(ID3D11DeviceContext* context, DirectX::CommonStates& states, const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);

	// デバッグカプセルの中間部の線分の設定関数
	void SetCenterSegment(const DirectX::SimpleMath::Vector3 &a, const DirectX::SimpleMath::Vector3 &b);

	// デバッグスフィアの半径の設定関数
	void SetRadius(float radius);

	// デバッグスフィアのセンターの取得関数
	void GetCenterSegment(DirectX::SimpleMath::Vector3* a, DirectX::SimpleMath::Vector3* b);

	// デバッグスフィアの半径の取得関数
	float GetRadius();

private:
	// 中間部の線分の開始点
	DirectX::SimpleMath::Vector3                    m_a;

	// 中間部の線分の終了点
	DirectX::SimpleMath::Vector3                    m_b;

	// 半径
	float                                           m_radius;

	// 平行移動行列
	DirectX::SimpleMath::Matrix                     m_trans;
	DirectX::SimpleMath::Matrix                     m_trans_head;

	// スケール行列
	DirectX::SimpleMath::Matrix                     m_scale;
	DirectX::SimpleMath::Matrix                     m_scale_head;

	// 回転行列
	DirectX::SimpleMath::Matrix                     m_rotate;
	DirectX::SimpleMath::Matrix                     m_rotate_head;

	// 深度ステンシルステート
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;

	// ラスタライザステート
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>   m_pRasterizerState;

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>            m_pVertexBuffer[2];

	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>            m_pIndexBuffer[2];

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>            m_pConstantBuffer;

	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader>      m_pVertexShader;

	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader>       m_pPixelShader;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_pInputLayout;
};
