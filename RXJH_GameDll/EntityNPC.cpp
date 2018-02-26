#include "stdafx.h"
#include "EntityNPC.h"


EntityNPC::EntityNPC(DWORD a_id)
:EntityBase(a_id)
{
	m_nSize = 60;
}

EntityNPC::EntityNPC(const CString& a_name)
	: EntityBase(EntityBase::ID_NULL)
{
	m_nSize = 60;
	// 根据NPC名称获取ID
	for (DWORD dwID = 1; dwID < 0x2710; dwID++)
	{
		// 类型为NPC
		if (ET_NPC == EntityBase::GetTypeWithID(dwID))
		{
			DWORD dwNation = Read_RD(EntityPropAddress + dwID * 4);
			CString szName = Read_RS(dwNation + EntityNameOffset);
			if (a_name == szName)
			{
				SetID(dwID);
				break;
			}
		}
	}
}


EntityNPC::~EntityNPC()
{
}

bool EntityNPC::IsTalkOpened()
{
	DWORD dwNPCBaseAddr = 0x02D37C7C; // 02D36518 + 5D9 * 4
	DWORD dwOpen = Read_RD((Read_RD(dwNPCBaseAddr) + 0x338) + 0x40);
	return dwOpen != 0;
}
bool EntityNPC::IsShopOpened()
{
	DWORD dwNPCBaseAddr = 0x02D37C7C; // 02D36518 + 5D9 * 4
	DWORD dwOpen = Read_RD((Read_RD(dwNPCBaseAddr) + 0x348) + 0x40);
	return dwOpen != 0;
}

void EntityNPC::OpenTalk()
{
	// 先选中NPC
	Select();

	// 打开对话选项卡
	DWORD dwNation = m_nation;
	_asm
	{
		mov ecx, dwNation
		mov edx, [ecx]
		mov eax, [edx + 4]
		push 0
		push 0
		push 0x401
		call eax
	}
	// 等待选项卡打开才返回
	/*bool bOpen = IsTalkOpened();
	int tryCount = 10;
	while (!bOpen && tryCount > 0)
	{
		::Sleep(50);
		bOpen = IsTalkOpened();
		tryCount--;
	}
	::Sleep(1000);*/
}
void EntityNPC::CloseTalk()
{
	DWORD NPCCloseTalkBaseAddress = 0x0109CF50;
	DoNPCOperation(NPCCloseTalkBaseAddress);
}

void EntityNPC::ChooseOption(DWORD a_index)
{
	DWORD NPCChooseBaseAddress = 0x0109CF5C + a_index * 4;
	DoNPCOperation(NPCChooseBaseAddress);
}

void EntityNPC::OpenShop()
{
	DWORD NPCCloseTalkBaseAddress = 0x0109CF5C;
	DoNPCOperation(NPCCloseTalkBaseAddress);
	// 等待窗口打开才返回
	/*bool bOpen = IsShopOpened();
	int tryCount = 10;
	while (!bOpen && tryCount > 0)
	{
		::Sleep(50);
		bOpen = IsShopOpened();
		tryCount--;
	}
	::Sleep(1000);*/
}

void EntityNPC::CloseShop()
{
	DWORD NPCCloseTalkBaseAddress = 0x0109CFB8;
	DoNPCOperation(NPCCloseTalkBaseAddress);
}

void EntityNPC::CloseDepot()
{
	DWORD NPCCloseTalkBaseAddress = 0x0109D080;
	DoNPCOperation(NPCCloseTalkBaseAddress);
}

void EntityNPC::DoNPCOperation(DWORD a_baseAddr)
{
	DWORD dwBaseAddress = a_baseAddr;
	_asm
	{
		mov eax, dwBaseAddress
		mov eax, [eax]
		add eax, 4
		mov esi, [eax]
		mov ecx, [esi + 4]
		mov eax, [esi + 0x254]
		mov edx, [ecx]
		mov edx, [edx + 4]
		push 0
		push eax
		push 0x3F4
		call edx
	}
}
