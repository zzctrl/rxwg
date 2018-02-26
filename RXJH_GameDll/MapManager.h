#pragma once
#include <vector>

// 邻近地图信息
struct NearMap
{
	CString szName; // 地图名称
	CString szTransferNpc; // 抵达该地图的传送NPC
	POINT transferPt; // 传送NPC坐标或者时抵达该地图的坐标
	DWORD dwTransIndex; // 点开对话后，地图传送选项卡的索引

	NearMap(){}
	NearMap(char* a_name, char* a_transferNpc = "", POINT a_pt = { 0 }, DWORD a_transIndex = 0)
	:szName(a_name), transferPt(a_pt), szTransferNpc(a_transferNpc), dwTransIndex(a_transIndex)
	{

	}
};
// 地图信息
struct MapInfo
{
	CString szName; // 地图名称
	CString szHCFName;// 回城符名称
	CString szDepotNpc; // 仓库NPC名称
	POINT depotPt; // 仓库NPC坐标
	CString szSupplyNpc; // 药品补给NPC名称
	POINT supplyPt; // 药品补给NPC坐标
	CString szArmNpc; // 弓手箭矢补给NPC名称
	POINT armPt; // 弓手箭矢补给NPC坐标
	bool bUseOtherHCF; // 是否可以使用其他地图的回城符
	// 相邻地图信息
	std::vector<NearMap> newarMaps;

	MapInfo(){}
	MapInfo(char* a_name, char* a_hcfName, char* a_depotNpc, POINT a_depotPt, \
		char* a_supplyNpc, POINT a_supplyPt, char* a_armNpc = "", POINT a_armPt = { 0 }, bool a_bOtherHCF = true)
	:szName(a_name), szHCFName(a_hcfName), szDepotNpc(a_depotNpc), depotPt(a_depotPt), \
		szSupplyNpc(a_supplyNpc), supplyPt(a_supplyPt), szArmNpc(a_armNpc), armPt(a_armPt), bUseOtherHCF(a_bOtherHCF)
	{

	}
};


class MapManager
{
public:
	static MapManager& GetMgr();

	const std::vector<MapInfo>& GetAllMaps();

	bool GetMapInfoByName(const CString& a_szName, MapInfo& a_info);

	CString GetCurrentMap();
private:
	MapManager();
	~MapManager();

	void InitMaps();

private:
	std::vector<MapInfo> m_maps;
};

