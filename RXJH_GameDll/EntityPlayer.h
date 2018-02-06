#pragma once
#include "EntityBase.h"
#include "EntityNPC.h"
#include "Package.h"
#include "Config.h"

/*
其他玩家类，包括当前登录用户角色
*/

class EntityPlayer : public EntityBase
{
public:
	EntityPlayer(DWORD a_id = ID_NULL);
	~EntityPlayer();
};

class EntityRole : public EntityBase
{
public:
	// 人物职业:刀，剑，枪，弓，医，刺，琴，韩飞官，谭花灵，拳师，梅柳真，卢风郎
	enum Career{Career_Dao = 1, Career_Jian = 2, Career_Qiang = 3, Career_Gong = 4,\
		Career_Yi = 5, Career_Ci = 0xB, Career_Qin = 0xC, Career_Han = 0xD, \
		Career_Tan = 0xE, Career_Quan = 0x10, Career_Mei = 0x11, Career_Lu = 0x12
	};
	// 动作索引：跑/走，打坐，攻击，拾取物品
	enum Action{Run = 0, Sit, Attack, PickUp};

	// F1-F10索引
	enum ShortCut {
		SC_F1 = 0, FC_F2, FC_F3, FC_F4, FC_F5, SC_F6, FC_F7, FC_F8, FC_F9, FC_F10
	};
public:
	EntityRole(Config& a_config, DWORD a_id = ID_NULL);
	~EntityRole();

	virtual char* GetName();

	// 获取角色的最大攻击距离
	BYTE GetMaxAttackDist();

	// 获取角色当前血/蓝的百分比
	DWORD GetCurrentHP();
	DWORD GetCurrentMP();

	// 获取角色当前坐标
	PointF GetPoint();

	//存取物品
	void SetDepotNPC(const EntityNPC& a_npc);
	bool StoreGoods();
	// 买卖物品，买卖完成后才返回true
	void SetBuyNPC(const EntityNPC& a_npc);
	bool BuySellGoods();

	// 使用物品
	void UseGoods(const CString& a_goodsName);

	// 寻路到指定坐标
	void WalkTo(PointF a_pt);
	// 检测是否在寻路状态
	bool CheckWalkStatus();

	// 使用动作
	void DoAction(Action a_index);

	// 使用快捷键F1-F10
	void UseShortcutF1_F10(ShortCut a_index);

	// 使用技能
	void UseSkill(const CString& a_skillName);

	Package& GetPackage();

private:
	// NPC操作：存取物品，买卖物品
	enum NPCOperType{ Oper_Store, Oper_BuySell };
	// NPC操作状态
	enum NPCOperStatus{BS_OpenTalk= 0, BS_SelOption, BS_Operation, BS_CloseOption, BS_CloseTalk, BS_Finished};

	bool DoNpcOperation(NPCOperType a_operType);
private:
	Config& m_config;

	Package m_package;

	// 寻路状态和目的坐标
	bool m_bWalking;
	PointF m_destPt;

	// 当前要对话的NPC
	EntityNPC m_npc;
	// NPC操作
	NPCOperType m_npcOper;
	NPCOperStatus m_npcOperStatus;
	// 下次动作时间点，需要等待下(因为打开、关闭NPC等需要一定时间)
	DWORD m_nextOperationTime;
};