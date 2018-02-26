#pragma once
#include "EntityBase.h"

/*
物品对象类
*/

class EntityGoods : public EntityBase
{
public:
	EntityGoods(DWORD a_id = ID_NULL);
	~EntityGoods();

	// 获取对象名称
	virtual char* GetName();
	// 获取坐标
	virtual PointF GetPoint();

	// 是否可以不移动就拾取物品
	bool IsCanPickupNoMove();
	// 拾取物品需要移动的距离
	float GetNeedMoveDist();

	// 获取物品的距离
	float GetDistance();
};

