#pragma once
#include "EntityPlayer.h"

/*
挂机辅助类
*/

class PlayHelper
{
public:
	PlayHelper();
	~PlayHelper();

	// 开始/停止挂机
	void Start();
	void Stop();
	// 更新设置
	void UpdateConfig();

	// 定时调用，自动选怪/攻击
	void Work();
	// 保护功能，自动加血/蓝
	void Protect();

	// 获取当前坐标
	POINT GetCurPoint();
	CString GetCurMap();

private:
	// 挂机相关功能
	// 自动选怪，攻击
	void AutoAttack();
	// 移动到仓库NPC处
	void WalkToDepotNPC();
	// 移动到补给NPC处
	void WalkToSupplyNPC();
	// 移动到挂机点
	void WalkToWorkPoint();

private:
	// 获取当前选中的ID
	DWORD GetCurSelID();

	// 选怪，返回怪物对象ID
	DWORD CheckMonster();

	// 初始化周围怪物的ID范围
	void InitMonterIDRange();

	//  回城检测
	void CheckBackForSupply();
private:
	// 无状态，自动选怪攻击状态，寻路到补给NPC，寻路到挂机点
	enum WorkStatus{WS_None = 0, WS_Attack, WS_GoToDepotNPC, WS_GoToSupplyNPC, WS_GoToWorkPt};

	// 当前角色对象
	EntityRole m_role;

	// 挂机攻击的怪物坐标范围
	RECT m_rcRange;
	// 周围怪物的ID范围
	DWORD m_minMonsterID;
	DWORD m_maxMonsterID;

	// 当前状态
	WorkStatus m_curStatus;
};

