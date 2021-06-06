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

	//�v���C���[�̏���������
	void PlayerInitialize(const DirectX::SimpleMath::Vector3 &pos)override;
	//�v���C���[�̍X�V����
	void PlayerUpdate(const DX::StepTimer &timer)override;

	//�G�̏���������
	void EnemyInitialize()override;
	//�G�̍X�V����
	void EnemyUpdate(const DX::StepTimer &timer)override;

	//�J�����̈ʒu�̌v�Z�Ɛݒ�
	void Stage1::Cal_Set_EysPos()override;
};