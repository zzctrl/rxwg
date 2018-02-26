#pragma once
#include <vector>

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
// 选npc
void CheckNPC(DWORD dwID);

void OpenNPCTalk(DWORD dwID);


void UseTheF1_F10Call_(DWORD dwIndex);
BOOL UseTheF1_F10Call(DWORD dwPostion);

// 动作，默认为2(普通攻击)
void ActionCall(DWORD dwIndex = 2);

void FindWalk(float x,float y);

// log
void LogA(LPCSTR p_szFormat, ...);

struct PointF
{
	float x;
	float y;
};

bool IsNearPoint(PointF p1, PointF p2);
float CalcPointDist(PointF pt1, PointF pt2);

// 计算开始坐标往目的坐标移动指定距离后的目的坐标点
PointF CalcMoveDestPt(const PointF& a_start, const PointF& a_end, float a_moveDist);

bool IsFilterItem(const CString& a_item, const std::vector<CString>& a_filters);