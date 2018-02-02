#include "stdafx.h"
#include "PlayHelper.h"
#include "EntifyMonster.h"
#include "Config.h"



PlayHelper::PlayHelper()
{
	m_minMonsterID = 1;
	m_maxMonsterID = 0x2710;
	m_curStatus = WS_None;
}


PlayHelper::~PlayHelper()
{
}

// 开始/停止挂机
void PlayHelper::Start()
{
	m_curStatus = WS_Attack;
	UpdateConfig();
}
void PlayHelper::Stop()
{
	m_curStatus = WS_None;
}

void PlayHelper::UpdateConfig()
{
	Config& cfg = Config::GetConfig();
	// 初始化挂机范围
	m_rcRange.left = cfg.pt.x - cfg.nAttackRange;
	m_rcRange.right = cfg.pt.x + cfg.nAttackRange;
	m_rcRange.top = cfg.pt.y - cfg.nAttackRange;
	m_rcRange.bottom = cfg.pt.y + cfg.nAttackRange;
	// 初始话周围怪物的ID范围(最大，最小值)
	InitMonterIDRange();
}

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

	const Config& cfg = Config::GetConfig();
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
				int nAttackIndex = cfg.nAttackType;
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
		// 没有野怪，则返回挂机点
		m_role.WalkTo(cfg.pt);
	}
}
void PlayHelper::WalkToDepotNPC()
{

}
// 移动到指定NPC处
void PlayHelper::WalkToSupplyNPC()
{
	const Config& cfg = Config::GetConfig();
	MapInfo info;
	if (cfg.GetMapInfoByName(cfg.szSupplyMap, info))
	{
		POINT curPt = m_role.GetPoint();
		POINT& destPt = info.supplyPt;
		// 应该先判断当前是否是补给地图??
		// 判断是否已经到达目的点
		if (curPt.x == destPt.x && curPt.y == destPt.y)
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
	const Config& cfg = Config::GetConfig();
	MapInfo info;
	if (cfg.GetMapInfoByName(cfg.szWorkMap, info))
	{
		POINT curPt = m_role.GetPoint();
		const POINT& destPt = cfg.pt;
		// 判断是否已经到挂机点
		if (curPt.x == destPt.x && curPt.y == destPt.y)
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

POINT PlayHelper::GetCurPoint()
{
	POINT pt = { 0 };
	pt.x = Read_RD(Read_RD(CordinateBaseAddress) + CordinateXOffset);
	pt.y = Read_RD(Read_RD(CordinateBaseAddress) + CordinateYOffset);
	pt.y = 0 - pt.y;
	return pt;
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
}

// 获取当前选中的ID
DWORD PlayHelper::GetCurSelID()
{
	return Read_RD(Read_RD(EntityBaseAddress) + EntitySelOffset);
}

DWORD PlayHelper::CheckMonster()
{
	DWORD dwSelID = EntityBase::ID_NULL;
	float dwDistion = 0.0f;
	EntifyMonster monster;
	Config& cfg = Config::GetConfig();
	bool bNearest = cfg.bNearestPrior;
	for (DWORD dwID = m_minMonsterID; dwID <= m_maxMonsterID; dwID++)
	{
		// 类型为野怪
		if (ET_Monster == EntityBase::GetTypeWithID(dwID))
		{
			monster.SetID(dwID);
			// 未死亡
			if (!monster.IsDead())
			{
				// 怪物是否在挂机范围内
				POINT pt = monster.GetPoint();
				if (::PtInRect(&m_rcRange, pt))
				{
					float dist = monster.GetDistance();
					// 最近优先还是最远优先
					if (bNearest)
					{
						if (dwDistion == 0.0f || dwDistion > dist)
						{
							dwDistion = dist;
							dwSelID = dwID;
						}
					}
					else
					{
						if (dwDistion == 0.0f || dwDistion < dist)
						{
							dwDistion = dist;
							dwSelID = dwID;
						}
					}
				}
			}
		}
	}
	// 选中怪物
	if (EntityBase::ID_NULL != dwSelID)
	{
		monster.SetID(dwSelID);
		monster.Select();
	}

	return dwSelID;
}

void PlayHelper::CheckBackForSupply()
{
	Config& cfg = Config::GetConfig();
	Package& package = m_role.GetPackage();
	bool bNeedSupply = false;
	do
	{
		if (cfg.bCheckHPDrugs)
		{
			if (package.GetAllHPDrugCount() < cfg.nMinHPDrugs)
			{
				bNeedSupply = true;
				break;
			}
		}
		if (cfg.bCheckMPDrugs)
		{
			if (package.GetAllMPDrugCount() < cfg.nMinMPDrugs)
			{
				bNeedSupply = true;
				break;
			}
		}
		if (cfg.bCheckPackage)
		{
			if (package.IsPackageFull())
			{
				bNeedSupply = true;
				break;
			}
		}
		if (cfg.bCheckArrows)
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
		if (cfg.GetMapInfoByName(cfg.szSupplyMap, info))
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
				if (cfg.szSupplyMap == cfg.szWorkMap)
				{
					m_role.WalkTo(info.supplyPt);
				}
				else
				{
					// 找到回补给地图的路径
				}
			}
		}
	}
}
