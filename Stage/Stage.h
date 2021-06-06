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

	//����������
	virtual void Initialize() = 0;
	//�X�V����
	virtual void Update(const DX::StepTimer &timer) = 0;
	//�`�揈��
	virtual void Render() = 0;
	//���Z�b�g����
	virtual void Reset() = 0;

	//�v���C���[�̏���������
	virtual void PlayerInitialize(const DirectX::SimpleMath::Vector3 &pos) = 0;
	//�v���C���[�̍X�V����
	virtual void PlayerUpdate(const DX::StepTimer &timer) = 0;

	//�G�l�~�[�̏���������
	virtual void EnemyInitialize() = 0;
	//�G�l�~�[�̍X�V����
	virtual void EnemyUpdate(const DX::StepTimer &timer) = 0;

	//�J�����̈ʒu�̌v�Z�Ɛݒ�
	virtual void Cal_Set_EysPos() = 0;

public:
	//�G�l�~�[�̍쐬
	void CreatEnemy(Params::EnemyTyp typ, DirectX::SimpleMath::Vector3 pos);
	//���̃G�l�~�[�̗̑͂�0�ɂȂ�����
	void EnemyDethCount();
	//�Q�[���N���A
	bool GameClear();
	//�Q�[���I�[�o�[
	bool GameOver();
	//CSV��ǂݍ���
	void LoadCsv(char* fileName);
	//�ǂݍ��񂾃f�[�^��ϐ��Ɋi�[����
	void SettingCsvData();

	FollowCamera* GetCamera() { return m_pFollowCamera.get(); };
	std::vector<std::vector<int>> GetMapData() { return m_mapDataList; };
	std::vector<std::vector<int>> GetCharacterData() { return m_characterData; };
private:
	//csv�̃f�[�^���i�[����ϐ�
	std::vector<std::vector<std::string>> m_csvData;
public:
	//�}�b�v�̃f�[�^���i�[����ϐ�
	std::vector<std::vector<int>>           m_mapDataList;
	//�L�����N�^�[�̈ʒu���ނ����߂�f�[�^������ϐ�
	std::vector<std::vector<int>>           m_characterData;

	//�v���C���[�N���X
	std::unique_ptr<Player>                 m_pPlayer;

	//�G�l�~�[�N���X
	std::vector<std::unique_ptr<Enemy>>                     m_pEnemy;
	//�쐬�����G�l�~�[�̐�
	int                                     m_creatEnemyNum;
	//�G�����񂾐�
	int                                     m_DethEnemyNum;

	//�J����
	std::unique_ptr<FollowCamera>           m_pFollowCamera;
};

