#pragma once

#include <memory>

#include "GameState.h"
#include "../DeviceResources.h"
#include "../StepTimer.h"
#include "CommonStates.h"
#include <SimpleMath.h>
#include "Mouse.h"
#include "Keyboard.h"
#include "Projection.h"
#include <vector>
#include <string>
#include "GeometricPrimitive.h"
#include "../StepTimer.h"

#include "../Stage/Stage.h"

class DebugCamera;
class GridFloor;
class Purpose;

class Time;

class DebugCapsule;

class FollowCamera;

class ADX2;

class PlayState : public GameState
{
public:
	PlayState();

public:
	virtual ~PlayState();


public:
	void Initialize() override;
	void Update(DX::StepTimer timer) override;
	void Render() override;
	void Finalize() override;

private:
	//その他の初期化処理(タイマー(スコアのUI、目的表示のエフェクト))
	void OtherInitialize();
	//その他の更新処理(タイマー(スコアのUI、目的表示のエフェクト))
	void OtherUpdate(DX::StepTimer timer);

	//ゲームクリア
	void GameClear();
	//ゲームオーバー
	void GameOver();
private:
	std::unique_ptr<DebugCamera>            m_pDebugCamera;
	//キーボード判定用
	std::unique_ptr<DirectX::Keyboard>      m_pKeyBord;

	//ステージ(プレイヤー、エネミー、カメラ、マップ)
	std::unique_ptr<Stage>                  m_pStage;

	//時間
	std::unique_ptr<Time>                   m_pTime;

	//目的表示エフェクト
	std::unique_ptr<Purpose>                m_pPurpose;
};
