#pragma once
#include "../Stage.h"
class Stage1 : public Stage
{
public:
	Stage1();
	~Stage1();

	void Initialize() override;
	void Update(const DX::StepTimer &timer)override;
	void Render()override;
	void Reset()override;

	//プレイヤーの初期化処理
	void PlayerInitialize(const DirectX::SimpleMath::Vector3 &pos)override;
	//プレイヤーの更新処理
	void PlayerUpdate(const DX::StepTimer &timer)override;

	//敵の初期化処理
	void EnemyInitialize()override;
	//敵の更新処理
	void EnemyUpdate(const DX::StepTimer &timer)override;

	//カメラの位置の計算と設定
	void Stage1::Cal_Set_EysPos()override;
};