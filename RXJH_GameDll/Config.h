#pragma once
#include <map>
#include <vector>



class Config
{
public:
	Config();
	~Config();
	// 清空数据
	void ResetData();

	// 加载/保存配置数据
	bool LoadConfig();
	bool SaveConfig();

	// 保护设置
	int nProtectHP;
	int nProtentMP;
	CString szPriorHPDrug;
	CString szPriorMPDrug;

	// 挂机设置
	POINT pt;
	CString szWorkMap;
	int nAttackType;
	int nAttackRange;
	BOOL bNearestPrior;
	BOOL bPickUp;

	// 回城设置
	BOOL bCheckHPDrugs;
	int nMinHPDrugs;
	BOOL bCheckMPDrugs;
	int nMinMPDrugs;
	BOOL bCheckArrows;
	int nMinArrows;
	BOOL bCheckPackage;
	CString szSupplyMap;

	// 买卖设置: 物品名称，数量
	std::map<CString, DWORD> buys;


	std::vector<CString> hpDrugs;
	std::vector<CString> mpDrugs;
};

