#pragma once

#include <map>
#include <vector>

class Character;

class CharacterMapList
{
public:
	CharacterMapList() {};
	~CharacterMapList() {};

	//�G���e�B�e�B��o�^����֐�
	void RegisterEntity(Character* entity);

	//�G���e�B�e�B���폜����֐�
	void RemoveEntity(Character* entity);

	//ID����G���e�B�e�B���擾����֐�
	Character* GetEntityFromID(int id) const;

	//�S�ẴL�[���擾����
	std::vector<int> GetAllEntityKey()const;

	//���Z�b�g�֐�
	void Reset();

private:
	std::map<int, Character*> m_pObjMapList;
};

