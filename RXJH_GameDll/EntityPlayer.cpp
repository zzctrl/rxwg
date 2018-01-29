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

}
EntityRole::~EntityRole()
{

}

// 获取角色当前血/蓝的百分比
DWORD EntityRole::GetHPPercent()
{
	return 70;
}
DWORD EntityRole::GetMPPercent()
{
	return 70;
}

// 获取角色当前坐标
POINT EntityRole::GetPoint()
{
	POINT pt = { 0 };
	return pt;
}

// 使用物品
void EntityRole::UseGoods(const CString& a_goodsName)
{

}

// 寻路到指定坐标
void EntityRole::WalkTo(POINT a_pt)
{

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
