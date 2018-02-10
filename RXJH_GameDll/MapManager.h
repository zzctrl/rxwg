#pragma once
#include <vector>

// 邻近地图信息
struct NearMap
{
	CString szName; // 地图名称
	CString szHCFName; // 回城符名称
	bool bCanUseHCF; // 是否可以使用回城符
	CString szTransferNpc; // 传送NPC
	DWORD dwTransIndex; // 点开对话后，地图传送选项卡的索引
	POINT transferPt; // 传送坐标

	NearMap(){}
	NearMap(char* a_name, char* a_hcfName, bool a_bUseHCF, POINT a_pt, char* a_transferNpc = "", DWORD a_transIndex = 0)
	:szName(a_name), szHCFName(a_hcfName), bCanUseHCF(a_bUseHCF), transferPt(a_pt), szTransferNpc(a_transferNpc), dwTransIndex(a_transIndex)
	{

	}
	void SetTransferPt(POINT pt) { transferPt = pt; }
};
// 地图信息
struct MapInfo
{
	CString szName; // 地图名称
	CString szHCFName;// 回城符名称
	CString szDepotNpc; // 仓库NPC名称
	POINT depotPt; // 仓库NPC坐标
	CString szSupplyNpc; // 补给NPC名称
	POINT supplyPt; // 补给NPC坐标
	// 相邻地图信息
	std::vector<NearMap> newarMaps;

	MapInfo(){}
	MapInfo(char* a_name, char* a_hcfName, char* a_depotNpc, POINT a_depotPt, char* a_supplyNpc, POINT a_supplyPt)
	:szName(a_name), szHCFName(a_hcfName), szDepotNpc(a_depotNpc), depotPt(a_depotPt), szSupplyNpc(a_supplyNpc), supplyPt(a_supplyPt)
	{

	}
};


class MapManager
{
public:
	static MapManager& GetMgr();

	const std::vector<MapInfo>& GetAllMaps();

	bool GetMapInfoByName(const CString& a_szName, MapInfo& a_info);

private:
	MapManager();
	~MapManager();

	void InitMaps();

private:
	std::vector<MapInfo> m_maps;
};

