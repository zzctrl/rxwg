#include "stdafx.h"
#include "EntifyMonster.h"


EntifyMonster::EntifyMonster(DWORD a_id)
:EntityBase(a_id)
{
}


EntifyMonster::~EntifyMonster()
{
}

PointF EntifyMonster::GetPoint()
{
	float fx = Read_RF(m_nation + EntityCordinateXOffset);
	float fy = Read_RF(m_nation + EntityCordinateYOffset);
	PointF pt = { fx, fy };
	return pt;
}

// 获取怪物与角色的距离
float EntifyMonster::GetDistance()
{
	float dist = Read_RF(m_nation + EntityDistOffset);
	return dist;
}
// 是否死亡
bool EntifyMonster::IsDead()
{
	DWORD dwDead = Read_RD(m_nation + EntityDeadOffset);
	return dwDead != 0;
}
// 是否可使用必杀技
bool EntifyMonster::IsKillStatus()
{
	DWORD status = Read_RD(m_nation + EntityKillOffset);
	return status != 0;
}
// 怪物是否处于攻击状态
bool EntifyMonster::IsAttackStatus()
{
	return false;
}
