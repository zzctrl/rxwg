#include "stdafx.h"
#include "PlayHelper.h"
#include "EntifyMonster.h"
#include "EntityNPC.h"
#include "EntityGoods.h"
#include "Config.h"
#include "MapManager.h"
#include <queue>



PlayHelper::PlayHelper()
:m_role(m_config)
{
	m_minMonsterID = 1;
	m_maxMonsterID = 0x2710;
	m_curStatus = WS_None;
	m_attackRange = 30;
	m_roadIndex = 0;
	m_roadStatus = RS_CalcRoad;
	m_statusAfterRoadMap = WS_GoToWorkPt;
	m_nextOperTime = 0;
}


PlayHelper::~PlayHelper()
{
}

void PlayHelper::Initialize()
{
	// 初始化角色ID
	DWORD dwRoleID = Read_RD(Read_RD(EntityBaseAddress) + 0xC);
	m_role.SetID(dwRoleID);
	// 修改攻击距离
	ModifyAttackDistance();
	// 穿墙
	EnableChuanQiang();
	// 降低内存使用
	::SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
}

bool PlayHelper::IsDataValid()
{
	DWORD dwNation = Read_RD(EntityBaseAddress);
	return dwNation != 0;
}

void ModifyOpCode(DWORD a_addr, BYTE* a_code, DWORD a_len)
{
	DWORD oldProtect = 0;
	// 修改原来的代码
	::VirtualProtect((LPVOID)a_addr, a_len, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((LPVOID)a_addr, a_code, a_len);
	::VirtualProtect((LPVOID)a_addr, a_len, oldProtect, &oldProtect);
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
		ModifyOpCode(dwCodeAddr, pCode, INCODE_LEN);
		// 修改原来的代码
		//::VirtualProtect((LPVOID)dwCodeAddr, INCODE_LEN, PAGE_EXECUTE_READWRITE, &oldProtect);
		//memcpy((LPVOID)dwCodeAddr, pCode, INCODE_LEN);
		//::VirtualProtect((LPVOID)dwCodeAddr, INCODE_LEN, oldProtect, &oldProtect);
	}
}

void PlayHelper::EnableChuanQiang(bool a_bEnable)
{
	// 障碍物判断
	static const DWORD s_dwJudgeLen = 3;
	DWORD dwJudgeAddr = 0x005B4099;
	BYTE oldJudgeCode[s_dwJudgeLen] = { 0x8B, 0x45, 0xD8 };
	BYTE newJudgeCode[s_dwJudgeLen] = { 0x31, 0xC0, 0x90 };
	ModifyOpCode(dwJudgeAddr, a_bEnable ? newJudgeCode : oldJudgeCode, s_dwJudgeLen);

	// 穿墙判断
	static const DWORD s_dwCQLen = 6;
	DWORD dwCQAddr = 0x0052B88C;
	BYTE oldCQCode[s_dwCQLen] = { 0x0F, 0x84, 0xA8, 0x00, 0x00, 0x00 };
	BYTE newCQCode[s_dwCQLen] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	ModifyOpCode(dwCQAddr, a_bEnable ? newCQCode : oldCQCode, s_dwCQLen);
}

char* PlayHelper::GetRoleName()
{
	return m_role.GetName();
}

// 开始/暂停挂机
void PlayHelper::Start()
{
	// 当前是在挂机地图
	CString szCurMap = MapManager::GetMgr().GetCurrentMap();
	if (szCurMap == m_config.szWorkMap)
	{
		PointF curPt = m_role.GetPoint();
		PointF workPt = { m_config.pt.x, m_config.pt .y };
		if (IsNearPoint(curPt, workPt))
		{
			m_curStatus = WS_Attack;
			UpdateConfig(m_config);
			// 初始话周围怪物的ID范围(最大，最小值)
			InitMonterIDRange();
		}
		else
		{
			m_curStatus = WS_GoToWorkPt;
		}
	}
	else
	{
		// 寻路返回挂机点
		m_roadMaps = FindRoadMap(szCurMap, m_config.szWorkMap);
		m_roadIndex = 0;
		m_roadStatus = RS_CalcRoad;
		m_curStatus = WS_RoadMap;
		m_statusAfterRoadMap = WS_GoToWorkPt;
	}
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

	ModifyAttackDistance(false);
}

