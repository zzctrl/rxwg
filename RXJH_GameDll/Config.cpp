#include "stdafx.h"
#include "Config.h"


Config::Config()
{
	hpDrugs.push_back("金创药(小)");
	hpDrugs.push_back("金创药(中)");
	hpDrugs.push_back("金创药(大)");
	hpDrugs.push_back("金创药(特)");
	hpDrugs.push_back("金创药(超)");
	hpDrugs.push_back("秘制金创药");
	hpDrugs.push_back("药仙金创药");

	mpDrugs.push_back("人参");
	mpDrugs.push_back("野山参");
	mpDrugs.push_back("雪原参");
	mpDrugs.push_back("益元参");
	mpDrugs.push_back("血参");
	mpDrugs.push_back("幻型人参");
	mpDrugs.push_back("秘制益元参");
	mpDrugs.push_back("药仙益元参");

	maps.push_back({ "泫勃派", "回城符(泫勃派)", "韦大宝",{ 742, 1958 }, "平十指", {742, 1958} });
	// 9泉NPC 殷黎亭 55, 1861
	maps.push_back({ "三邪关", "回城符(三邪关)", "周叔通",{ 6610, 176 }, "陆小凰",{ 6599, 746 } });
	maps.push_back({ "柳善府", "回城符(柳善府)", "平十指",{ 742, 1958 }, "丁春秋",{ 735, 1856 } });
	maps.push_back({ "神武门", "回城符(神武门)", "平十指",{ 742, 1958 }, "陆小凤",{ 735, 1856 } });
}


Config::~Config()
{
}

// 清空数据
void Config::ResetData()
{

}

// 加载/保存配置数据
bool Config::LoadConfig()
{
	return true;
}
bool Config::SaveConfig()
{
	return true;
}

bool Config::GetMapInfoByName(const CString& a_mapName, MapInfo& a_info) const
{
	for (int i = 0; i < maps.size(); i++)
	{
		if (a_mapName == maps[i].szName)
		{
			a_info = maps[i];
			return true;
		}
	}
	return false;
}