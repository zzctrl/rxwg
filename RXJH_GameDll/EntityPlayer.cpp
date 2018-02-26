#include "stdafx.h"
#include "EntityPlayer.h"


EntityPlayer::EntityPlayer(DWORD a_id)
:EntityBase(a_id)
{
	
}


EntityPlayer::~EntityPlayer()
{
}




EntityRole::EntityRole(Config& a_config, DWORD a_id)
:EntityBase(a_id)
,m_config(a_config)
{
	m_bWalking = false;
	m_npcOper = Oper_Store;
	m_npcOperStatus = BS_OpenTalk;
	m_nextOperationTime = 0;
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

void EntityRole::SetDepotNPC(const EntityNPC& a_npc)
{
	m_npc = a_npc;
	m_npcOper = Oper_Store;
	m_npcOperStatus = BS_OpenTalk;
	m_nextOperationTime = ::GetTickCount();
}
bool EntityRole::StoreGoods()
{
	return DoNpcOperation(Oper_Store);
}

void EntityRole::SetBuyNPC(const EntityNPC& a_npc)
{
	m_npc = a_npc;
	m_npcOper = Oper_BuySell;
	m_npcOperStatus = BS_OpenTalk;
	m_nextOperationTime = ::GetTickCount();
}

bool EntityRole::BuySellGoods()
{
	return DoNpcOperation(Oper_BuySell);
}

void EntityRole::SetTransferNPC(const EntityNPC& a_npc)
{
	m_npc = a_npc;
	m_npcOper = Oper_Transfer;
	m_npcOperStatus = BS_OpenTalk;
	m_nextOperationTime = ::GetTickCount();
}
bool EntityRole::TransferToMap(DWORD a_transIndex)
{
	return DoNpcOperation(Oper_Transfer, a_transIndex);
}

bool EntityRole::DoNpcOperation(NPCOperType a_operType, DWORD a_optionIndex)
{
	bool bFinished = false;
	DWORD dwCurTime = ::GetTickCount();
	if (dwCurTime < m_nextOperationTime)
	{
		return bFinished;
	}

	static DWORD s_waitTime = 2000;
	switch (m_npcOperStatus)
	{
	case EntityRole::BS_OpenTalk:
	{
		m_npc.OpenTalk();
		m_npcOperStatus = BS_SelOption;
		// 延迟2s再进行打开商店操作
		m_nextOperationTime = dwCurTime + s_waitTime;
	}
	break;
	case EntityRole::BS_SelOption:
	{
		//m_npc.OpenShop();
		m_npc.ChooseOption(a_optionIndex);
		m_npcOperStatus = Oper_Transfer == a_operType ? BS_Finished : BS_Operation;
		m_nextOperationTime = dwCurTime + s_waitTime;
	}
	break;
	case EntityRole::BS_Operation:
	{
		if (Oper_Store == a_operType)
		{
			m_package.StoreAndSellGoods(m_config);
		}
		// 买卖物品
		else if (Oper_BuySell == a_operType)
		{
			// 购买清单列表的物品
			for (auto item : m_config.buys)
			{
				m_package.BuyGoodsByName(item.first, item.second);
				::Sleep(10);
			}
		}
		m_npcOperStatus = BS_CloseOption;
		//m_nextOperationTime = dwCurTime + 2000;
	}
	break;
	case EntityRole::BS_CloseOption:
	{
		if (Oper_Store == a_operType)
		{
			m_npc.CloseDepot();
			m_npcOperStatus = BS_Finished;
		}
		else
		{
			m_npc.CloseShop();
			m_npcOperStatus = BS_CloseTalk;
		}

		m_nextOperationTime = dwCurTime + s_waitTime;
	}
	break;
	case EntityRole::BS_CloseTalk:
	{
		m_npc.CloseTalk();
		m_npcOperStatus = BS_Finished;
		m_nextOperationTime = dwCurTime + s_waitTime;
	}
	break;
	case EntityRole::BS_Finished:
	{
		// 重新设置初始状态，并设置完成标记
		m_npc.SetID(EntityBase::ID_NULL);
		m_npcOperStatus = BS_OpenTalk;
		m_nextOperationTime = 0;
		bFinished = true;
	}
	break;
	default:
		break;
	}
	return bFinished;
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
	// todo: 需要加入防卡处理，如果移动后跟前一个坐标点m_prePt一样，则可以x,y随机加上一个较小的值进行移动

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

static const DWORD s_dwShortcutBaseAddress = 0x02D11D40;
static const DWORD s_dwShortcutOffset = 0x410;
#define TYPE_SKILL		2
#define SKILL_ATTACK	4			
#define SKILL_FZ		5	
// 使用必杀技
void EntityRole::UseKillSkill()
{
	DWORD dwBase = Read_RD(s_dwShortcutBaseAddress);
	for (int i = 0; i < 20; i++)
	{
		DWORD dwNation = Read_RD(dwBase + i * 4 + s_dwShortcutOffset);
		if (0 != dwNation)
		{
			DWORD dwShortcutType = Read_RD(dwNation + 0x48);
			if (TYPE_SKILL == dwShortcutType)
			{
				DWORD dwSkillType = Read_RD(dwNation + 0x230);
				if (SKILL_ATTACK == dwSkillType)
				{
					// 获得技能描述
					char * pDescribe = Read_RS(dwNation + 0xF1);
					if (pDescribe)
					{
						CString szDescribe = pDescribe;
						if (szDescribe.Find("绝命技") > 0)
						{
							ShortCut shortCut = static_cast<ShortCut>(i);
							UseShortcutF1_F10(shortCut);
						}
					}
				}
			}
		}
	}
}
// 使用辅助武功
void EntityRole::UseFZSkill()
{
	DWORD dwBase = Read_RD(s_dwShortcutBaseAddress);
	for (int i = 0; i < 20; i++)
	{
		DWORD dwNation = Read_RD(dwBase + i * 4 + s_dwShortcutOffset);
		if (0 != dwNation)
		{
			DWORD dwShortcutType = Read_RD(dwNation + 0x48);
			if (TYPE_SKILL == dwShortcutType)
			{
				DWORD dwSkillType = Read_RD(dwNation + 0x230);
				if (SKILL_FZ == dwSkillType)
				{
					// 不是轻功
					CString szSkillName = Read_RS(dwNation + 0x5C);
					if (szSkillName != "疾风御气术" && szSkillName != "梯云纵" && szSkillName != "草上飞")
					{
						ShortCut shortCut = static_cast<ShortCut>(i);
						UseShortcutF1_F10(shortCut);
					}
				}
			}
		}
	}
}

// 使用技能
void EntityRole::UseSkill(const CString& a_skillName)
{
	DWORD dwBase = Read_RD(s_dwShortcutBaseAddress);
	for (int i = 0; i < 20; i++)
	{
		DWORD dwNation = Read_RD(dwBase + i * 4 + s_dwShortcutOffset);
		if (0 != dwNation)
		{
			DWORD dwShortcutType = Read_RD(dwNation + 0x48);
			if (TYPE_SKILL == dwShortcutType)
			{
				CString szSkillName = Read_RS(dwNation + 0x5C);
				if (a_skillName == szSkillName)
				{
					ShortCut shortCut = static_cast<ShortCut>(i);
					UseShortcutF1_F10(shortCut);
				}
			}
		}
	}
}
