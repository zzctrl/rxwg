#pragma once
#include <vector>
#include "SellFilter.h"

/*
 角色背包类
*/

class Config;
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

	// 使用红/蓝药品
	void UseHPDrug(const std::vector<CString>& a_hpDrugs);
	void UseMPDrug(const std::vector<CString>& a_mpDrugs);
	// 统计背包里所有红/蓝药数量
	DWORD GetAllHPDrugCount(const std::vector<CString>& a_hpDrugs);
	DWORD GetAllMPDrugCount(const std::vector<CString>& a_mpDrugs);
	// 弓箭数量
	DWORD GetArrowCount();

	// 背包是否已满
	bool IsPackageFull();

	// 购买物品名称，数量
	bool BuyGoodsByName(const CString& a_name, DWORD a_count);
	// 自动存，卖物品
	void StoreAndSellGoods(const Config& a_config);

private:
	CString GetGoodsName(DWORD dwData);
	int GetGoodsCount(DWORD dwData);

	int CountGoodsArray(const std::vector<CString>& a_Names);

	DWORD GetShopGoodsID(const CString& a_name);
	// 买卖物品，根据ID
	bool BuyGoodsByID(DWORD a_id, DWORD a_count);
	bool SellGoodsByPosition(BYTE a_postion);
private:
	static const int s_nPackageSize = 36;
	static const int s_nShopSize = 60;

	SellFilter m_filter;
};

