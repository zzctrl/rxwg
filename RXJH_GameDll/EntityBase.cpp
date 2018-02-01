#include "stdafx.h"
#include "EntityBase.h"



EntityBase::EntityBase()
{
	Reset();
}
EntityBase::EntityBase(DWORD a_id)
{
	if (ID_NULL == a_id)
	{
		Reset();
	}
	else
	{
		SetID(a_id);
	}
}

EntityBase::~EntityBase()
{
}

void EntityBase::Reset()
{
	m_id = ID_NULL;
	m_type = ET_NULL;
	m_nation = 0;
}

EntityType EntityBase::GetTypeWithID(DWORD a_id)
{
	EntityType eType = ET_NULL;
	DWORD dwNation = Read_RD(EntityPropAddress + a_id * 4);
	if (0x0 != dwNation)
	{
		DWORD dwType = Read_RD(dwNation + EntityTypeOffset);
		if (EntityType_Monster == dwType)
		{
			// 等级大于0的为野怪，为0是NPC
			DWORD dwLevel = Read_RD(dwNation + EntityLevelOffset);
			eType = dwLevel > 0 ? ET_Monster : ET_NPC;
		}
		else if (EntityType_Player == dwType)
		{
			eType = ET_Player;
		}
		else if (EntityType_Goods == dwType)
		{
			eType = ET_Goods;
		}
	}
	
	return eType;
}

void EntityBase::SetID(DWORD a_id)
{
	m_id = a_id;
	m_type = GetTypeWithID(m_id);
	m_nation = Read_RD(EntityPropAddress + m_id * 4);
}

DWORD EntityBase::GetID()
{
	return m_id;
}

EntityType EntityBase::GetType()
{
	return m_type;
}

// 获取对象地址
DWORD EntityBase::GetNation()
{
	return m_nation;
}

void EntityBase::Select()
{
	DWORD dwNation = GetNation();
	if (0 != dwNation)
	{
		_asm
		{
			MOV ESI, dwNation
			MOV EAX, [ESI]
			MOV EDX, [EAX + 0x4]
			PUSH 0x1
			PUSH 0x450
			MOV ECX, ESI
			CALL EDX
			MOV EAX, [ESI + 0x0C]
			MOV ECX, EntityBaseAddress
			mov ecx, [ecx]
			mov dword ptr [ecx + 0x1a3c], eax
		}
	}
}

void EntityBase::UnSelect()
{

}

int EntityBase::GetLevel()
{
	DWORD dwLevel = Read_RD(m_nation + EntityLevelOffset);
	return dwLevel;
}

CString EntityBase::GetName()
{
	char* pName = Read_RS(m_nation + EntityNameOffset);
	return pName;
}