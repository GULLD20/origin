#pragma once

#include <map>
#include <vector>

class Character;

class CharacterMapList
{
public:   //コンスト・デスト
	CharacterMapList() {};
	~CharacterMapList() {};

public:   //関数
	//エンティティを登録する関数
	void RegisterEntity(Character* entity);

	//エンティティを削除する関数
	void RemoveEntity(Character* entity);

	//IDからエンティティを取得する関数
	Character* GetEntityFromID(int id) const;

	//全てのキーを取得する
	std::vector<int> GetAllEntityKey()const;

	//リセット関数
	void Reset();

public:   //メンバ変数（public）
	std::map<int, Character*> m_pObjMapList;
};

