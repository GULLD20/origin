#pragma once

#include <SimpleMath.h>
#include "../DeviceResources.h"
#include <WICTextureLoader.h>

class Time
{
public:
	//タイマーの描画位置
	const static DirectX::SimpleMath::Vector2 DRAW_POS_1;
	const static DirectX::SimpleMath::Vector2 DRAW_POS_2;

public:
	Time();
	~Time();

	//生成
	void Create(ID3D11Device1* device) ;
	//初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// リセット
	void Reset();

	//プレイシーンの合計時間を取得
	float GetPlayTime() const { return m_total; };

	//1フレームにかかった時間を設定
	void SetElapsedTime(const float &elapsedTime) { m_elapsedTime = elapsedTime; };

	//1フレームにかかった時間を取得
	float GetElapsedTime() const { return m_elapsedTime; };

private:
	//画像保存
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pNum[10];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pColon;

	DirectX::SimpleMath::Vector2                     m_basePos;

	float                                            m_total;
	int                                              m_time;
	int                                              m_second;
	int                                              m_minute;
	int                                              m_oneDigitSecond;
	int                                              m_tenDigitSecond;
	int                                              m_oneDigitMinute;
	int                                              m_tenDigitMinute;

	float                                            m_elapsedTime;
};

