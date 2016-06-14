#pragma once

#include"base.h"
#include"Class.h"



//----------------
//----2.���� ����
class C_So_Manage {
private:
	char g_EnemyType[4][3] = { "��", "��", "��", "��" };
	clock_t g_StartTime;
	int g_nEnemyIndex; // �� ĳ���� ���� �ε���
	int g_nEnemyCount; // �� ĳ���� ��
	clock_t g_nLimitTime;
	GAME_STATE		g_nGameState;
	int				g_nStage;
	int				g_nGrade; // ���� 
	int				g_nTotalGrade; // ����
	clock_t			g_GameStartTime, g_UpdateOldTime, g_RemainTime;  // ready�� �����ð� ������ �ֱ� ����,�� ������Ʈ �� �ð����� main���� �ѹ� �� ����.
	int g_nPatCount;  //������ ����
	int g_nDeadEnemy;
	int g_nBossPatCount;   //���� ������ ����

public:
	C_So_Manage();
	virtual int Init();
	virtual int Run();
	void PlayerCollision();
	int Collision(int nX1, int nY1, int nX2, int nY2, int nX3, int nY3, int nX4, int nY4);
	void Update();
	void Render();
	void BossMissileCollsion();
	void PlayerMissileBossCollision();
	void Release();
};


