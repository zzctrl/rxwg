#include "stdafx.h"
#include "EntityGoods.h"


EntityGoods::EntityGoods(DWORD a_id)
:EntityBase(a_id)
{
}


EntityGoods::~EntityGoods()
{
}

char* EntityGoods::GetName()
{
	if (0 == m_nation)
	{
		return "";
	}
	char* pName = Read_RS(m_nation + 0x94);
	return pName;
}

PointF EntityGoods::GetPoint()
{
	PointF pt = { 0 };
	pt.x = Read_RF(m_nation + 0x48);
	pt.y = Read_RF(m_nation + 0x50);
	return pt;
}

static const float s_fMaxPickpuDist = 40.0;

bool EntityGoods::IsCanPickupNoMove()
{
	float dist = GetDistance();
	
	return dist < s_fMaxPickpuDist;
}
float EntityGoods::GetNeedMoveDist()
{
	float dist = GetDistance();
	return dist - s_fMaxPickpuDist + 1;
}
// 获取物品的距离
float EntityGoods::GetDistance()
{
	if (0 == m_nation)
	{
		return 9999.0;
	}
	float fDist = Read_RF(m_nation + 0x64);
	return fDist;
}
