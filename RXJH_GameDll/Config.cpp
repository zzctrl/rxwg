#include "stdafx.h"
#include "Config.h"


Config::Config()
{
	bPickUp = TRUE;
	bNearestPrior = TRUE;
	bFilterMonster = FALSE;
	bFixedPos = FALSE;
	nProtectHP = 150;
	nProtectMP = 50;
	nAttackType = 0;
	nAttackRange = 150;
	nFilterDist = 0;
	nMinHPDrugs = 3;
	nMinMPDrugs = 3;
	nMinArrows = 3;
	bCheckPackage = FALSE;
	szWorkMap = "柳正关";
	szSupplyMap = "泫勃派";
	szPriorHPDrug = "金创药(小)";
	szPriorMPDrug = "人参";

	hpDrugs.push_back("金创药(小)");
	hpDrugs.push_back("金创药(中)");
	hpDrugs.push_back("金创药(大)");
	hpDrugs.push_back("金创药(特)");
	hpDrugs.push_back("金创药(超)");
	hpDrugs.push_back("秘制金创药");
	hpDrugs.push_back("药仙金创药");
	hpDrugs.push_back("皇级金创药");

	mpDrugs.push_back("人参");
	mpDrugs.push_back("野山参");
	mpDrugs.push_back("雪原参");
	mpDrugs.push_back("益元参");
	mpDrugs.push_back("血参");
	mpDrugs.push_back("幻型人参");
	mpDrugs.push_back("秘制益元参");
	mpDrugs.push_back("药仙益元参");

	armArrows.push_back("雕翎箭");
	armArrows.push_back("奔雷");
	armArrows.push_back("紫电");
	armArrows.push_back("透骨钉");
	armArrows.push_back("狼牙箭");
	armArrows.push_back("破甲锥");

	hcfs.push_back("悟神丹");
	hcfs.push_back("回城符(泫勃派)");
	hcfs.push_back("回城符(三邪关)");
	hcfs.push_back("回城符(柳正关)");
	hcfs.push_back("回城符(柳善府)");
	hcfs.push_back("回城符(神武门)");
	hcfs.push_back("回城符(松月关)");
	hcfs.push_back("回城符(百武关)");
	hcfs.push_back("回城符(九泉)");
	hcfs.push_back("回城符(北海冰宫)");
	hcfs.push_back("回城符(南林)");
	hcfs.push_back("回城符(虎峡谷)");
	hcfs.push_back("回城符(花亭平原)");

	pickFilters.push_back("粽子");
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
	/*m_nEnv = ::GetPrivateProfileIntA(CONFIG_SETTING, CONFIG_ENV, DEF_APPENV, m_configFile.c_str());
	int nValue = ::GetPrivateProfileIntA(CONFIG_SETTING, CONFIG_AUTORUN, 0, m_configFile.c_str());
	m_bAutoRun = nValue != 0;

	nValue = ::GetPrivateProfileIntA(CONFIG_SETTING, CONFIG_SENDTYPE, 1, m_configFile.c_str());
	m_bSendByEnter = nValue != 0;

	nValue = ::GetPrivateProfileIntA(CONFIG_SETTING, CONFIG_FLUSH, 1, m_configFile.c_str());
	m_bFlush = nValue != 0;

	nValue = ::GetPrivateProfileIntA(CONFIG_SETTING, CONFIG_REMIND, 1, m_configFile.c_str());
	m_bRemind = nValue != 0;

	char szTime[64] = { 0 };
	::GetPrivateProfileStringA(CONFIG_SETTING, CONFIG_RECOMMENDTIME, "", szTime, 64, m_configFile.c_str());
	m_updateTime = (time_t)_atoi64(szTime);*/
	return true;
}
bool Config::SaveConfig()
{
	// config
	char szValue[64] = { 0 };
	//sprintf_s(szValue, 64, ("%d"), m_nEnv);
	//::WritePrivateProfileStringA(CONFIG_SETTING, CONFIG_ENV, szValue, m_configFile.c_str());
	/*sprintf_s(szValue, 64, ("%d"), m_bAutoRun ? 1 : 0);
	::WritePrivateProfileStringA(CONFIG_SETTING, CONFIG_AUTORUN, szValue, m_configFile.c_str());
	sprintf_s(szValue, 64, ("%d"), m_bSendByEnter ? 1 : 0);
	::WritePrivateProfileStringA(CONFIG_SETTING, CONFIG_SENDTYPE, szValue, m_configFile.c_str());
	sprintf_s(szValue, 64, ("%d"), m_bFlush ? 1 : 0);
	::WritePrivateProfileStringA(CONFIG_SETTING, CONFIG_FLUSH, szValue, m_configFile.c_str());
	sprintf_s(szValue, 64, ("%d"), m_bRemind ? 1 : 0);
	::WritePrivateProfileStringA(CONFIG_SETTING, CONFIG_REMIND, szValue, m_configFile.c_str());*/

	return true;
}
