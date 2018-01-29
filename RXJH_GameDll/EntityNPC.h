#pragma once
#include "EntityBase.h"

/*
NPC对象类
*/

class EntityNPC : public EntityBase
{
public:
	EntityNPC(DWORD a_id = ID_NULL);
	~EntityNPC();
};

