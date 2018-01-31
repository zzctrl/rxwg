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

	// 定时调用，自动选怪/攻击
	void Work();
	// 保护功能，自动加血/蓝
	void Protect();

	// 获取当前坐标
	POINT GetCurPoint();
	CString GetCurMap();

	// 设置挂机点和范围
	void SetWorkPoint(POINT a_pt);
	void SetWorkRange(DWORD a_range);

	void SetAttackIndex(int a_index);
	void SetNearestPrior(bool a_nearest);

	// 设置HP/MP保护值，低于这个值则自动加血/蓝
	void SetProtectHP(DWORD a_protectHP);
	void SetProtectMP(DWORD a_protectMP);

	void SetPriorHPDrug(const CString& a_drugName);
	void SetPriorMPDrug(const CString& a_drugName);

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
	// 当前角色对象
	EntityRole m_role;

	// HP/MP保护设置，百分比
	DWORD m_protectHP;
	DWORD m_protectMP;
	// 挂机点和范围
	POINT m_workPt;
	DWORD m_workRange;
	// 攻击方式索引
	DWORD m_attackIndex;
	// 挂机范围
	RECT m_rcRange;
	// 周围怪物的ID范围
	DWORD m_minMonsterID;
	DWORD m_maxMonsterID;
	// 优先选最近的怪,false表示优先选挂机范围内最远的怪
	bool m_bNearest;
	// 优先使用的红和蓝药
	CString m_szPriorHPDrug;
	CString m_szPriorMPDrug;
};

