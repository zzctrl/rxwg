﻿#include "stdafx.h"
#include "PlayHelper.h"
#include "EntifyMonster.h"
#include "Config.h"



PlayHelper::PlayHelper()
{
	m_minMonsterID = 1;
	m_maxMonsterID = 0x2710;
	m_curStatus = WS_None;
	m_attackRange = 0;
}


PlayHelper::~PlayHelper()
{
}

void PlayHelper::Initialize()
{
	// 初始化角色ID
	DWORD dwRoleID = Read_RD(Read_RD(EntityBaseAddress) + 0xC);
	m_role.SetID(dwRoleID);
}

bool PlayHelper::IsDataValid()
{
	DWORD dwNation = Read_RD(EntityBaseAddress);
	return dwNation != 0;
}



#define INCODE_LEN		9
void PlayHelper::ModifyAttackDistance(bool a_bModify)
{
	BYTE dwAttackDist = m_role.GetMaxAttackDist();
	m_attackRange = dwAttackDist;
	if (dwAttackDist > 0)
	{
		DWORD dwCodeAddr = 0x00506509;
		BYTE oldCode[INCODE_LEN] = { 0x75, 0x07, 0xC7, 0x45, 0x0C, 0x05, 0x00, 0x00, 0x00 };
		BYTE newCode[INCODE_LEN] = { 0x75, 0x00, 0xC7, 0x45, 0x0C, dwAttackDist, 0x00, 0x00, 0x00 };
		DWORD oldProtect = 0;
		BYTE* pCode = a_bModify ? newCode : oldCode;
		// 修改原来的代码
		::VirtualProtect((LPVOID)dwCodeAddr, INCODE_LEN, PAGE_EXECUTE_READWRITE, &oldProtect);
		memcpy((LPVOID)dwCodeAddr, pCode, INCODE_LEN);
		::VirtualProtect((LPVOID)dwCodeAddr, INCODE_LEN, oldProtect, &oldProtect);
	}
}

char* PlayHelper::GetRoleName()
{
	return m_role.GetName();
}

// 开始/暂停挂机
void PlayHelper::Start()
{
	m_curStatus = WS_Attack;
	UpdateConfig(m_config);
}

void PlayHelper::Pause()
{
	m_curStatus = WS_None;
}

void PlayHelper::Reset()
{
	m_curStatus = WS_None;
	m_role.SetID(EntityBase::ID_NULL);
	m_config.ResetData();
}

void PlayHelper::UpdateConfig(const Config& a_config)
{
	m_config = a_config;
	// 初始化挂机范围
	m_rcRange.left = m_config.pt.x - m_config.nAttackRange;
	m_rcRange.right = m_config.pt.x + m_config.nAttackRange;
	m_rcRange.top = m_config.pt.y - m_config.nAttackRange;
	m_rcRange.bottom = m_config.pt.y + m_config.nAttackRange;
	// 初始话周围怪物的ID范围(最大，最小值)
	InitMonterIDRange();
}

#define MOVETOPOINT		0xFFFE

// 自动选怪，攻击
void PlayHelper::AutoAttack()
{
	DWORD dwSelID = GetCurSelID();
	if (EntityBase::ID_NULL == dwSelID)
	{
		dwSelID = CheckMonster();
	}
	else
	{
		// 类型不是野怪，则重新选怪(可能用户自己手动点了其他玩家或者物品之类的)
		if (ET_Monster != EntityBase::GetTypeWithID(dwSelID))
		{
			dwSelID = CheckMonster();
		}
	}

	if (EntityBase::ID_NULL != dwSelID)
	{
		// 类型为野怪
		if (ET_Monster == EntityBase::GetTypeWithID(dwSelID))
		{
			// 判断是否可以使用必杀技
			EntifyMonster monster(dwSelID);
			if (monster.IsKillStatus())
			{
				// 必杀技快捷键
				m_role.UseShortcutF1_F10(EntityRole::FC_F8);
			}
			else
			{
				int nAttackIndex = m_config.nAttackType;
				// 普攻或技能
				if (0 == nAttackIndex)
				{
					m_role.DoAction(EntityRole::Attack);
				}
				else
				{
					EntityRole::ShortCut sc = static_cast<EntityRole::ShortCut>(nAttackIndex - 1);
					m_role.UseShortcutF1_F10(sc);
				}
			}
		}
	}
	else
	{
		if (MOVETOPOINT != dwSelID)
		{
			// 没有野怪，则返回挂机点
			PointF pt = { m_config.pt.x, m_config.pt.y };
			m_role.WalkTo(pt);
		}
	}
}
void PlayHelper::WalkToDepotNPC()
{

}
// 移动到指定NPC处
void PlayHelper::WalkToSupplyNPC()
{
	MapInfo info;
	if (m_config.GetMapInfoByName(m_config.szSupplyMap, info))
	{
		PointF curPt = m_role.GetPoint();
		PointF destPt = { info.supplyPt.x, info.supplyPt.y };
		// 应该先判断当前是否是补给地图??
		// 判断是否已经到达目的点
		if (IsNearPoint(curPt, destPt))
		{
			// 购买清单上的物品

			// 返回挂机点
			m_curStatus = WS_GoToWorkPt;
		}
		else
		{
			m_role.WalkTo(destPt);
		}
	}
}
// 移动到挂机点
void PlayHelper::WalkToWorkPoint()
{
	MapInfo info;
	if (m_config.GetMapInfoByName(m_config.szWorkMap, info))
	{
		PointF curPt = m_role.GetPoint();
		PointF destPt = { m_config.pt.x, m_config.pt.y };
		// 判断是否已经到挂机点
		if (IsNearPoint(curPt, destPt))
		{
			// 开始自动选怪，攻击
			m_curStatus = WS_Attack;
		}
		else
		{
			m_role.WalkTo(destPt);
		}
	}
}

