#pragma once
#include <map>
#include <vector>


struct MapInfo
{
	CString szName;
	CString szSupplyNpc;
	POINT supplyPt;
};

class Config
{
public:
	static Config& GetConfig();

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

	std::vector<MapInfo> maps;
private:
	Config();
	~Config();
};

