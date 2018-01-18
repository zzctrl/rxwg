#include "stdafx.h"
#include "Funtion.h"
#include "CommonRead.h"
#include "AddressData.h"


//-------------------------------------------------------------------------------------
char* GetGoodsName(DWORD dwData)
{
	return Read_RS(dwData + 0x5c);
}

DWORD GetGoodsPostion(DWORD dwData)
{
	return Read_RB(dwData + 0x1F4);
}

DWORD GetGoodsIDforName(CString strName)
{
	for (DWORD i=0;i<36;i++)
	{
		DWORD dwNation = Read_RD(Read_RD(GoodsBaseAddress) + i*4 +GoodsBaseOffestAddress);
		if (0x0 == dwNation)
		{
			continue;
		}
		if (GetGoodsName(dwNation) == strName)
		{
			return GetGoodsPostion(dwNation);
		}
	}
	return 0x0FFFF;
}

BOOL UseTheGoodsCall(DWORD dwPostion)
{
	if (0x0FFFF == dwPostion)
	{
		return FALSE;
	}
	_asm
	{
		mov edi,GoodsBaseAddress
		mov edi,[edi]
		MOV EAX,[EDI+0x1608]
		MOV ECX,[EDI+0x1BD0]
		push dwPostion
		push eax
		push ecx
		mov ecx,edi
		mov ebx,GoodsUseCallAddress
		CALL ebx
	}
	return TRUE;
}

//--------------------------------------------------------------------
char* GetEntityName(DWORD dwData)
{
	return Read_RS(dwData + EntityNameOffset);
}

DWORD GetEntityType(DWORD dwData)
{
	return Read_RD(dwData + EntityTypeOffset);
}

DWORD GetEntityId(DWORD dwData)
{
	return Read_RD(dwData + EntityIdOffset);
}

float GetEntityDistance(DWORD dwData)
{
	return Read_RF(dwData + EntityIdOffset);
}

DWORD GetEntityDeath(DWORD dwData)
{
	return Read_RD(dwData + EntityDeadOffset);
}

DWORD GetEntityHP(DWORD dwData)
{
	return Read_RD(dwData + EntityHalfOffset);
}

DWORD GetEntityDengJi(DWORD dwData)
{
	return Read_RD(dwData + EntityLevelOffset);
}

DWORD GetEntityNotID()
{
	return Read_RD(Read_RD(EntityBaseAddress) + EntitySelOffset);
}

void CheckEntity()
{
	if (0x0FFFF == GetEntityNotID())
	{
		DWORD dwTempNation = 0;
		float dwDistion = 0.0f;
		for (DWORD dwID = 0x10; dwID < 0x2710; dwID++)
		{
			DWORD dwNation = Read_RD(EntityPropAddress + dwID * 4);
			if (0x0 == dwNation)
			{
				continue;
			}
			// 类型为野怪，NPC等级为0
			if (EntityType_Monster == GetEntityType(dwNation) && GetEntityDengJi(dwNation) > 0)
			{
				// 未死亡
				if (0x0 == GetEntityDeath(dwNation))
				{
					if (dwDistion == 0.0f || dwDistion > GetEntityDistance(dwNation))
					{
						dwDistion = GetEntityDistance(dwNation);
						dwTempNation = dwNation;
					}
				}
			}
		}
		if (dwTempNation != 0)
		{
			_asm
			{
				MOV ESI, dwTempNation
				MOV EAX, [ESI]
				MOV EDX, [EAX + 0x4]
				PUSH 0x1
				PUSH 0x450
				MOV ECX, ESI
				CALL EDX
				MOV EAX, [ESI + 0x0C]
				MOV ECX, EntityBaseAddress
				mov ecx, [ecx]
				mov[ecx + EntitySelOffset], eax
			}
		}
	}
}

//-----------------------------------------------------------------------
BOOL UseTheF1_F10Call(DWORD dwPostion)
{
	DWORD dwNation = Read_RD(Read_RD(F1_F10_BaseAddress) + dwPostion*4 + F1_F10_BaseOffestAddress);
	if (0x0 == dwNation)
	{
		return FALSE;
	}
	_asm
	{
		push dwPostion
		mov ebx,F1_F10_CallAddress
		call ebx
	}
	return TRUE;
}
//---------------------------------------------------------------------------
void FindWalk(float x,float y)
{
	BYTE* pDate = new BYTE[84];
	memset(pDate,0,84);

	_asm
	{
		mov ebx,pDate
		mov eax,x
		mov [ebx],eax
		mov eax,y
		mov [ebx+8],eax
		mov [ebx+0x14],0xFFFF
		mov [ebx+0x18],1
		mov BYTE PTR DS:[ebx+0x1C],1

		push 0x54
		push pDate
		push 0x3EF
		mov ecx,EntityBaseAddress
		mov ecx,[ecx]
		mov edx,[ecx]
		mov edx,[edx+4]
		call edx

	}
	delete pDate;
	pDate = nullptr;
}
//----------------------------------------------------------------------------