// 定时调用，自动选怪/攻击
void PlayHelper::Work()
{
	switch (m_curStatus)
	{
	case PlayHelper::WS_Attack:
		AutoAttack();
		break;
	case PlayHelper::WS_GoToDepotNPC:
		WalkToDepotNPC();
		break;
	case PlayHelper::WS_GoToSupplyNPC:
		WalkToSupplyNPC();
		break;
	case PlayHelper::WS_GoToWorkPt:
		WalkToWorkPoint();
		break;
	default:
		break;
	}
}
// 保护功能，自动加血/蓝
void PlayHelper::Protect()
{
	Config& cfg = Config::GetConfig();
	DWORD dwCurrentHP= m_role.GetCurrentHP();
	if (dwCurrentHP <= cfg.nProtectHP)
	{
		// 吃红药
		m_role.UseShortcutF1_F10(EntityRole::FC_F2);
	}

	DWORD dwCurrentMP = m_role.GetCurrentMP();
	if (dwCurrentMP <= cfg.nProtentMP)
	{
		// 吃蓝药
		m_role.UseShortcutF1_F10(EntityRole::FC_F3);
	}
	// 回城检测
	if (WS_Attack == m_curStatus)
	{
		CheckBackForSupply();
	}
}

void PlayHelper::SetWorkPoint(POINT a_pt)
{
	m_config.pt = a_pt;
	UpdateConfig(m_config);
}

POINT PlayHelper::GetCurPoint()
{
	PointF pt = m_role.GetPoint();
	POINT ret = { pt.x, pt.y };
	return ret;
}

CString PlayHelper::GetCurMap()
{
	char* pMapName = Read_RS(Read_RD(MapBaseAddress) + MapNameOffset);
	return pMapName ? pMapName : "";
}

void PlayHelper::InitMonterIDRange()
{
	m_minMonsterID = 0x2710;
	m_maxMonsterID = 0;
	for (DWORD dwID = 0x1; dwID < 0x2710; dwID++)
	{
		// 类型为野怪
		if (ET_Monster == EntityBase::GetTypeWithID(dwID))
		{
			if (m_minMonsterID > dwID)
			{
				m_minMonsterID = dwID;
			}
			if (m_maxMonsterID < dwID)
			{
				m_maxMonsterID = dwID;
			}
		}
	}
	// 适当增加ID检测范围
	m_minMonsterID -= 100;
	if (m_minMonsterID < 1)
	{
		m_minMonsterID = 1;
	}
	m_maxMonsterID += 100;
	if (m_maxMonsterID > 0x2700)
	{
		m_maxMonsterID = 0x2700;
	}
}

// 获取当前选中的ID
DWORD PlayHelper::GetCurSelID()
{
	return Read_RD(Read_RD(EntityBaseAddress) + EntitySelOffset);
}

float CalcPointDist(PointF pt1, PointF pt2)
{
	int x = pt1.x - pt2.x;
	int y = pt1.y - pt2.y;
	double dist = x*x + y*y;
	float ret = sqrt(dist);
	return ret;
}

