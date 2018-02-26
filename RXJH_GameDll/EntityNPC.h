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

	// 打开对话后，选择指定索引的选项卡
	void ChooseOption(DWORD a_index);

	// 关闭仓库
	void CloseDepot();

	
private:

	// NPC相关操作
	void DoNPCOperation(DWORD a_baseAddr);

	bool IsTalkOpened();
	bool IsShopOpened();

	// 
	int m_nSize;
};

