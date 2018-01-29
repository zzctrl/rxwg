#include "stdafx.h"
#include "Package.h"


Package::Package()
{
}


Package::~Package()
{
}

// 获取物品的索引，如果物品不存在返回-1
int Package::GetGoodsIndex(const CString& a_goodsName)
{
	return -1;
}
// 使用指定索引的物品
void Package::UseGoods(DWORD a_index)
{

}

// 获取指定名称物品的数量
DWORD Package::GetGoodsCount(const CString& a_goodsName)
{
	return 0;
}
// 使用指定名称的物品
void Package::UseGoods(const CString& a_goodsName)
{

}

// 统计背包里所有红/蓝药数量
DWORD Package::GetAllHPDrugCount()
{
	return 0;
}
DWORD Package::GetAllMPDrugCount()
{
	return 0;
}
