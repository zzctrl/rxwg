#include "stdafx.h"
#include "Package.h"
#include "CommonRead.h"
#include "AddressData.h"



Package::Package()
{
	m_HPdrugs.push_back("金创药(小)");
	m_HPdrugs.push_back("金创药(中)");
	m_HPdrugs.push_back("金创药(大)");
	m_HPdrugs.push_back("金创药(特)");
	m_HPdrugs.push_back("金创药(超)");
	m_HPdrugs.push_back("秘制金创药");
	m_HPdrugs.push_back("药仙金创药");

	m_MPdrugs.push_back("人参");
	m_MPdrugs.push_back("野山参");
	m_MPdrugs.push_back("雪原参");
	m_MPdrugs.push_back("益元参");
	m_MPdrugs.push_back("血参");
	m_MPdrugs.push_back("幻型人参");
	m_MPdrugs.push_back("秘制益元参");
	m_MPdrugs.push_back("药仙益元参");
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
	return Read_RD(dwData + GoodsNameOffset);
}
// 获取指定名称物品的数量
DWORD Package::GetGoodsCount(const CString& a_goodsName)
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
			return GetGoodsCount(dwNation);
		}
	}
	return 0;
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

// 统计背包里所有红/蓝药数量
DWORD Package::GetAllHPDrugCount()
{
	int count = CountGoodsArray(m_HPdrugs);
	return count;
}
DWORD Package::GetAllMPDrugCount()
{
	int count = CountGoodsArray(m_MPdrugs);
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