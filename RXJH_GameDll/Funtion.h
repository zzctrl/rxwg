#pragma once



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
DWORD GetEntityNotID();
void CheckEntity();

BOOL UseTheF1_F10Call(DWORD dwPostion);

void FindWalk(float x,float y);

