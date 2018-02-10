#include "stdafx.h"
#include "MapManager.h"


MapManager::MapManager()
{
	InitMaps();
}


MapManager::~MapManager()
{
}


MapManager& MapManager::GetMgr()
{
	static MapManager s_instance;
	return s_instance;
}

const std::vector<MapInfo>& MapManager::GetAllMaps()
{
	return m_maps;
}

bool MapManager::GetMapInfoByName(const CString& a_szName, MapInfo& a_info)
{
	for (int i = 0; i < m_maps.size(); i++)
	{
		if (a_szName == m_maps[i].szName)
		{
			a_info = m_maps[i];
			return true;
		}
	}
	return false;
}

void MapManager::InitMaps()
{
	// 泫勃派
	//MapInfo xbp("泫勃派", "回城符(泫勃派)", "韦大宝", { 155, 1780 }, "平十指", { 742, 1958 });
	MapInfo xbp("泫勃派", "回城符(泫勃派)", "", { 155, 1780 }, "刀剑笑", { 1155, 2227 });
	m_maps.push_back(xbp);

	MapInfo jq("九泉之下1层", "", "", { 0, 0 }, "殷黎亭", { 0, -264 });
	m_maps.push_back(jq);

	MapInfo sxg("三邪关", "回城符(三邪关)", "周叔通",{ 6610, 176 }, "陆小凰",{ 6599, 746 });
	m_maps.push_back(sxg);
	MapInfo lzg("柳正关", "回城符(柳正关)", "红凤凰",{ -6832, 297 }, "萧春水",{ -6639, 203 });
	m_maps.push_back(lzg);
	MapInfo nmh("南明湖", "", "", { 0, 0 }, "", { 0, 0 });
	m_maps.push_back(nmh);
	// 九泉和北海冰宫不能使用其他地图的回城符
	//  北海冰宫，南林 船夫金氏 xbp:649 1071 bhbg:1578 1185 nl:-1063 265

	MapInfo lsf("柳善提督府", "回城符(柳善提督府)", "轩辕二光",{ 1176, 239 }, "丁冬夏",{ 1142, 979 });
	m_maps.push_back(lsf);
	MapInfo swm("神武门", "回城符(神武门)", "平十指", { 742, 1958 }, "陆小凤", { 735, 1856 });
	m_maps.push_back(swm);
}