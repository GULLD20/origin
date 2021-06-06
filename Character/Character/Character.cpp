#include "../../pch.h"
#include "Character.h"

#include "../List/CharacterMapList.h"
#include "../../Scene/GameContext.h"

//1�x
const float Character::ONE_DEGREE = DirectX::XM_PI / 180.0f;
//�v���C���[�̃^�O
const std::string Character::PLAYER_TAG = "player";
//�G�l�~�[�̃^�O
const std::string Character::ENEMY_TAG = "enemy";

int Character::m_nextID = 0;

Character::Character(int id, const DirectX::SimpleMath::Vector3 &pos, float attack, float hp, float speed, float thisRange, const State &state)
	:m_charaData{
	//�L�����N�^�[�̈ʒu
	pos,
	//��]�p�x
	DirectX::SimpleMath::Vector3::Zero,
	// �v���C���[�̃��[���h�s��
	DirectX::SimpleMath::Matrix::Identity,
	//�v���C���[�̉�]�s��
	DirectX::SimpleMath::Matrix::Identity,
	//HP
	hp,
	//�ő�HP
	hp,
	//�U����
	attack,
	//����
	speed,
	//���
	state,
	//�����蔻��͈̔�
	thisRange,
	//�����̃J�v�Z��(�����蔻��p)
	Collision::Capsule3D{pos ,pos,thisRange},
	//�L�����N�^�[��ID
	id,
}
{
	GameContext<CharacterMapList>::Get()->RegisterEntity(this);

	SetID(id);
}

Character::~Character()
{

}

void Character::SetID(int id)
{
	m_charaData.id = id;

	m_nextID = m_charaData.id + 1;
}