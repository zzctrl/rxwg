#include "stdafx.h"
#include "MapManager.h"
#include "CommonRead.h"


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

CString MapManager::GetCurrentMap()
{
	static const DWORD s_dwMapIndexAddress = 0x010c8cb8;
	static const DWORD s_dwMapPropAddress = 0x010F61B4;
	DWORD dwIndex = Read_RD(Read_RD(Read_RD(s_dwMapIndexAddress) + 0x2A0) + 0x248);
	char* pMapName = Read_RS(s_dwMapPropAddress + dwIndex * 0x2E8);
	return pMapName ? pMapName : "";
}

void MapManager::InitMaps()
{
	// 泫勃派
	//MapInfo xbp("泫勃派", "回城符(泫勃派)", "韦大宝", { 155, 1780 }, "平十指", { 742, 1958 });
	MapInfo xbp("泫勃派", "回城符(泫勃派)", "韦大宝", { 155, 1780 }, "平十指", { 742, 1958 }, "刀剑笑", { 1155, 2227 });
	xbp.newarMaps.push_back(NearMap("三邪关", "", {2440, -338}));
	xbp.newarMaps.push_back(NearMap("柳正关", "", {-2352, -460}));
	xbp.newarMaps.push_back(NearMap("九泉", "殷黎亭", {60, 1866}));
	xbp.newarMaps.push_back(NearMap("南明湖", "", { -15, -2350 }));
	xbp.newarMaps.push_back(NearMap("北海冰宫", "船夫金氏", { 649, 1071 }));
	xbp.newarMaps.push_back(NearMap("伏魔洞", "船夫金氏", { 649, 1071 }, 1));
	xbp.newarMaps.push_back(NearMap("南林", "船夫金氏", { 649, 1071 }, 2));
	xbp.newarMaps.push_back(NearMap("虎峡谷", "船夫金氏", { 649, 1071 }, 3));
	xbp.newarMaps.push_back(NearMap("花亭平原", "船夫老徐", { 574, 1032 }));
	m_maps.push_back(xbp);

	// 九泉之下需要另外处理下
	MapInfo jq("九泉之下1层", "回城符(九泉)", "", { 0, 0 }, "殷黎亭", { 0, -264 }, "", {0}, false);
	jq.newarMaps.push_back(NearMap("泫勃派", "殷黎亭", {0, -264}, 1));
	jq.newarMaps.push_back(NearMap("九泉之下100层", "张大成 长子", { -2, 81}, 1));
	m_maps.push_back(jq);

	MapInfo sxg("三邪关", "回城符(三邪关)", "周叔通",{ 6610, 176 }, "陆小凰",{ 6599, 746 });
	sxg.newarMaps.push_back(NearMap("柳善提督府", "", { 7350, 1235 }));
	sxg.newarMaps.push_back(NearMap("泫勃派", "", { 2685, -369 }));
	m_maps.push_back(sxg);

	MapInfo lzg("柳正关", "回城符(柳正关)", "红凤凰",{ -6832, 297 }, "萧春水",{ -6639, 203 });
	lzg.newarMaps.push_back(NearMap("神武门", "", { -7432, 920 }));
	lzg.newarMaps.push_back(NearMap("泫勃派", "", { -2688, -292 }));
	m_maps.push_back(lzg);

	MapInfo nmh("南明湖", "", "", { 0, 0 }, "", { 0, 0 });
	nmh.newarMaps.push_back(NearMap("泫勃派", "", { -104, 2418 }));
	nmh.newarMaps.push_back(NearMap("南明洞", "南明洞护法", { 1346, 1156 }));
	m_maps.push_back(nmh);
	// 九泉和北海冰宫不能使用其他地图的回城符
	//  北海冰宫，南林 船夫金氏 xbp:649 1071 bhbg:1578 1185 nl:-1063 265

	MapInfo lsf("柳善提督府", "回城符(柳善府)", "轩辕二光",{ 1176, 239 }, "丁冬夏",{ 1142, 979 }, "萧十二郎", { 780, 922 });
	lsf.newarMaps.push_back(NearMap("三邪关", "", { -2285, -525}));
	lsf.newarMaps.push_back(NearMap("松月关", "", { 1346, 1156 }));
	lsf.newarMaps.push_back(NearMap("血魔洞", "南宫剑", { 0, 0 }));
	m_maps.push_back(lsf);

	MapInfo swm("神武门", "回城符(神武门)", "平十指", { 742, 1958 }, "陆小凤", { 735, 1856 });
	swm.newarMaps.push_back(NearMap("柳正关", "", { 2195, 393}));
	swm.newarMaps.push_back(NearMap("百武关", "南明洞护法", { 0, 0 }));
	swm.newarMaps.push_back(NearMap("地灵洞", "南宫剑", { 0, 0 }));
	m_maps.push_back(swm);

	MapInfo bhbg("北海冰宫", "回城符(北海冰宫)", "金十龄", { 1792, -333 }, "吕孤城", { 2054, -308}, "司空揽月", { 1563, -844}, false);
	bhbg.newarMaps.push_back(NearMap("泫勃派", "船夫金氏", { 1578, 1185 }));
	m_maps.push_back(bhbg);

	MapInfo nl("南林", "回城符(南林)", "黄浩虎", { 181, 464}, "艺花兰", { 55, 615}, "吴霸武", { 86, -19 }, false);
	nl.newarMaps.push_back(NearMap("泫勃派", "船夫金氏", { -1063, 265}));
	m_maps.push_back(nl);
}