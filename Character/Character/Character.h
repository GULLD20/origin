#pragma once
#include <SimpleMath.h>
#include "../../StepTimer.h"
#include "../../MyLib/Collision.h"

class Character
{
public:
	enum State
	{
		Idle = 0,      //�ҋ@
		Move,          //�ړ�
		Battle,        //�퓬
		Attack,        //�U��
		Stop,          //��~
		HitDamage,     //�_���[�W
		Avoidance,     //���
		BlowAway,      //������΂���Ă�����
		Deth           //HP���[��(�����ȊO���f�[�^���Q�Ƃ���Ƃ��Ɏg��)
	};

	struct CharaacterData
	{
		//�L�����N�^�[�̈ʒu
		DirectX::SimpleMath::Vector3  pos;
		//��]�p�x
		DirectX::SimpleMath::Vector3  angle;
		// �v���C���[�̃��[���h�s��
		DirectX::SimpleMath::Matrix	  matrix;
		//�v���C���[�̉�]�s��
		DirectX::SimpleMath::Matrix   rotation;
		//HP
		float                         hp;
		//�ő�HP
		float                         maxHp;
		//�U����
		float                         attack;
		//����
		float                         speed;
		//���
		Character::State              state;
		//�����蔻��͈̔�
		float                         thisRange;
		//�����̃J�v�Z��(�����蔻��p)
		Collision::Capsule3D          thisCapsule;
		//�L�����N�^�[��ID
		int                           id;
	};

	//1�x
	const static float                        ONE_DEGREE;
	//�v���C���[�̃^�O
	const static std::string                  PLAYER_TAG;
	//�G�l�~�[�̃^�O
	const static std::string                  ENEMY_TAG;

public:
	Character(int id, const DirectX::SimpleMath::Vector3 &pos, float attack, float hp, float speed, float thisRange , const State &state = State::Idle);
	virtual ~Character();

	//����������
	virtual void Initialize() = 0;
	//�X�V����
	virtual void Update(const DX::StepTimer &timer) = 0;
	//�`�揈��
	virtual void Draw(const DirectX::SimpleMath::Matrix &viewMatrix) = 0;
	//���Z�b�g����
	virtual void Reset() = 0;

	//�_���[�W����
	virtual void Damage(float attack, const DirectX::SimpleMath::Matrix &rotation, bool blowAway, float power) = 0;
	//������я���
	virtual void BlowAwayMove() = 0;

public:
	//�ʒu�̎擾�E�ݒ�
	DirectX::SimpleMath::Vector3 GetPos()const { return m_charaData.pos; }
	void SetPos(const DirectX::SimpleMath::Vector3 &pos) { m_charaData.pos = pos; }

	//�U���͂̎擾
	float GetAttack() const { return m_charaData.attack; }

	//�̗͂̎擾�E�ݒ�
	float GetHP()const { return m_charaData.hp; }
	void SetHP(float hp) { m_charaData.hp = hp; }

	//�����̎擾
	float GetSpeed() const { return m_charaData.speed; }

	//�����蔻��͈̔͂̎擾
	float GetThisRange() const { return m_charaData.thisRange; }

	//��Ԃ̎擾
	State GetState()const { return m_charaData.state; }

	int GetID()const { return m_charaData.id; }
	void SetID(int id);

	Collision::Capsule3D GetCapsule() const { return m_charaData.thisCapsule; }
	void SetCapsule(const Collision::Capsule3D &capsule) { m_charaData.thisCapsule = capsule; };

	static int GetNextID() { return m_nextID; };

public:
	CharaacterData                  m_charaData;

	//���G����
	bool                            m_invincible;
	//���G����
	float                           m_invincibleTime;
	//�U����������������
	bool                            m_hitAttack;
	//�^�O
	std::string                     m_tag;

private:
	//����ID
	static int                      m_nextID;
};