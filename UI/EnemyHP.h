#pragma once

#include <SimpleMath.h>
#include "../DeviceResources.h"
#include <WICTextureLoader.h>
#include <map>
#include <string>
#include <CommonStates.h>

#include "DarwName.h"

class EnemyHP
{
public:
	struct Data
	{
		//名前
		std::string name;
		//エネミーのHP
		float       hp;
		//エネミーのHPの最大値
		float       maxHp;
		//エネミーが生きているか
		bool        alive;
		//エネミーが発見されているか
		bool        discovery;
		//戦闘中
		bool        battle;
		//画像のサイズ
		RECT        textureSize;
	};

	//画像の大きさ
	const static int                          TEXTUER_SIZE_X;
	const static int                          TEXTUER_SIZE_Y;

	//位置の基準
	const static DirectX::SimpleMath::Vector2 BASE_POS;
	//HPゲージの位置計算用
	const static DirectX::SimpleMath::Vector2 HP_POS_CAL;
	//縦の位置の計算用
	const static float                        POS_CAL_Y;

public:
	EnemyHP();
	~EnemyHP();

	//生成
	void Create(ID3D11Device1* device, ID3D11DeviceContext1* context);
	//初期化
	void Initialize(float maxHp, const std::string& name,int createNumber);
	// 更新
	void Update(float hp, int createNumber);
	// 描画
	void Draw(int createNumber, DirectX::CommonStates& state);
	// リセット
	void Reset();

	//生きているかのデータを入れる
	void SetAlive(bool alive, int createNumber);
	//発見されているかのデータを入れる
	void SetDiscovery(bool discovery , int createNumber);
	//戦闘中
	void SetBattle(bool battle, int createNumber);
private:
	//画像保存
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pHpGuage;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pBack;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pDiscovery;

	//データの保存
	std::map<int,Data>                               m_dataMapList;
	//表示位置を決める
	int                                              m_setingPos;

	std::unique_ptr<DrawName>                        m_pName;
};