DWORD PlayHelper::CheckMonster()
{
	DWORD dwSelID = EntityBase::ID_NULL;
	float dwDistion = 0.0f;
	EntifyMonster monster;

	PointF rolePt = m_role.GetPoint();
	PointF monsterPt = { 0.0f };
	bool bNearest = m_config.bNearestPrior;
	for (DWORD dwID = m_minMonsterID; dwID <= m_maxMonsterID; dwID++)
	{
		// 类型为野怪
		if (ET_Monster == EntityBase::GetTypeWithID(dwID))
		{
			monster.SetID(dwID);
			// 未死亡
			if (!monster.IsDead())
			{
				// 怪物是否在挂机范围内或者距离在当前攻击范围内
				PointF pt = monster.GetPoint();
				POINT intPt = { pt.x, pt.y };
				BOOL bInRange = ::PtInRect(&m_rcRange, intPt);

				float dist = monster.GetDistance(); //距离有的不对，用自己算的
				//float dist = CalcPointDist(pt, rolePt);
				LogA("rxjh: in=%d, dist=%f, attack=%d, monsterpt=%f.%f, roltpt=%f.%f", bInRange, dist, (int)m_attackRange, \
				pt.x, pt.y, rolePt.x, rolePt.y);
				if (bInRange || dist < m_attackRange)
				{
					// 最近优先还是最远优先
					if (bNearest)
					{
						if (0 == dwDistion || dwDistion > dist)
						{
							dwDistion = dist;
							dwSelID = dwID;
							monsterPt = pt;
						}
					}
					else
					{
						if (0 == dwDistion || dwDistion < dist)
						{
							dwDistion = dist;
							dwSelID = dwID;
							monsterPt = pt;
						}
					}
				}
			}
		}
	}
	// 距离是否大于攻击距离
	LogA("rxjh: dist=%f, attack=%d", dwDistion, (int)m_attackRange);
	if (bNearest && dwDistion  > m_attackRange)
	{
		// 往怪物方向走一段距离，再选怪攻击
		PointF destPt = rolePt;
		float dwNeedWalkDist = dwDistion - m_attackRange + 1;
		if (monsterPt.x == rolePt.x)
		{
			if (monsterPt.y > rolePt.y)
			{
				destPt.y += dwNeedWalkDist;
			}
			else
			{
				destPt.y -= dwNeedWalkDist;
			}
		}
		else
		{
			dwNeedWalkDist = dwNeedWalkDist * dwNeedWalkDist;
			float k = (float)(monsterPt.y - rolePt.y) / (monsterPt.x - rolePt.x);
			k = abs(k);
			float temp = 1 + k*k;
			float ret = (float)dwNeedWalkDist / temp;
			float x = sqrt(ret);
			float y = x * k;
			if (monsterPt.x > rolePt.x)
			{
				destPt.x = (float)rolePt.x + x;
			}
			else
			{
				destPt.x = (float)rolePt.x - x;
			}
			if (monsterPt.y > rolePt.y)
			{
				destPt.y = (float)rolePt.y + y;
			}
			else
			{
				destPt.y = (float)rolePt.y - y;
			}
		}
		// 移动
		LogA("rxjh: star move... monsterpt=%f.%f destpt=%f.%f, rolept=%f.%f", monsterPt.x, monsterPt.y, \
			destPt.x, destPt.y, rolePt.x, rolePt.y);
		m_role.WalkTo(destPt);
		return MOVETOPOINT;
	}
	// 选中怪物
	if (EntityBase::ID_NULL != dwSelID)
	{
		monster.SetID(dwSelID);
		monster.Select();

		float di = monster.GetDistance();
		LogA("rxjh: select dist=%f, mondist=%f, attack=%d", dwDistion, di, (int)m_attackRange);
	}

	return dwSelID;
}

void PlayHelper::CheckBackForSupply()
{
	Package& package = m_role.GetPackage();
	bool bNeedSupply = false;
	do
	{
		if (m_config.bCheckHPDrugs)
		{
			if (package.GetAllHPDrugCount() < m_config.nMinHPDrugs)
			{
				bNeedSupply = true;
				break;
			}
		}
		if (m_config.bCheckMPDrugs)
		{
			if (package.GetAllMPDrugCount() < m_config.nMinMPDrugs)
			{
				bNeedSupply = true;
				break;
			}
		}
		if (m_config.bCheckPackage)
		{
			if (package.IsPackageFull())
			{
				bNeedSupply = true;
				break;
			}
		}
		if (m_config.bCheckArrows)
		{
			/*if (1)
			{
				bNeedSupply = true;
				break;
			}*/
		}
	} while (0);
	
	// 需要回城补给
	if (bNeedSupply)
	{
		MapInfo info;
		if (m_config.GetMapInfoByName(m_config.szSupplyMap, info))
		{
			m_curStatus = WS_GoToSupplyNPC;
			// 判断是否有回城符
			int index = package.GetGoodsIndex(info.szHCFName);
			if (index >= 0)
			{
				package.UseGoods(index);
			}
			else
			{
				// 如果补给地图跟挂机地图一样，则跑路回去
				if (m_config.szSupplyMap == m_config.szWorkMap)
				{
					PointF pt = { info.supplyPt.x, info.supplyPt.y };
					m_role.WalkTo(pt);
				}
				else
				{
					// 找到回补给地图的路径
				}
			}
		}
	}
}
