#pragma once
#include "EntityBase.h"

/*
NPC对象类
*/

class EntityNPC : public EntityBase
{
public:
	EntityNPC(DWORD a_id = ID_NULL);
	EntityNPC(const CString& a_name);
	~EntityNPC();

	// 打开/关闭NPC
	void OpenTalk();
	void CloseTalk();

	// 打开商店
	void OpenShop();
	// 关闭商店
	void CloseShop();

	// 关闭仓库
	void CloseDepot();

	// 购买物品名称，数量
	bool BuyGoodsByName(const CString& a_name, DWORD a_count);

	// 存取物品

private:
	DWORD GetGoodsID(const CString& a_name);
	// 购买物品，根据ID
	bool BuyGoodsByID(DWORD a_id, DWORD a_count);

	// NPC相关操作
	void DoNPCOperation(DWORD a_baseAddr);

	bool IsTalkOpened();
	bool IsShopOpened();

	// 
	int m_nSize;
};

