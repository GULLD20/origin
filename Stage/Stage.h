#pragma once
#include "../StepTimer.h"
#include <string>
#include <vector>

#include "../Camera/FollowCamera.h"

#include "../Character/Player/Player.h"
#include "../Character/Player/Weapon/WeaponType.h"

#include "../Character/Enemy/Enemy.h"

#include "../Character/Params/Params.h"

class Stage
{
public:
	Stage();
	virtual ~Stage();

	//初期化処理
	virtual void Initialize() = 0;
	//更新処理
	virtual void Update(const DX::StepTimer &timer) = 0;
	//描画処理
	virtual void Render() = 0;
	//リセット処理
	virtual void Reset() = 0;

	//プレイヤーの初期化処理
	virtual void PlayerInitialize(const DirectX::SimpleMath::Vector3 &pos) = 0;
	//プレイヤーの更新処理
	virtual void PlayerUpdate(const DX::StepTimer &timer) = 0;

	//エネミーの初期化処理
	virtual void EnemyInitialize() = 0;
	//エネミーの更新処理
	virtual void EnemyUpdate(const DX::StepTimer &timer) = 0;

	//カメラの位置の計算と設定
	virtual void Cal_Set_EysPos() = 0;

public:
	//エネミーの作成
	void CreatEnemy(Params::EnemyTyp typ, DirectX::SimpleMath::Vector3 pos);
	//何体エネミーの体力が0になったか
	void EnemyDethCount();
	//ゲームクリア
	bool GameClear();
	//ゲームオーバー
	bool GameOver();
	//CSVを読み込む
	void LoadCsv(char* fileName);
	//読み込んだデータを変数に格納する
	void SettingCsvData();

	FollowCamera* GetCamera() { return m_pFollowCamera.get(); };
	std::vector<std::vector<int>> GetMapData() { return m_mapDataList; };
	std::vector<std::vector<int>> GetCharacterData() { return m_characterData; };
private:
	//csvのデータを格納する変数
	std::vector<std::vector<std::string>> m_csvData;
public:
	//マップのデータを格納する変数
	std::vector<std::vector<int>>           m_mapDataList;
	//キャラクターの位置や種類を決めるデータを入れる変数
	std::vector<std::vector<int>>           m_characterData;

	//プレイヤークラス
	std::unique_ptr<Player>                 m_pPlayer;

	//エネミークラス
	std::vector<std::unique_ptr<Enemy>>                     m_pEnemy;
	//作成したエネミーの数
	int                                     m_creatEnemyNum;
	//敵が死んだ数
	int                                     m_DethEnemyNum;

	//カメラ
	std::unique_ptr<FollowCamera>           m_pFollowCamera;
};

