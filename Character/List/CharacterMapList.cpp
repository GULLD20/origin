#include "../../pch.h"
#include "CharacterMapList.h"

#include "../Character/Character.h"

//�G���e�B�e�B��o�^����֐�
void CharacterMapList::RegisterEntity(Character* entity)
{
	//�v�f�̒ǉ�(id���L�[�Ƃ��ėv�f��ǉ�����)
	m_pObjMapList.insert(std::make_pair(entity->GetID(), entity));
}

//�G���e�B�e�B���폜����֐�
void CharacterMapList::RemoveEntity(Character* entity)
{
	m_pObjMapList.erase(m_pObjMapList.find(entity->GetID()));
}

//ID����G���e�B�e�B���擾����֐�
Character* CharacterMapList::GetEntityFromID(int id) const
{
	//�}�b�v�̒���id�Ō������ėv�f(second)��n��
	return m_pObjMapList.find(id)->second;
}

//�S�ẴL�[���擾����
std::vector<int>  CharacterMapList::GetAllEntityKey()const
{
	std::vector<int> allKey;

	for(const auto& itr : m_pObjMapList)
	{
		allKey.push_back(itr.first);
	}

	return allKey;
}

//���Z�b�g�֐�
void CharacterMapList::Reset()
{
	m_pObjMapList.clear();
}