void PlayHelper::UpdateConfig(const Config& a_config)
{
	m_config = a_config;
	// 初始化挂机范围
	m_rcRange.left = m_config.pt.x - m_config.nAttackRange;
	m_rcRange.right = m_config.pt.x + m_config.nAttackRange;
	m_rcRange.top = m_config.pt.y - m_config.nAttackRange;
	m_rcRange.bottom = m_config.pt.y + m_config.nAttackRange;
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
		if (ET_Goods == EntityBase::GetTypeWithID(dwSelID))
		{
		}
		// 类型不是野怪，则重新选怪(可能用户自己手动点了其他玩家)
		else if (ET_Monster != EntityBase::GetTypeWithID(dwSelID))
		{
			dwSelID = CheckMonster();
		}
	}

	if (EntityBase::ID_NULL != dwSelID)
	{
		// 类型为物品
		if (ET_Goods == EntityBase::GetTypeWithID(dwSelID))
		{
			EntityGoods goods(dwSelID);
			PointF rolePt = m_role.GetPoint();
			if (goods.IsCanPickupNoMove())
			{
				m_role.DoAction(EntityRole::PickUp);
				// 捡物后返回挂机点
				PointF workPt = { m_config.pt.x, m_config.pt.y };
				if (!IsNearPoint(rolePt, workPt))
				{
					m_curStatus = WS_GoToWorkPt;
				}
			}
			else
			{
				PointF pt = goods.GetPoint();
				PointF movePt = CalcMoveDestPt(rolePt, pt, goods.GetNeedMoveDist());
				m_role.WalkTo(movePt);
			}
		}
		// 类型为野怪
		else if (ET_Monster == EntityBase::GetTypeWithID(dwSelID))
		{
			// 判断是否可以使用必杀技
			EntifyMonster monster(dwSelID);
			if (monster.IsKillStatus())
			{
				// 必杀技快捷键
				m_role.UseKillSkill();
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
	MapInfo info;
	MapManager& mapMgr = MapManager::GetMgr();
	if (mapMgr.GetMapInfoByName(m_config.szSupplyMap, info))
	{
		PointF curPt = m_role.GetPoint();
		PointF destPt = { info.depotPt.x, info.depotPt.y };
		// 应该先判断当前是否是补给地图??
		// 判断是否已经到达目的点
		if (IsNearPoint(curPt, destPt))
		{
			// 打开仓库NPC对话，开始存取物品
			EntityNPC npc(info.szDepotNpc);
			if (npc.IsValid())
			{
				m_role.SetDepotNPC(npc);

				m_curStatus = WS_Store;
			}
			else
			{
				// 停止挂机还是。。。？
			}
		}
		else
		{
			m_role.WalkTo(destPt);
		}
	}
}
void PlayHelper::StoreGoods()
{
	bool bFinished = m_role.StoreGoods();
	if (bFinished)
	{
		m_curStatus = WS_GoToSupplyNPC;
	}
}
// 移动到指定NPC处
void PlayHelper::WalkToSupplyNPC()
{
	MapInfo info;
	MapManager& mapMgr = MapManager::GetMgr();
	if (mapMgr.GetMapInfoByName(m_config.szSupplyMap, info))
	{
		PointF curPt = m_role.GetPoint();
		PointF destPt = { info.supplyPt.x, info.supplyPt.y };
		// 应该先判断当前是否是补给地图??
		// 判断是否已经到达目的点
		if (IsNearPoint(curPt, destPt))
		{
			// 打开NPC对话，开始买卖物品
			EntityNPC npc(info.szSupplyNpc);
			if (npc.IsValid())
			{
				m_role.SetBuyNPC(npc);

				m_curStatus = WS_BuySellGoods;
			}
			else
			{
				// 停止挂机还是。。。？
			}
		}
		else
		{
			m_role.WalkTo(destPt);
		}
	}
}
void PlayHelper::BuySellGoods()
{
	bool bFinished = m_role.BuySellGoods();
	if (bFinished)
	{
		// 寻路返回挂机点
		m_roadMaps = FindRoadMap(m_config.szSupplyMap, m_config.szWorkMap);
		m_roadIndex = 0;
		m_roadStatus = RS_CalcRoad;
		m_curStatus = WS_RoadMap;
		m_statusAfterRoadMap = WS_GoToWorkPt;
	}
}
// 移动到挂机点
void PlayHelper::WalkToWorkPoint()
{
	MapInfo info;
	MapManager& mapMgr = MapManager::GetMgr();
	if (mapMgr.GetMapInfoByName(m_config.szWorkMap, info))
	{
		PointF curPt = m_role.GetPoint();
		PointF destPt = { m_config.pt.x, m_config.pt.y };
		// 判断是否已经到挂机点
		if (IsNearPoint(curPt, destPt))
		{
			// 开始自动选怪，攻击
			m_curStatus = WS_Attack;
			// 初始话周围怪物的ID范围(最大，最小值)
			InitMonterIDRange();
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
	DWORD dwTime = ::GetTickCount();
	if (dwTime < m_nextOperTime)
	{
		return;
	}
	try
	{
		switch (m_curStatus)
		{
		case PlayHelper::WS_Attack:
			AutoAttack();
			break;
		case PlayHelper::WS_GoToDepotNPC:
			WalkToDepotNPC();
			break;
		case PlayHelper::WS_Store:
			StoreGoods();
			break;
		case PlayHelper::WS_GoToSupplyNPC:
			WalkToSupplyNPC();
			break;
		case PlayHelper::WS_BuySellGoods:
			BuySellGoods();
			break;
		case PlayHelper::WS_GoToWorkPt:
			WalkToWorkPoint();
			break;
		case PlayHelper::WS_RoadMap:
			RoadMapForSupply();
			break;
		default:
			break;
		}
	}
	catch (...)
	{
		LogA("rxjh: PlayHelper::Work，出现未捕获的异常...");
	}
}
// 保护功能，自动加血/蓝
void PlayHelper::Protect()
{
	try
	{
		Package& package = m_role.GetPackage();

		DWORD dwCurrentHP = m_role.GetCurrentHP();
		if (dwCurrentHP <= m_config.nProtectHP)
		{
			// 吃红药
			int index = package.GetGoodsIndex(m_config.szPriorHPDrug);
			if (index >= 0)
			{
				package.UseGoods(index);
			}
			else
			{
				package.UseHPDrug(m_config.hpDrugs);
			}
		}

		DWORD dwCurrentMP = m_role.GetCurrentMP();
		if (dwCurrentMP <= m_config.nProtectMP)
		{
			// 吃蓝药
			int index = package.GetGoodsIndex(m_config.szPriorMPDrug);
			if (index >= 0)
			{
				package.UseGoods(index);
			}
			else
			{
				package.UseMPDrug(m_config.mpDrugs);
			}
		}
		// 回城检测
		if (WS_Attack == m_curStatus)
		{
			if (CheckNeedBackForSupply())
			{
				// 找出跨图路径
				m_roadMaps = FindRoadMap(m_config.szWorkMap, m_config.szSupplyMap);
				m_roadIndex = 0;
				m_roadStatus = RS_CalcRoad;
				m_curStatus = WS_RoadMap;
				m_statusAfterRoadMap = WS_GoToDepotNPC;

				RoadMapForSupply();
			}
		}
	}
	catch (...)
	{
		LogA("rxjh: PlayHelper::Protect，出现未捕获的异常...");
	}
}

void PlayHelper::SetWorkPointAndMap(POINT a_pt, const CString& a_mapName)
{
	m_config.pt = a_pt;
	m_config.szWorkMap = a_mapName;
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
	return MapManager::GetMgr().GetCurrentMap();
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
	if (m_minMonsterID > 100)
	{
		m_minMonsterID -= 100;
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

bool CmpDistance(bool bNearest, float p1, float p2)
{
	if (bNearest)
	{
		return p1 > p2;
	}
	else
	{
		return p1 < p2;
	}
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
		// 捡物
		if (m_config.bPickUp && ET_Goods == EntityBase::GetTypeWithID(dwID))
		{
			EntityGoods goods(dwID);
			float fdist = goods.GetDistance();
			CString szName = goods.GetName();
			LogA("rxjh: 掉落物品：%s, 距离：%f", szName.GetString(), fdist);
			// 不在过滤列表的才拾取
			if (!IsFilterItem(szName, m_config.pickFilters))
			{
				// 物品在挂机范围内才拾取
				PointF goodsPt = goods.GetPoint();
				POINT pt = { goodsPt.x, goodsPt.y };
				if (::PtInRect(&m_rcRange, pt))
				{
					goods.Select();
					return dwSelID;
				}
			}
		}
		// 类型为野怪
		else if (ET_Monster == EntityBase::GetTypeWithID(dwID))
		{
			monster.SetID(dwID);
			// 未死亡
			if (!monster.IsDead())
			{
				float dist = monster.GetDistance(); 
				if (dist > m_config.nAttackRange)
				{
					continue;
				}
				// 怪物是否在挂机范围内或者距离在当前攻击范围内
				PointF pt = monster.GetPoint();
				POINT intPt = { pt.x, pt.y };
				BOOL bInRange = ::PtInRect(&m_rcRange, intPt);
				if (bInRange || dist < m_attackRange)
				{
					// 最近优先还是最远优先
					if (0 == dwDistion || CmpDistance(bNearest, dwDistion, dist))
					{
						dwDistion = dist;
						dwSelID = dwID;
						monsterPt = pt;
					}
				}
			}
		}
	}
	// 距离是否大于攻击距离
	/*if (bNearest && dwDistion  > m_attackRange)
	{
		float calcDist = CalcPointDist(rolePt, monsterPt);
		float deviation = abs(calcDist - dwDistion);
		// 怪物刚刷新时，坐标还是之前死亡时的坐标，这时就直接选怪，不移动
		if (deviation < 2.0)
		{
			// 往怪物方向走一段距离，下次再选怪攻击
			float dwNeedWalkDist = dwDistion - m_attackRange + 1;
			PointF destPt = CalcMoveDestPt(rolePt, monsterPt, dwNeedWalkDist);
			// 移动
			LogA("rxjh: star move... monsterpt=%f.%f destpt=%f.%f, rolept=%f.%f", monsterPt.x, monsterPt.y, \
				destPt.x, destPt.y, rolePt.x, rolePt.y);
			m_role.WalkTo(destPt);
			return MOVETOPOINT;
		}
	}*/
	// 选中怪物
	if (EntityBase::ID_NULL != dwSelID)
	{
		monster.SetID(dwSelID);
		monster.Select();

		LogA("rxjh: select id=%x, dist=%f", dwSelID, dwDistion);
	}

	return dwSelID;
}

bool PlayHelper::CheckNeedBackForSupply()
{
	Package& package = m_role.GetPackage();
	bool bNeedSupply = false;
	do
	{
		if (m_config.bCheckHPDrugs)
		{
			if (package.GetAllHPDrugCount(m_config.hpDrugs) < m_config.nMinHPDrugs)
			{
				bNeedSupply = true;
				break;
			}
		}
		if (m_config.bCheckMPDrugs)
		{
			if (package.GetAllMPDrugCount(m_config.mpDrugs) < m_config.nMinMPDrugs)
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
			if (package.GetArrowCount() < m_config.nMinArrows)
			{
				bNeedSupply = true;
				break;
			}
		}
	} while (0);

	return bNeedSupply;
}

void PlayHelper::RoadMapForSupply()
{
	if (m_roadMaps.size() < 2)
	{
		return;
	}

	DWORD dwTime = ::GetTickCount();
	MapInfo workMap, supplyMap;
	MapManager& mapMgr = MapManager::GetMgr();
	if (mapMgr.GetMapInfoByName(m_roadMaps[m_roadIndex], workMap) && mapMgr.GetMapInfoByName(m_roadMaps.back(), supplyMap))
	{
		if (RS_CalcRoad != m_roadStatus)
		{
			PointF transferPt = { 0 };
			NearMap nearMap;
			for (auto& item : workMap.newarMaps)
			{
				if (item.szName == m_roadMaps[m_roadIndex+1])
				{
					nearMap = item;
					transferPt.x = nearMap.transferPt.x;
					transferPt.y = nearMap.transferPt.y;
				}
			}
			// 传送到下一个地图
			if (IsNearPoint(transferPt, m_role.GetPoint()))
			{
				// 有传送NPC
				if (!nearMap.szTransferNpc.IsEmpty())
				{
					//EntityNPC npc(nearMap.szTransferNpc);
					//m_role.SetTransferNPC(npc);
					if (m_role.TransferToMap(nearMap.dwTransIndex))
					{
						m_roadIndex++;
						m_roadStatus = RS_CalcRoad;
						m_nextOperTime = dwTime + 2000;
					}
				}
				else
				{
					m_roadIndex++;
					m_roadStatus = RS_CalcRoad;
					m_nextOperTime = dwTime + 2000;
				}
			}
			else
			{
				m_role.WalkTo(transferPt);
			}
		}
		else
		{
			Package& package = m_role.GetPackage();
			// 补给地图跟挂机地图一样
			if (workMap.szName == supplyMap.szName)
			{
				// 判断是否有回城符
				int index = package.GetGoodsIndex(supplyMap.szHCFName);
				if (index >= 0)
				{
					package.UseGoods(index);
				}
				if (WS_GoToWorkPt != m_statusAfterRoadMap)
				{
					m_curStatus = supplyMap.szDepotNpc.IsEmpty() ? WS_GoToSupplyNPC : WS_GoToDepotNPC;
				}
				else
				{
					m_curStatus = WS_GoToWorkPt;
				}
				m_nextOperTime = dwTime + 2000;
			}
			else
			{
				bool bMoveToTransferPoint = false;
				// 挂机地图可以使用其他地图回城符
				if (workMap.bUseOtherHCF)
				{
					// 补给地图可以使用其他地图回城符
					if (supplyMap.bUseOtherHCF)
					{
						// 背包有回城符
						int index = package.GetGoodsIndex(supplyMap.szHCFName);
						if (index >= 0)
						{
							package.UseGoods(index);
							if (WS_GoToWorkPt != m_statusAfterRoadMap)
							{
								m_curStatus = supplyMap.szDepotNpc.IsEmpty() ? WS_GoToSupplyNPC : WS_GoToDepotNPC;
							}
							else
							{
								m_curStatus = WS_GoToWorkPt;
							}
							m_nextOperTime = dwTime + 2000;
						}
						else
						{
							// 按照路径跑路去补给地图
							bMoveToTransferPoint = true;
						}
					}
					else
					{
						// 可以先传送到离目的地图最近的地图，然后再跑路过去
						bool bUseHCF = false;
						for (int i = m_roadMaps.size() - 2; i > m_roadIndex; i--)
						{
							MapInfo info;
							if (mapMgr.GetMapInfoByName(m_roadMaps[i], info))
							{
								if (info.bUseOtherHCF)
								{
									// 背包有回城符
									int index = package.GetGoodsIndex(info.szHCFName);
									if (index >= 0)
									{
										package.UseGoods(index);
										m_roadIndex = i;
										bUseHCF = true;
										m_nextOperTime = dwTime + 2000;
										break;
									}
								}
							}
						}
						// 按照路径跑路去补给地图
						if (!bUseHCF)
						{
							bMoveToTransferPoint = true;
						}
					}
				}
				else
				{
					// 按照路径跑路去补给地图
					bMoveToTransferPoint = true;
				}
				if (bMoveToTransferPoint)
				{
					// 按照路径跑路去补给地图
					m_roadStatus = RS_MoveToTransPoint;
					NearMap nearMap;
					for (auto& item : workMap.newarMaps)
					{
						if (item.szName == m_roadMaps[m_roadIndex + 1])
						{
							EntityNPC npc(item.szTransferNpc);
							m_role.SetTransferNPC(npc);
						}
					}
				}
			}
		}
	}
	else
	{
		LogA("未找到地图信息: %s, %s", m_config.szWorkMap, m_config.szSupplyMap);
	}
}

struct RoadMap
{
	std::vector<CString> roads; // 抵达当前地图的路径
	CString name; // 地图名称
	RoadMap(const CString& a_name)
	{
		name = a_name;
	}
};

std::vector<CString> PlayHelper::FindRoadMap(const CString& a_start, const CString& a_dest)
{
	std::vector<CString> roads;
	if (a_start != a_dest)
	{
		std::queue<RoadMap> roadMaps;
		RoadMap node(a_start);
		roadMaps.push(node);
		MapInfo mapInfo;
		while (!roadMaps.empty())
		{
			RoadMap& head = roadMaps.front();
			if (a_dest == head.name)
			{
				roads = head.roads;
				roads.push_back(a_dest);
			}
			else
			{
				// 
				if (MapManager::GetMgr().GetMapInfoByName(head.name, mapInfo))
				{
					for (auto& nearmap : mapInfo.newarMaps)
					{
						if (head.roads.empty() || nearmap.szName != head.roads.back())
						{
							node.name = nearmap.szName;
							node.roads = head.roads;
							node.roads.push_back(head.name);
							roadMaps.push(node);
						}
					}
				}
			}

			roadMaps.pop();
		}
	}
	else
	{
		roads.push_back(a_start);
		roads.push_back(a_dest);
	}
	return roads;
}
