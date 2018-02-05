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

char* EntityRole::GetName()
{
	char* pName = Read_RS(m_nation + 0x18);
	return pName;
}

BYTE EntityRole::GetMaxAttackDist()
{
	DWORD dwCareer = Read_RD(UserBaseAddress + UserCareerOffset);
	switch (dwCareer)
	{
	case Career_Dao:
	case Career_Jian:
	case Career_Qiang:
	case Career_Ci:
	case Career_Han:
	case Career_Tan:
	case Career_Quan:
	case Career_Lu:
		return 38;
	case Career_Yi:
	case Career_Qin:
	case Career_Mei:
		return 90;
	case Career_Gong:
		return 228;
	default:
		break;
	}
	return 0;
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
PointF EntityRole::GetPoint()
{
	PointF pt = { 0.0f };
	pt.x = Read_RF(m_nation + 0x1C40);
	pt.y = Read_RF(m_nation + 0x1C48);
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
		PointF pt = GetPoint();
		if (IsNearPoint(pt, m_destPt))
		{
			m_bWalking = false;
		}
	}
	return m_bWalking;
}
// 寻路到指定坐标
void EntityRole::WalkTo(PointF a_pt)
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
	DWORD CallBaseAddress = 0x010C8CB8;
	DWORD dwNation = Read_RD(Read_RD(CallBaseAddress) + 0x27C);
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
