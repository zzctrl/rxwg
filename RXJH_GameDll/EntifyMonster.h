#pragma once
#include "EntityBase.h"

/*
怪物对象类
*/

class EntifyMonster : public EntityBase
{
public:
	EntifyMonster(DWORD a_id = ID_NULL);
	~EntifyMonster();

	// 获取怪物坐标
	POINT GetPoint();
	// 获取怪物与角色的距离
	float GetDistance();
	// 是否死亡
	bool IsDead();
	// 是否可使用必杀技
	bool IsKillStatus();
	// 怪物是否处于攻击状态
	bool IsAttackStatus();

};

