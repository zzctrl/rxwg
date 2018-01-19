#pragma once

#define ENTITY_NOTSEL_ID		0xFFFF

DWORD GetGoodsIDforName(CString strName);
char* GetGoodsName(DWORD dwData);
DWORD GetGoodsPostion(DWORD dwData);
BOOL  UseTheGoodsCall(DWORD dwPostion);

char* GetEntityName(DWORD dwData);
DWORD GetEntityType(DWORD dwData);
DWORD GetEntityId(DWORD dwData);
float GetEntityDistance(DWORD dwData);
DWORD GetEntityDeath(DWORD dwData);
DWORD GetEntityHP(DWORD dwData);
DWORD GetEntityDengJi(DWORD dwData);
DWORD GetEntityJueming(DWORD dwData);

// 获取当前选中怪物ID
DWORD GetEntitySelID();
// 选怪
DWORD CheckEntity();


void UseTheF1_F10Call_(DWORD dwIndex);
BOOL UseTheF1_F10Call(DWORD dwPostion);

// 动作，默认为2(普通攻击)
void ActionCall(DWORD dwIndex = 2);

void FindWalk(float x,float y);

