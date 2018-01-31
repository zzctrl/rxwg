#include "stdafx.h"
#include "EntityPlayer.h"


EntityPlayer::EntityPlayer(DWORD a_id)
:EntityBase(a_id)
{
}


EntityPlayer::~EntityPlayer()
{
}




EntityRole::EntityRole(DWORD a_id)
:EntityBase(a_id)
{
	m_bWalking = false;
}
EntityRole::~EntityRole()
{

}

Package& EntityRole::GetPackage()
{
	return m_package;
}

// 获取角色当前血/蓝的百分比
DWORD EntityRole::GetCurrentHP()
{
	DWORD dwHP = Read_RD(UserBaseAddress + UserHPOffset);
	return dwHP;
}
DWORD EntityRole::GetCurrentMP()
{
	DWORD dwMP = Read_RD(UserBaseAddress + UserMPOffset);
	return dwMP;
}

// 获取角色当前坐标
POINT EntityRole::GetPoint()
{
	POINT pt = { 0 };
	pt.x = Read_RD(Read_RD(CordinateBaseAddress) + CordinateXOffset);
	pt.y = Read_RD(Read_RD(CordinateBaseAddress) + CordinateYOffset);
	pt.y = 0 - pt.y;
	return pt;
}

// 使用物品
void EntityRole::UseGoods(const CString& a_goodsName)
{
	m_package.UseGoods(a_goodsName);
}

bool EntityRole::CheckWalkStatus()
{
	if (m_bWalking)
	{
		POINT pt = GetPoint();
		if (pt.x == m_destPt.x && pt.y == m_destPt.y)
		{
			m_bWalking = false;
		}
	}
	return m_bWalking;
}
// 寻路到指定坐标
void EntityRole::WalkTo(POINT a_pt)
{
	m_bWalking = true;
	m_destPt = a_pt;

	float x = a_pt.x;
	float y = a_pt.y;
	BYTE* pDate = new BYTE[84];
	memset(pDate, 0, 84);

	_asm
	{
		mov ebx, pDate
		mov eax, x
		mov[ebx], eax
		mov eax, y
		mov[ebx + 8], eax
		mov[ebx + 0x18], 0xFFFF
		mov[ebx + 0x1C], 1
		mov BYTE PTR DS : [ebx + 0x20], 1

		push 0x54
		push pDate
		push 0x3EF
		mov ecx, EntityBaseAddress
		mov ecx, [ecx]
		mov edx, [ecx]
		mov edx, [edx + 4]
		call edx

	}
	delete pDate;
	pDate = nullptr;
}

// 使用动作
void EntityRole::DoAction(Action a_index)
{
	int index = a_index;
	DWORD dwNation = Read_RD(Read_RD(ActionBaseAddress) + index * 4 + 0x410);
	if (0 != dwNation)
	{
		_asm
		{
			mov eax, dwNation
			mov ecx, [eax + 0x4C]
			mov edx, 0x010C8CB8
			mov edx, [edx]
			push ecx
			mov ecx, [edx + 0x27C]
			call ActionCallAddress
		}
	}
}

// 使用快捷键F1-F10
void EntityRole::UseShortcutF1_F10(ShortCut a_index)
{
	int index = a_index;
	DWORD dwNation = Read_RD(F1_F10_BaseAddress);
	if (0x0 != dwNation)
	{
		_asm
		{
			push index
			mov ecx, dwNation
			mov ebx, F1_F10_CallAddress
			call ebx
		}
	}
}

// 使用技能
void EntityRole::UseSkill(const CString& a_skillName)
{

}
