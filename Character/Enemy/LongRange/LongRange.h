#pragma once

#include "../Enemy.h"

#include "../../MyEffect/Effect2D/Enemy/AttackLine.h"

#include "../LongRangeAttack/LongRangeAttack.h"

///======================================================
///เe
///ฃU๐sคล่Cไ
///======================================================
class LongRange : public Enemy
{
public:   //่
	//Uิ
	const static float ATTACK_TIME;
	//Uออ
	const static float ATTACK_RANGE;
	//Uิu
	const static int   ATTACK_INTERVAL;
	//ํฌออ
	const static float BATTLE_RANGE;
	//eฬUฌx(1bิฬฌx)
	const static float ATTACK_SPEED;

	//vC[๐ม๒ฮทอ(1bิษ)
	const static float BLOW_AWAY_POWER_PLAYER_TARGET;

	//GtFNgฬย
	//Uออ
	const static int   NUMBER_OF_EFFECT_ATTACK_AREA;
	//Uชฝมฝ
	const static int   NUMBER_OF_EFFECT_HIT;
	//S
	const static int   NUMBER_OF_EFFECT_DETH;

	//XE๓ิฬvZpฬl
	const static float SLOW_CAL;
	//สํ๓ิฬvZpฬl
	const static float NORMAL_CAL;

public:   //RXgEfXg
	LongRange(const DirectX::SimpleMath::Vector3 &pos, float attack, float hp, float speed, float thisRange);
	~LongRange();

public:   //ึioverridej
	//GtFNgฬ์ฌ
	void CreateEffect() override;

	//hถNXpXV
	void SubInitialize() override;
	//GtFNgฬ๚ป
	void EffectInitialize() override;

	//hถNXpXV
	void SubUpdate()override;
	//GtFNgฬXV
	void EffectUpdate(const DX::StepTimer &timer)override;

	//ถถฬ`ๆ
	void AliveDraw(const DirectX::SimpleMath::Matrix &viewMatrix, const DirectX::SimpleMath::Matrix &projection)override;

	//ํฌ
	void Battle()override;
	//U
	void Attack()override;
	//โ~
	void Stop()override;


	//ฺฎป่
	void SelectMove();
	//ํฌJnป่
	void SelectBattle()override;
	//UJnป่
	void SelectAttack()override;
	//โ~ป่
	void SelectStop()override;

	//vC[ฬWXg๑๐ป่
	void PlayerJustAvoidance()override;

	//UIน
	void EndAttack()override;

private:   //ึiวมj
	//Uฬฝ่ป่
	void HitAttack();

private:   //oฯ
	//Uออ
	std::unique_ptr<AttackLine>      m_pAttackLine;
	//UNX(e)
	std::unique_ptr<LongRangeAttack> m_pLongRangeAttack;
};