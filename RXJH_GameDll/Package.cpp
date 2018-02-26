#include "stdafx.h"
#include "Package.h"
#include "CommonRead.h"
#include "AddressData.h"
#include "Config.h"
#include "Funtion.h"



Package::Package()
{
	
}


Package::~Package()
{
}

CString Package::GetGoodsName(DWORD dwData)
{
	return Read_RS(dwData + GoodsNameOffset);
}

int Package::GetGoodsCount(DWORD dwData)
{
	return Read_RD(dwData + GoodsNumberOffset);
}
// 获取指定名称物品的数量
DWORD Package::GetGoodsCount(const CString& a_goodsName)
{
	DWORD dwCount = 0;
	for (int i = 0; i < s_nPackageSize; i++)
	{
		DWORD dwNation = Read_RD(Read_RD(GoodsBaseAddress) + i * 4 + GoodsBaseOffestAddress);
		if (0x0 == dwNation)
		{
			continue;
		}
		if (a_goodsName == GetGoodsName(dwNation))
		{
			dwCount = GetGoodsCount(dwNation);
			break;
		}
	}
	return dwCount;
}
// 获取物品的索引，如果物品不存在返回-1
int Package::GetGoodsIndex(const CString& a_goodsName)
{
	for (int i = 0; i < s_nPackageSize; i++)
	{
		DWORD dwNation = Read_RD(Read_RD(GoodsBaseAddress) + i * 4 + GoodsBaseOffestAddress);
		if (0x0 == dwNation)
		{
			continue;
		}
		CString szName = GetGoodsName(dwNation);
		if (szName.Find(a_goodsName) >= 0)
		{
			return i;
		}
	}
	return -1;
}
// 使用指定索引的物品
void Package::UseGoods(DWORD a_index)
{
	DWORD index = a_index;
	// edx是物品在背包中的索引下标
	_asm
	{
		mov edx, index
		push edx
		push 0x1
		push 0x0
		mov edx, GoodsBaseAddress
		mov ecx, [edx]
		mov ebx, GoodsUseCallAddress
		call ebx
	}
}

// 使用指定名称的物品
void Package::UseGoods(const CString& a_goodsName)
{
	int index = GetGoodsIndex(a_goodsName);
	if (index >= 0)
	{
		UseGoods(index);
	}
}

bool Package::IsPackageFull()
{
	for (int i = 0; i < s_nPackageSize; i++)
	{
		DWORD dwNation = Read_RD(Read_RD(GoodsBaseAddress) + i * 4 + GoodsBaseOffestAddress);
		if (0x0 == dwNation)
		{
			return false;
		}
	}
	return true;
}

DWORD Package::GetArrowCount()
{
	DWORD dwCount = 0;
	DWORD dwBaseAddress = 0x02D105D8;
	DWORD dwNation = Read_RD(Read_RD(dwBaseAddress) + 0x440);
	if (0 != dwNation)
	{
		dwCount = Read_RD(dwNation + 0xC44);
	}

	return dwCount;
}

void Package::UseHPDrug(const std::vector<CString>& a_hpDrugs)
{
	for (int i = 0; i < a_hpDrugs.size(); i++)
	{
		int index = GetGoodsIndex(a_hpDrugs[i]);
		if (index >= 0)
		{
			UseGoods(index);
		}
	}
}
void Package::UseMPDrug(const std::vector<CString>& a_mpDrugs)
{
	for (int i = 0; i < a_mpDrugs.size(); i++)
	{
		int index = GetGoodsIndex(a_mpDrugs[i]);
		if (index >= 0)
		{
			UseGoods(index);
		}
	}
}

// 统计背包里所有红/蓝药数量
DWORD Package::GetAllHPDrugCount(const std::vector<CString>& a_hpDrugs)
{
	int count = CountGoodsArray(a_hpDrugs);
	return count;
}
DWORD Package::GetAllMPDrugCount(const std::vector<CString>& a_mpDrugs)
{
	int count = CountGoodsArray(a_mpDrugs);
	return count;
}

int Package::CountGoodsArray(const std::vector<CString>& a_Names)
{
	int count = 0;
	for (int i = 0; i < a_Names.size(); i++)
	{
		count += GetGoodsCount(a_Names[i]);
	}
	return count;
}

bool Package::BuyGoodsByName(const CString& a_name, DWORD a_count)
{
	bool bRet = false;
	if (a_count > 0)
	{
		DWORD dwGoodsId = GetShopGoodsID(a_name);
		if (dwGoodsId > 0)
		{
			// 购买物品
			bRet = BuyGoodsByID(dwGoodsId, a_count);
		}
	}

	return bRet;
}

