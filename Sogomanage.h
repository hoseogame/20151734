#pragma once

#include"base.h"
#include"Class.h"



//----------------
//----2.변수 선언
class C_So_Manage {
private:
	char g_EnemyType[4][3] = { "♨", "◆", "☎", "▣" };
	clock_t g_StartTime;
	int g_nEnemyIndex; // 적 캐릭터 출현 인덱스
	int g_nEnemyCount; // 적 캐릭터 수
	clock_t g_nLimitTime;
	GAME_STATE		g_nGameState;
	int				g_nStage;
	int				g_nGrade; // 점수 
	int				g_nTotalGrade; // 총점
	clock_t			g_GameStartTime, g_UpdateOldTime, g_RemainTime;  // ready를 일정시간 지속해 주기 위해,전 업데이트 한 시간으로 main에서 한번 값 설정.
	int g_nPatCount;  //패턴의 개수
	int g_nDeadEnemy;
	int g_nBossPatCount;   //보스 패턴의 개수

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


