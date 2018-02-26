#pragma once
#include "EntityPlayer.h"
#include "Config.h"

/*
挂机辅助类
*/

class PlayHelper
{
public:
	PlayHelper();
	~PlayHelper();

	// 初始化角色ID等属性
	void Initialize();
	// 数据是否还有效
	bool IsDataValid();
	// 清空数据
	void Reset();

	// 开始/暂停挂机
	void Start();
	void Pause();
	// 更新设置
	void UpdateConfig(const Config& a_config);

	// 定时调用，自动选怪/攻击
	void Work();
	// 保护功能，自动加血/蓝
	void Protect();

	// 获取当前坐标
	POINT GetCurPoint();
	CString GetCurMap();
	// 设置挂机坐标点和地图
	void SetWorkPointAndMap(POINT a_pt, const CString& a_mapName);

	// 增加人物攻击距离
	void ModifyAttackDistance(bool a_bModify = true);
	// 穿墙
	void EnableChuanQiang(bool a_bEnable = true);
	// 获取角色名称
	char* GetRoleName();

private:
	// 挂机相关功能
	// 自动选怪，攻击
	void AutoAttack();
	// 移动到仓库NPC处
	void WalkToDepotNPC();
	// 存取物品
	void StoreGoods();
	// 移动到补给NPC处
	void WalkToSupplyNPC();
	// 买卖物品
	void BuySellGoods();
	// 移动到挂机点
	void WalkToWorkPoint();

	// 寻找跨图路径
	std::vector<CString> FindRoadMap(const CString& a_start, const CString& a_dest);
private:
	// 获取当前选中的ID
	DWORD GetCurSelID();

	// 选怪，返回怪物对象ID
	DWORD CheckMonster();

	// 初始化周围怪物的ID范围
	void InitMonterIDRange();

	//  回城检测
	bool CheckNeedBackForSupply();
	void RoadMapForSupply();
private:
	// 无状态，自动选怪攻击状态，到仓库，存取物品，到补给NPC，买卖物品，到挂机点，跨图寻路
	enum WorkStatus{WS_None = 0, WS_Attack, WS_GoToDepotNPC, WS_Store, WS_GoToSupplyNPC, WS_BuySellGoods, WS_GoToWorkPt, WS_RoadMap};

	// 设置
	Config m_config;

	// 当前角色对象
	EntityRole m_role;

	// 挂机攻击的怪物坐标范围
	RECT m_rcRange;
	// 周围怪物的ID范围
	DWORD m_minMonsterID;
	DWORD m_maxMonsterID;
	// 当前角色攻击距离
	DWORD m_attackRange;

	// 当前状态
	WorkStatus m_curStatus;

	// 跨图路径
	std::vector<CString> m_roadMaps;
	int m_roadIndex;

	enum RoadStatus{ RS_CalcRoad = 0, RS_MoveToTransPoint};
	RoadStatus m_roadStatus;
	WorkStatus m_statusAfterRoadMap;
	DWORD m_nextOperTime;
};