// 自动存，卖物品
void Package::StoreAndSellGoods(const Config& a_config)
{
	for (BYTE i = 0; i < s_nPackageSize; i++)
	{
		DWORD dwNation = Read_RD(Read_RD(GoodsBaseAddress) + i * 4 + GoodsBaseOffestAddress);
		if (0x0 == dwNation)
		{
			continue;
		}
		CString szName = Read_RS(dwNation + 0x5C);
		// 判断是否是装备，装备和奇遇石等级>1，其他物品为1
		DWORD dwLevel = Read_RD(dwNation + 0xAC);
		if (dwLevel > 1)
		{
			// 装备合成，强化属性
			DWORD dwQHLevel = Read_RD(dwNation + 0xD24);
			char * pHCProp = Read_RS(dwNation + 0x430);
			if (szName.Find("奇玉石") >= 0)
			{
				// 初级奇玉石只保留 觉醒2，体5，中级奇玉石都保留
				if (60 == dwLevel)
				{
					if (dwQHLevel != 22 && dwQHLevel != 10)
					{
						SellGoodsByPosition(i);
					}
				}
			}
			// 未合成强化过的才处理
			else if (0 == dwQHLevel && 0 == strlen(pHCProp))
			{
				// 存仓库或者卖
				SellGoodsByPosition(i);
			}
		}
		// 根据过滤列表判断物品是该存还是卖
		else
		{
			bool bSell = false;
			// 金刚石，寒玉石
			if (szName.Find("金刚石") >= 0 || szName.Find("寒玉石") >= 0)
			{
				// 石头类型为武功，攻击力，全部气功等级，追加伤害，防御力，武功防御力才保留
				DWORD dwType = Read_RD(dwNation + 0xD1C);
				DWORD dwValue = Read_RD(dwNation + 0xD24);
				bSell = m_filter.IsSellStone(szName, dwType, dwValue);
			}
			else if (szName.Find("热血石") >= 0)
			{
				// 只保留 货币10%，物品升级1%，回柳身法，真武绝击  9:物品升级1%，12:货币10%，
				DWORD dwType = Read_RD(dwNation + 0xD1C);
				if (dwType != 9 && dwType != 12)
				{
					bSell = true;
				}
			}
			else if (szName.Find("集魂珠") >= 0 || szName.Find("玛瑙石") >= 0)
			{
				bSell = true;
			}
			else
			{
				// 药，回城符，强化石不卖，其他都卖商店
			}
			if (bSell)
			{
				SellGoodsByPosition(i);
			}
		}
	}
}

bool Package::BuyGoodsByID(DWORD a_id, DWORD a_count)
{
	BYTE* pDate = new BYTE[142];
	memset(pDate, 0, 142);
	_asm
	{
		mov eax, pDate
		mov dword ptr[eax + 2], 0x00880092
		mov dword ptr[eax + 6], 0x1
		mov dword ptr[eax + 0x0A], 0
		mov ebx, a_id
		mov dword ptr[eax + 0x0E], ebx
		mov dword ptr[eax + 0x12], 0
		mov ebx, a_count
		mov dword ptr[eax + 0x16], ebx
		mov ecx, dword ptr ds : [0x1097760]
		push 0x8E
		push eax
		mov edx, ShopCallAddress
		call edx
	}
	delete pDate;
	pDate = nullptr;
	return true;
}

bool Package::SellGoodsByPosition(BYTE a_postion)
{
	BYTE* pDate = new BYTE[142];
	memset(pDate, 0, 142);
	DWORD dwNation = Read_RD(Read_RD(GoodsBaseAddress) + a_postion * 4 + GoodsBaseOffestAddress);
	DWORD dwID = Read_RD(dwNation + 0x4C);
	DWORD dwCount = Read_RD(dwNation + 0xC44);
	memcpy(pDate + 0x26, (BYTE*)dwNation + 0x54, 8);
	_asm
	{
		mov eax, pDate
		mov dword ptr[eax + 2], 0x00880092
		mov dword ptr[eax + 6], 0x2
		mov dword ptr[eax + 0x0A], 0
		mov ebx, dwID
		mov dword ptr[eax + 0x0E], ebx
		mov dword ptr[eax + 0x2E], ebx
		mov dword ptr[eax + 0x12], 0
		mov ebx, dwCount
		mov dword ptr[eax + 0x16], ebx
		mov dword ptr[eax + 0x36], ebx
		mov byte ptr[eax + 0x3E], 1
		mov bl, a_postion
		mov byte ptr[eax + 0x3F], bl
		mov ecx, dword ptr ds : [0x1097760]
		push 0x8E
		push eax
		mov edx, ShopCallAddress
		call edx
	}
	delete pDate;
	pDate = nullptr;
	return true;
}

DWORD Package::GetShopGoodsID(const CString& a_name)
{
	DWORD dwID = 0;
	for (int i = 0; i < s_nShopSize; i++)
	{
		DWORD dwNation = Read_RD(Read_RD(ShopBaseAddress) + i * 4 + ShopBaseOffset);
		if (0 == dwNation)
		{
			break;
		}
		CString szName = Read_RS(dwNation + ShopItemNameOffset);
		if (a_name == szName)
		{
			dwID = Read_RD(dwNation + ShopItemIDOffset);
			break;
		}
	}
	return dwID;
}