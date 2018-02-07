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
