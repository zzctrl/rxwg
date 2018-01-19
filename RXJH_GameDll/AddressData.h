#pragma once

// 人物属性相关地址
extern DWORD UserBaseAddress; // 人物属性基址
extern DWORD UserNameOffset; // 名称偏移
extern DWORD UserLevelOffset; // 等级偏移
extern DWORD UserHPOffset; // 血量偏移
extern DWORD UserMPOffset; // 蓝量偏移
extern DWORD UserMaxHPOffset; // 血量最大值偏移
extern DWORD UserMaxMPOffset; // 蓝量最大值偏移

extern DWORD CordinateBaseAddress; // 坐标基址
extern DWORD CordinateXOffset; // X坐标偏移
extern DWORD CordinateYOffset; // Y坐标偏移

// 背包相关地址
extern DWORD GoodsBaseAddress;//背包的基址
extern DWORD GoodsBaseOffestAddress;     //背包基本的偏移
extern DWORD GoodsNameOffset; // 物品名称偏移
extern DWORD GoodsNumberOffset; // 物品数量偏移

extern DWORD GoodsUseCallAddress;// 使用背包物品call，参数为物品在背包中的索引下标(0-35)

// 选中的怪物相关地址
extern DWORD EntityBaseAddress;// 选中怪物ID基址
extern DWORD EntitySelOffset; // 选中怪物ID偏移值
// 怪物属性相关
extern DWORD EntityPropAddress;// 怪物属性基址
extern DWORD EntityTypeOffset; // 类型偏移
extern DWORD EntityIdOffset; // ID偏移
extern DWORD EntityNameOffset; // 名称偏移
extern DWORD EntityDistOffset; // 距离偏移
extern DWORD EntityLevelOffset; // 等级偏移
extern DWORD EntityHalfOffset; // 血量偏移
extern DWORD EntityDeadOffset; // 是否死亡偏移
extern DWORD EntityJuemingOffset; // 是否可以施展绝命技的偏移，为1是可以
// 怪物类型 物品：0x33，玩家：0x31, 怪物/NPC：0x2E(NPC等级为0，血量为32000)
extern DWORD EntityType_Monster; // 野怪
extern DWORD EntityType_NPC; // NPC
extern DWORD EntityType_Player; // 其他玩家
extern DWORD EntityType_Goods; // 物品


// 快捷键相关地址
extern DWORD F1_F10_BaseAddress;//快捷键基址
extern DWORD F1_F10_BaseOffestAddress;     //快捷键基本偏移
extern DWORD F1_F10_CallAddress;//快捷键call地址

// 人物动作相关地址
extern DWORD ActionBaseAddress; // 动作基址
extern DWORD ActionCallAddress; // 动作call地址

// 可以从本地配置文件中读取地址信息
void InitAddress();

