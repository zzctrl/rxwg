#pragma once
#include "AddressData.h"
#include "CommonRead.h"

/*
热血对象基类
*/

// 对象类型：怪物，NPC，物品，玩家
enum EntityType { ET_NULL = 0, ET_Monster, ET_NPC, ET_Goods, ET_Player };

class EntityBase
{
public:
	enum { ID_NULL = 0xFFFF };
	
	// 获取ID对应的对象类型
	static EntityType GetTypeWithID(DWORD a_id);

public:
	EntityBase();
	EntityBase(DWORD a_id = ID_NULL);
	~EntityBase();

	void SetID(DWORD a_id);
	DWORD GetID();
	EntityType GetType();

	// 获取对象地址
	DWORD GetNation();

	// 获取对象名称
	CString GetName();

	// 选中(或取消选中)当前对象
	virtual void Select();
	void UnSelect();

	// 获取对象等级
	int GetLevel();
	
	// 清空对象属性
	void Reset();
protected:
	// 对象ID和类型
	DWORD m_id;
	EntityType m_type;
	// 对象地址
	DWORD m_nation;
};

