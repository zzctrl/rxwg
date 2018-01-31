#pragma once
#include <vector>

/*
 角色背包类
*/

class Package
{
public:
	Package();
	~Package();

	// 获取指定名称物品的数量
	DWORD GetGoodsCount(const CString& a_goodsName);
	// 获取物品的索引，如果物品不存在返回-1
	int GetGoodsIndex(const CString& a_goodsName);
	// 使用指定索引的物品
	void UseGoods(DWORD a_index);

	// 使用指定名称的物品
	void UseGoods(const CString& a_goodsName);

	// 统计背包里所有红/蓝药数量
	DWORD GetAllHPDrugCount();
	DWORD GetAllMPDrugCount();

	// 背包是否已满
	bool IsPackageFull();

private:
	CString GetGoodsName(DWORD dwData);
	int GetGoodsCount(DWORD dwData);

	int CountGoodsArray(const std::vector<CString>& a_Names);
private:
	static const int s_nPackageSize = 36;
};

