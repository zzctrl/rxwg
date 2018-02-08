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
		if (a_goodsName == GetGoodsName(dwNation))
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
	DWORD dwBaseAddress = 0x02D105D8;
	DWORD dwNation = Read_RD(Read_RD(dwBaseAddress) + 0x440);
	DWORD dwCount = Read_RD(dwNation + 0xC44);
	return dwCount;
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