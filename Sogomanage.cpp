#include"Sogomanage.h"


//구조체 선언
STAGE_INFO g_StageInfo[] = { { 10, 12 },{ 10, 20 },{ 15, 30 } };

//----1.객체 선언
C_MainObject g_Player;         //플레이어
C_MainObject g_PlayerMissile[HERO_MISSILE];  //플레이어 미사일을 배열로
C_PatInfo *g_PatInfo;                        //적 패턴의 정보
C_PatInfo *g_BossPatInfo;                   //보스의 패턴의 정보
C_Boss g_Boss;                              //보스
C_Enemy g_Enemy[50];                        //적
C_BossMissileInfo g_sBossMissile;           //보스미사일
char *g_strFileName[] = { "pat1.txt", "pat2.txt", "pat3.txt", "pat4.txt" };
char *g_strBossFileName[] = { "boss_pat1.txt", "boss_pat2.txt" };
C_MainObject  g_EnemyMissile[100];   // 미사일 버퍼

C_So_Manage::C_So_Manage()
{
	g_nGameState = INIT;
	g_nStage = -1;
	g_nGrade = 0; // 점수 
	g_nTotalGrade = 0; // 총점
}

int C_So_Manage::Init()
{
	ScreenInit();       //창 생성.
	int i, j;
	FILE *fp;

	if (g_nStage == -1)    //맨 처음 실행한 경우
	{
		g_nStage = 0;      //다시 안 들어오게 0으로 변경.

		g_nPatCount = sizeof(g_strFileName) / sizeof(char *);               //정해진 패턴파일/한개의 패턴파일크기->패턴 개수
		g_PatInfo = (C_PatInfo *)malloc(sizeof(C_PatInfo) * g_nPatCount);     //패턴메모리 생성.  패턴개수의 크기만큼

																			// 패턴 파일 읽기
		for (i = 0; i < g_nPatCount; i++)   //패턴개수만큼 돌린다.
		{
			fp = fopen(g_strFileName[i], "r");		//패턴이 담긴 파일을 연다.
			fscanf(fp, "%d\n", &g_PatInfo[i].nCount); //첫 줄의 텍스쳐를 매개변수에 넣는다.  여기선 개수를
			fscanf(fp, "%d %d\n", &g_PatInfo[i].nX0, &g_PatInfo[i].nY0);  //여기선 시작 좌표값을
			g_PatInfo[i].pPat = (C_Pat*)malloc(sizeof(C_Pat) * g_PatInfo[i].nCount);  //패턴을 개수만큼 동적할당한다.
																				  //각 패턴을 열고선 그 패턴이 적용되는 개수와 시작 좌표를 변수에 저장한다. 그 후 얻어온 개수만큼 동적할당 시킨다.->같은 패턴을 가진 똑같은 적 여러명이 나옴.
			for (j = 0; j < g_PatInfo[i].nCount; j++)
				fscanf(fp, "%d %d %d %d\n", &g_PatInfo[i].pPat[j].nDirect, &g_PatInfo[i].pPat[j].nStep, &g_PatInfo[i].pPat[j].MoveTime,
					&g_PatInfo[i].pPat[j].nDist); //생성된 각 패턴의 적에게 이동방향,스템,이동시간간격,이동거리값 구해옴.
			fclose(fp);	 //파일을 닫는다.
		}

		//적 전부 초기화시킴

		g_nBossPatCount = sizeof(g_strBossFileName) / sizeof(char *);             //정해진 패턴파일/한개의 패턴파일크기->패턴 개수
		g_BossPatInfo = (C_PatInfo *)malloc(sizeof(C_PatInfo) * g_nBossPatCount); //패턴메모리 생성. 패턴개수의 크기만큼

																				// 보스 패턴 파일 읽기
		for (i = 0; i < g_nBossPatCount; i++)
		{
			fp = fopen(g_strBossFileName[i], "r");		//보스 패턴이 담긴 텍스트 연다.
			fscanf(fp, "%d\n", &g_BossPatInfo[i].nCount); //보스 패턴의 개수를 대입.
			fscanf(fp, "%d %d\n", &g_BossPatInfo[i].nX0, &g_BossPatInfo[i].nY0); //보스 패턴의 각 좌표값 대입
			g_BossPatInfo[i].pPat = (C_Pat*)malloc(sizeof(C_Pat) * g_BossPatInfo[i].nCount);  //보스의 패턴을 개수만큼 동적할당한다.

			for (j = 0; j < g_BossPatInfo[i].nCount; j++)
				fscanf(fp, "%d %d %d %d\n", &g_BossPatInfo[i].pPat[j].nDirect, &g_BossPatInfo[i].pPat[j].nStep,
					&g_BossPatInfo[i].pPat[j].MoveTime, &g_BossPatInfo[i].pPat[j].nDist);
			//생성된 각 패턴의 적에게 이동방향,스템,이동시간간격,이동거리값 구해옴.

			fclose(fp);
		}

	}

	g_nEnemyCount = g_StageInfo[g_nStage].nEnemyCount; // 적 캐릭터수 10
	srand((unsigned int)time(NULL));//매번 실행시 난수 만든

	int *set_time = new int;
	for (i = 0; i < g_nEnemyCount; i++)
	{
		*set_time= 1000 * (i + 1); // 출현 시간을 오름차순으로 설정	
		g_Enemy[i].setAp(*set_time);
        *set_time=rand() % 500 + 1000;
		g_Enemy[i].M_setTime(*set_time, 1);
		*set_time=clock();
		g_Enemy[i].M_setTime(*set_time, 2);
		g_Enemy[i].set_nPat(rand() % 4, 1);
		g_Enemy[i].set_nPat(-1, 2);
		g_Enemy[i].set_nPat(0,3);
	
		int pat = g_Enemy[i].get_nPat(1);
		g_Enemy[i].setXY(1,g_PatInfo[pat].nX0);
		g_Enemy[i].setXY(2,g_PatInfo[pat].nY0);
		g_Enemy[i].setLi(0);

		g_Enemy[i].setTime(g_PatInfo[pat].pPat[0].MoveTime, 1);
		g_Enemy[i].setTime(clock(), 2);
	}

	g_nEnemyIndex = 0; // 출현 캐릭터의 인덱스 범위
	g_StartTime = clock();
	delete set_time;
	// Note: 보스 설정
	g_Boss.set_nPat(0, 1);
	g_Boss.set_nPat(-1, 2);
	g_Boss.set_nPat(0, 3);

	int pat = g_Boss.get_nPat(1);
	g_Boss.setXY(1,g_BossPatInfo[pat].nX0);
	g_Boss.setXY(2,g_BossPatInfo[pat].nY0);
	g_Boss.setLi(g_StageInfo[g_nStage].nBossLife);
	g_Boss.setTime(g_BossPatInfo[pat].pPat[0].MoveTime, 1);
	g_Boss.setTime(clock(), 2);

	// Note: 보스 미사일 설정

	g_sBossMissile.F_setTime(1000, 1);
	g_sBossMissile.F_setTime(clock(), 2);
	g_sBossMissile.M_setTime(90, 1);

	memset(/*g_sBossMissile.sMissile*/g_sBossMissile.get(), 0, sizeof(C_BossMissile) * 30);
	// Note: 주인공

	g_Player.setXY(1, 30);
		g_Player.setXY(2, 21);
	g_Player.setLi(10);
	//g_Player.nLife = 10;
	//g_Player.nX = 30;
	//g_Player.nY = 21;
	
	g_Player.setTime(clock(),2);
	g_Player.setTime(50, 1);
	g_Player.F_setTime(50, 1);
	g_Player.F_setTime(clock(), 2);
		//OldTime = clock();
	/*g_Player.MoveTime = 50;
	g_Player.FireTime = 50;
	g_Player.OldFireTime = clock();
*/
	g_nGrade = 0;
	g_nDeadEnemy = 0;
	return 0;
}

int C_So_Manage::Run()
{
	int nKey, i;
	clock_t CurTime;	 //시간을 측정함.
	while (1)
	{
		CurTime = clock();

		if (_kbhit())//어떤 키라도 받으면
		{
			if (g_nGameState == RESULT)
				break;

			nKey = _getch(); //키를 아스키로

			switch (nKey)
			{
			case ' ': //스페이스->시작
				if (g_nGameState == INIT && g_nStage == 0)//맨 처음 시작.
				{
					g_nGameState = READY; //상태를 준비로 변경함.
					g_UpdateOldTime = clock();  // ready를 일정시간 지속해 주기 위해 							
				}
				break;

			case 75: // 왼쪽
				if (g_nGameState == RUNNING || g_nGameState == BOSS_WAR)  //게임 진행중 || 보스스테이지
				{
					if (CurTime - g_Player.getTime(2) > g_Player.getTime(1))  //시작시간-이전 이동 시간>이동 시간 간격->시간에 맞춰서 움직이게하려고
					{
						g_Player.setTime(CurTime, 2);
						//OldTime = CurTime;  //방금 이동한 시간을 처음시간으로 설정.
						if (g_Player.getXY(1) - 1 > 1) //맨 끝이 아니면
						{
							g_Player.setXY(1,(g_Player.getXY(1) - 1));
							//g_Player.nX--;  //좌표 감소

							PlayerCollision();  //
							break;
						}
					}
				}
				break;

			case 77: // 오른쪽
				if (g_nGameState == RUNNING || g_nGameState == BOSS_WAR)
				{
					if (CurTime - g_Player.getTime(2) > g_Player.getTime(1)/*CurTime - g_Player.OldTime > g_Player.MoveTime*/)
					{
						g_Player.setTime(CurTime, 2);
						//g_Player.OldTime = CurTime;
						if (g_Player.getXY(1) + 4 < 59)
						{
							//g_Player.nX++;
							g_Player.setXY(1,(g_Player.getXY(1) + 1));

							PlayerCollision();
							break;
						}
					}
				}
				break;

			case 's': // 미사일 발사
				if (g_nGameState == RUNNING || g_nGameState == BOSS_WAR)
				{
					if (CurTime - g_Player.F_getTime(2) > g_Player.F_getTime(1))
					{
						g_Player.F_setTime(CurTime, 2);
						//OldFireTime = CurTime;
						for (i = 0; i < HERO_MISSILE; i++)
						{
							if (g_PlayerMissile[i].getLi() == 0)
							{
								g_PlayerMissile[i].setXY(1, 3);
								g_PlayerMissile[i].setXY(1,g_Player.getXY(1) + 2);
								g_PlayerMissile[i].setXY(2,g_Player.getXY(2) - 1);
								g_PlayerMissile[i].setTime(clock(), 2);
								break;
							}
						}
					}
				}
				break;
			case 'y':
			case 'Y':
				if (g_nGameState == FAILED)
				{
					Init();
					g_nGameState = READY;
					g_UpdateOldTime = clock();
				}
				break;
			case 'n':
			case 'N':
				if (g_nGameState == FAILED)
				{
					g_nGameState = RESULT;
				}
				break;
			}
		}

		Update();
		Render();
	}
	return 0;
}
void C_So_Manage::BossMissileCollsion()
{
	int i, j;


	for (i = 0; i < 30; i++)
	{
		if (g_sBossMissile.get(i).getLi())
		{
			// Note: 보스 미사일과 주인공과 충돌
			if (Collision(g_sBossMissile.get(i).getLi(), g_sBossMissile.get(i).getXY(2),
				g_sBossMissile.get(i).getLi() + 1, g_sBossMissile.get(i).getXY(2),
				g_Player.getXY(1), g_Player.getXY(2), g_Player.getXY(1) + 4, g_Player.getXY(2)))
			{
				g_sBossMissile.get(i).setLi(0);

				g_Player.setLi(g_Player.getLi());
					//nLife--;
				//FMOD_System_PlaySound( g_System, FMOD_CHANNEL_FREE, g_EffectSound[5], 0, &g_pEffectChannel[5]);

				if (g_Player.getLi() < 1)
				{
					g_nGameState = STOP;
					return;
				}

				continue;
			}

			// Note: 보스 미사일과 주인공 미사일 충돌
			for (j = 0; j < HERO_MISSILE; j++)
			{
				if (g_PlayerMissile[j].getLi())
				{
					if (Collision(g_sBossMissile.get(i).getXY(1), g_sBossMissile.get(i).getXY(2),
						g_sBossMissile.get(i).getXY(1) + 1, g_sBossMissile.get(i).getXY(2),
						g_PlayerMissile[j].getXY(1), g_PlayerMissile[j].getXY(2), g_PlayerMissile[j].getXY(1), g_PlayerMissile[j].getXY(2)))
					{
						g_sBossMissile.get(i).setLi(0);
						g_PlayerMissile[j].setLi(0); 
						break;
					}
				}
			}
		}
	}
}


// 보스 미사일 입장에서 주인공과 주인공 미사일 충돌 체크

int C_So_Manage::Collision(int nX1, int nY1, int nX2, int nY2, int nX3, int nY3, int nX4, int nY4)
{
	if (nX1 <= nX4 && nX2 >= nX3 && nY1 <= nY4 && nY2 >= nY3)
		return 1;  // 충돌
	else
		return 0;  // 비충돌
}

void C_So_Manage::PlayerCollision()
{
	int i, j;

	for( i = 0 ; i < g_nEnemyIndex ; i++ )  
	{
		if( g_Enemy[i].getLi())
		{
			// 적 캐릭터와 충돌 체크
			if( Collision( g_Enemy[i].getXY(1), g_Enemy[i].getXY(2), g_Enemy[i].getXY(1) + 1, g_Enemy[i].getXY(2), g_Player.getXY(1), g_Player.getXY(2), g_Player.getXY(1) + 4, g_Player.getXY(2)) )
			{
				g_Enemy[i].setLi(0); 
				g_Player.setLi(g_Player.getLi() - 1);
				
				if( g_Player.getLi() < 1 )
				{
					g_nGameState = STOP;
					return ;
				}								
			}
		}
	}

	// 적 미사일과 충돌 체크
	for( j = 0 ; j < 100 ; j++ )
	{
		if( g_EnemyMissile[j].getLi())
		{
			if( Collision(  g_EnemyMissile[j].getXY(1),  g_EnemyMissile[j].getXY(2),  g_EnemyMissile[j].getXY(1) + 1,  g_EnemyMissile[j].getXY(2),
							g_Player.getXY(1), g_Player.getXY(2), g_Player.getXY(1) + 4, g_Player.getXY(2)) )
			{
				g_EnemyMissile[j].setLi(0);
				g_Player.setLi(g_Player.getLi() - 1);
			
				if( g_Player.getLi() < 1 )
				{
					g_nGameState = STOP;
					return ;
				}						
			}
		}
	}		
}
//// 주인공 미사일입장에서 보스와 보스 미사일 충돌
void C_So_Manage::PlayerMissileBossCollision()
{
	int i, j;	
	
	for( i = 0 ; i < HERO_MISSILE ; i++ )
	{
		if( g_PlayerMissile[i].getLi() )
		{
		    // 주인공 미사일과 보스 충돌체크
			if( Collision( g_PlayerMissile[i].getXY(1), g_PlayerMissile[i].getXY(2), g_PlayerMissile[i].getXY(1), g_PlayerMissile[i].getXY(2),
							g_Boss.getXY(1), g_Boss.getXY(2), g_Boss.getXY(1) + 16, g_Boss.getXY(2) + 2 ) )
			{
				g_PlayerMissile[i].setLi(0);
				g_Boss.setLi(g_Boss.getLi()-1);
				//FMOD_System_PlaySound( g_System, FMOD_CHANNEL_FREE, g_EffectSound[5], 0, &g_pEffectChannel[5]);
				
				if( g_Boss.getLi() < 1 )
				{
					g_nGameState = STOP;
					return ;
				}			
				
				continue;
			}

			// 주인공 미사일과 보스 미사일과 충돌 체크
			for( j = 0 ; j < 30 ; j++ )
			{
				if( g_sBossMissile.get(j).getLi())
				{
					if( Collision( g_PlayerMissile[i].getXY(1), g_PlayerMissile[i].getXY(2), g_PlayerMissile[i].getXY(1), g_PlayerMissile[i].getXY(2),
									g_sBossMissile.get(j).getXY(1), g_sBossMissile.get(j).getXY(2),
									g_sBossMissile.get(j).getXY(1) + 1 , g_sBossMissile.get(j).getXY(2)) )
					{
						g_PlayerMissile[i].setLi(0);
						g_sBossMissile.get(j).setLi(0);
						break;								
					}
				}
			}
		}
	}
}

void C_So_Manage::Update()
{
	int nSignX, nSignY, i, j, nFireMissileCount;
	clock_t CurTime = clock();

	switch (g_nGameState)
	{
	case READY: //전 상황이 ready면 3초 후 running
		if (CurTime - g_UpdateOldTime > 3000)  // 3초->대기시간이 3초
		{
			g_nGameState = RUNNING;  //진행으로 바꿈.
			g_GameStartTime = CurTime;	//측정한 시간이 시작시간.

		}
		break;

	case RUNNING:
		// Note: 출현 ( 출현 시간의 데이터는 오름차순으로 정렬되어 있다 )
		for (i = g_nEnemyIndex; i < g_nEnemyCount; i++)
		{
			if (g_Enemy[i].getLi() == 0)
			{
				if (CurTime - g_StartTime >= g_Enemy[i].getAp())  //함수 시작한 시간-초기화떄 측정한->INIT에서 uPDATE까지 걸린 시간. >= 적이 나오는 시간.-> 정해진 시간보다 나오는 시간이 적을 떄만
				{
					g_Enemy[i].setLi(1); //값 변경.
					g_nEnemyIndex++;      //->++i
				}
				else {			//시간 끝나면 종료.				
					break;
				}
			}
		}

		// Note:  적 캐릭터 이동 과 미사일 발사 시작 
		for (i = 0; i < g_nEnemyIndex; i++) //방금 적이 몇명 생겼나.
		{
			if (g_Enemy[i].getLi())
			{
				// Note: 적 캐릭터 업데이트
				if (CurTime - g_Enemy[i].getTime(3) > g_Enemy[i].getTime(1)) //함수 시작한 시간-각 적의 초기화 때의 시간>적의 움직이는 시간==정해진 시간보다 적을 떄에만 작동.
				{
					g_Enemy[i].setTime(CurTime,3);  //update
					g_Enemy[i].set_nPat(g_Enemy[i].get_nPat(2) + 1, 2);
					//	nPatStep++;  //0에서 증가함.

											// 다음 패턴 인덱스로 진행--->땅에 닿으면
					if (g_Enemy[i].get_nPat(2) == g_PatInfo[g_Enemy[i].get_nPat(1)].pPat[g_Enemy[i].get_nPat(3)].nStep) //패턴 일치할겨우
					{    //위에서 얼마큼 있는지 보는건데 내려갈수록 nPatStep이 증가하고 정해진 값만큼 내려왔다면 지운다.
						g_Enemy[i].set_nPat(g_Enemy[i].get_nPat(3) + 1, 3);
						//g_Enemy[i].nPatIndex++;
						if (g_Enemy[i].get_nPat(3) == g_PatInfo[g_Enemy[i].get_nPat(1)].nCount) //같으면->a를 3개 만들고 3개가 다 죽으면
						{
							g_Enemy[i].setLi(0);  //죽음을 표시
							g_nDeadEnemy++;     //죽은 경우 개수

							if (g_nDeadEnemy == g_nEnemyCount)//플레이어가 승리.
							{
								g_nGameState = PLAYER_TALK; //플레이어가 승리.
								g_UpdateOldTime = CurTime;  //업데이트 끝났으니 그 시간 대입.
								return;
							}
							else
								continue;  // 패턴이동이 종료되어 다음 캐릭터를 설정
						}
						else {
							g_Enemy[i].setTime(g_PatInfo[g_Enemy[i].get_nPat(1)].pPat[g_Enemy[i].get_nPat(3)].MoveTime,1);  //죽지 않았다면 패턴에서 가져온 시간으로 변경
							g_Enemy[i].set_nPat(0,2); //다시 초기화.
						}
					}

					switch (g_PatInfo[g_Enemy[i].get_nPat(1)].pPat[g_Enemy[i].get_nPat(3)].nDirect)
					{
					case UP:
						nSignX = 0;
						nSignY = -1;
						break;
					case UP_RIGHT:
						nSignX = 1;
						nSignY = -1;
						break;
					case RIGHT:
						nSignX = 1;
						nSignY = 0;
						break;
					case DOWN_RIGHT:
						nSignX = 1;
						nSignY = 1;
						break;
					case DOWN:
						nSignX = 0;
						nSignY = 1;
						break;
					case DOWN_LEFT:
						nSignX = -1;
						nSignY = 1;
						break;
					case LEFT:
						nSignX = -1;
						nSignY = 0;
						break;
					case UP_LEFT:
						nSignX = -1;
						nSignY = -1;
						break;
					}
					g_Enemy[i].setXY(1,g_Enemy[i].getXY(1) + nSignX*g_PatInfo[g_Enemy[i].get_nPat(1)].pPat[g_Enemy[i].get_nPat(3)].nDist);
					g_Enemy[i].setXY(2,g_Enemy[i].getXY(2) + nSignY*g_PatInfo[g_Enemy[i].get_nPat(1)].pPat[g_Enemy[i].get_nPat(3)].nDist);
				}

				// Note: 적 미사일 발사 하는 부분 ( 캐릭터가 살아 있는 때만 가능 )
				if (CurTime - g_Enemy[i].M_getTime(2) > g_Enemy[i].M_getTime(1))  //함수 시작한시간-전에 미사일 쏜 시간>미사일 발사 시간  ==미사일 발사 기간 일정하게
				{
					g_Enemy[i].M_setTime(CurTime,2);  //쏜 시간을 지금으로 교체
					for (j = 0; j < 100; j++)
					{
						if (g_EnemyMissile[j].getLi() == 0)  //미사일 나오기 전
						{
							g_EnemyMissile[j].setXY(1,g_Enemy[i].getXY(1));
							g_EnemyMissile[j].setXY(2,g_Enemy[i].getXY(2)+1);
							g_EnemyMissile[j].setLi(1);
							g_EnemyMissile[j].setTime(rand() % 100 + 80, 1);
							//g_EnemyMissile[j].nLife = 1; //미사일 생겨서 생명력 부여
						//	g_EnemyMissile[j].MoveTime = rand() % 100 + 80;
							g_EnemyMissile[j].setTime(CurTime,3);
							break;
						}
					}
				}
			}
		}

		//------------------ 적 캐릭터 이동 끝 ------------------------------

		for (i = 0; i < g_nEnemyIndex; i++)
		{
			if (g_Enemy[i].getLi())
			{
				// Note: 적캐릭터와 주인공의 충돌 체크
				if (Collision(g_Enemy[i].getXY(1), g_Enemy[i].getXY(2), g_Enemy[i].getXY(1) + 1, g_Enemy[i].getXY(2),
					g_Player.getXY(1), g_Player.getXY(2), g_Player.getXY(1) + 4, g_Player.getXY(2)))
				{
					g_Enemy[i].setLi(0);
					g_nGrade += 10; // 점수
					g_Player.setLi(g_Player.getLi()-1);;

					if (g_Player.getLi() < 1)
					{
						g_nGameState = STOP;
						return;
					}

					g_nDeadEnemy++;
					if (g_nDeadEnemy == g_nEnemyCount)
					{
						g_nGameState = PLAYER_TALK;
						g_UpdateOldTime = CurTime;
						g_nDeadEnemy = 0;
						return;
					}

					continue;
				}

				// Note: 적 캐릭터와 주인공 미사일의 충돌 체크
				for (j = 0; j < 10; j++)
				{
					if (g_PlayerMissile[j].getLi()) 
					{
						if (Collision(g_Enemy[i].getXY(1), g_Enemy[i].getXY(2), g_Enemy[i].getXY(1) + 1, g_Enemy[i].getXY(2),
							g_PlayerMissile[j].getXY(1), g_PlayerMissile[j].getXY(2), g_PlayerMissile[j].getXY(1), g_PlayerMissile[j].getXY(2)))
						{
							g_Enemy[i].setLi(0);
							g_PlayerMissile[j].setLi(0);
							g_nGrade += 10; // 점수
							g_nDeadEnemy++;

							if (g_nDeadEnemy == g_nEnemyCount)
							{
								g_nGameState = PLAYER_TALK;
								g_UpdateOldTime = CurTime;
								g_nDeadEnemy = 0;
								return;
							}

							break;
						}
					}
				}
			}
		}


		// Note: 적 미사일 이동 
		for (i = 0; i < 100; i++)
		{
			if (g_EnemyMissile[i].getLi())
			{
				if (CurTime - g_EnemyMissile[i].getTime(3) > g_EnemyMissile[i].getTime(1))
				{
					g_EnemyMissile[i].setTime(CurTime,3);

					if (g_EnemyMissile[i].getXY(2) + 1 > 22)
					{
						g_EnemyMissile[i].setLi(0);
						continue;
					}
					else {
						g_EnemyMissile[i].setXY(2,g_EnemyMissile[i].getXY(2)+1);
					}
				}
			}
		}


		// Note:  적 미사일과 주인공, 주인공 미사일 충돌 체크 
		for (i = 0; i < 100; i++)
		{
			if (g_EnemyMissile[i].getLi())
			{
				// Note: 충돌 체크 => 적 미사일과 주인공 
				if (Collision(g_EnemyMissile[i].getXY(1), g_EnemyMissile[i].getXY(2), g_EnemyMissile[i].getXY(1) + 1, g_EnemyMissile[i].getXY(2),
					g_Player.getXY(1), g_Player.getXY(2), g_Player.getXY(1) + 4, g_Player.getXY(2)))
				{
					g_EnemyMissile[i].setLi(0);
					g_Player.setLi(g_Player.getLi() - 1);
						//nLife--;
				
					if (g_Player.getLi() < 1)
					{
						g_nGameState = STOP;
						return;
					}

					continue;
				}

				// Note: 충돌 체크 => 적 미사일과 주인공 미사일
				for (j = 0; j < HERO_MISSILE; j++)
				{
					if (g_PlayerMissile[j].getLi())
					{
						if (Collision(g_EnemyMissile[i].getXY(1), g_EnemyMissile[i].getXY(2), g_EnemyMissile[i].getXY(1) + 1, g_EnemyMissile[i].getXY(2),
							g_PlayerMissile[j].getXY(1), g_PlayerMissile[j].getXY(2), g_PlayerMissile[j].getXY(1), g_PlayerMissile[j].getXY(2)))
						{
							g_EnemyMissile[i].setLi(0);
							g_PlayerMissile[j].setLi(0);
							break;
						}
					}
				}
			}
		}

		//----------------주인공 미사일 이동 ----------------------------------------------------
		for (i = 0; i < HERO_MISSILE; i++)
		{
			if (g_PlayerMissile[i].getLi())
			{   // 주인공 미사일 이동
				if (CurTime - g_PlayerMissile[i].getTime(2) > MISSILE_MOVE_TIME)
				{
					g_PlayerMissile[i].setTime(CurTime, 2);
					if (g_PlayerMissile[i].getXY(1) - 1 > 2)
						g_PlayerMissile[i].setXY(2,g_PlayerMissile[i].getXY(2) - 1);
						//nY--;
					else
						g_PlayerMissile[i].setLi(0);
				}
			}
		}

		//------------------주인공 미사일 과 적 캐릭터 및 적 미사일 충돌 ---------------------

		for (i = 0; i < HERO_MISSILE; i++)
		{
			if (g_PlayerMissile[i].getLi())
			{
				// Note: 주인공 미사일과 적 캐릭터의 충돌 체크
				for (j = 0; j < g_nEnemyIndex; j++)
				{
					if (g_Enemy[j].getLi())
					{
						if (Collision(g_PlayerMissile[i].getXY(1), g_PlayerMissile[i].getXY(2), g_PlayerMissile[i].getXY(1), g_PlayerMissile[i].getXY(2),
							g_Enemy[j].getXY(1), g_Enemy[j].getXY(2), g_Enemy[j].getXY(1) + 1, g_Enemy[j].getXY(2)))
						{
							g_PlayerMissile[i].setLi(0);
							g_Enemy[j].setLi(0);
							g_nGrade += 10; // 점수
							g_nDeadEnemy++; // 적 캐릭터 한개 소멸

							if (g_nDeadEnemy == g_nEnemyCount)
							{
								g_nGameState = PLAYER_TALK;
								g_UpdateOldTime = CurTime;
								g_nDeadEnemy = 0;
								return;
							}
							break;
						}
					}
				}
			}
		}

		// Note: 주인공 미사일과 적미사일의 충돌 체크
		for (i = 0; i < HERO_MISSILE; i++)
		{
			if (g_PlayerMissile[i].getLi())
			{
				for (j = 0; j < 100; j++)
				{
					if (g_EnemyMissile[j].getLi())
					{
						if (Collision(g_PlayerMissile[i].getXY(1), g_PlayerMissile[i].getXY(2), g_PlayerMissile[i].getXY(1), g_PlayerMissile[i].getXY(2),
							g_EnemyMissile[j].getXY(1), g_EnemyMissile[j].getXY(2), g_EnemyMissile[j].getXY(1) + 1, g_EnemyMissile[j].getXY(2)))
						{
							g_PlayerMissile[i].setXY(0,3);
							g_EnemyMissile[j].setXY(0,3);

							break;
						}
					}
				}
			}
		}
		break;

	case PLAYER_TALK:
		if (CurTime - g_UpdateOldTime > 3000)  // 3초
		{
			g_nGameState = BOSS_TALK;
			g_UpdateOldTime = CurTime;
		}
		break;
	case BOSS_TALK:
		if (CurTime - g_UpdateOldTime > 3000)  // 3초
		{
			g_nGameState = BOSS_WAR;
			g_UpdateOldTime = CurTime;
			memset(g_PlayerMissile, 0, sizeof(C_MainObject)*HERO_MISSILE);
		}
		break;

	case BOSS_WAR:
		// Note: 보스의 이동
		if (g_Boss.getLi())
		{
			if (CurTime - g_Boss.getTime(2) > g_Boss.getTime(1))
			{
				g_Boss.setTime(CurTime, 2);
				g_Boss.set_nPat(g_Boss.get_nPat(2) + 1, 2);
				
					//nPatStep++;

				if (g_Boss.get_nPat(2) == g_BossPatInfo[g_Boss.get_nPat(1)].pPat[g_Boss.get_nPat(3)].nStep)
				{
					g_Boss.set_nPat(g_Boss.get_nPat(3) + 1, 3);
						//nPatIndex++;

					if (g_Boss.get_nPat(3) == g_BossPatInfo[g_Boss.get_nPat(1)].nCount)
					{
						g_Boss.set_nPat(g_Boss.get_nPat(1) + 1, 1);
						//	nPatType++;
						g_Boss.set_nPat(g_Boss.get_nPat(1) % g_nBossPatCount,1); // 패턴 반복
						g_Boss.set_nPat(0,3);
						g_Boss.set_nPat(-1,2);
						g_Boss.setXY(1,g_BossPatInfo[g_Boss.get_nPat(1)].nX0);
						g_Boss.setXY(2,g_BossPatInfo[g_Boss.get_nPat(1)].nY0);
						g_Boss.setTime(g_BossPatInfo[g_Boss.get_nPat(1)].pPat[0].MoveTime,1);
						g_Boss.setTime(clock(),2);
					}
					else {
						g_Boss.setTime(g_BossPatInfo[g_Boss.get_nPat(1)].pPat[g_Boss.get_nPat(3)].MoveTime,1);
						g_Boss.set_nPat(0,2);
					}
				}

				switch (g_BossPatInfo[g_Boss.get_nPat(1)].pPat[g_Boss.get_nPat(3)].nDirect)
				{
				case UP:
					nSignX = 0;
					nSignY = -1;
					break;
				case UP_RIGHT:
					nSignX = 1;
					nSignY = -1;
					break;
				case RIGHT:
					nSignX = 1;
					nSignY = 0;
					break;
				case DOWN_RIGHT:
					nSignX = 1;
					nSignY = 1;
					break;
				case DOWN:
					nSignX = 0;
					nSignY = 1;
					break;
				case DOWN_LEFT:
					nSignX = -1;
					nSignY = 1;
					break;
				case LEFT:
					nSignX = -1;
					nSignY = 0;
					break;
				case UP_LEFT:
					nSignX = -1;
					nSignY = -1;
					break;
				}

				g_Boss.setXY(1,g_Boss.getXY(1) + nSignX*g_BossPatInfo[g_Boss.get_nPat(1)].pPat[g_Boss.get_nPat(3)].nDist);
				g_Boss.setXY(2,g_Boss.getXY(2) + nSignY*g_BossPatInfo[g_Boss.get_nPat(1)].pPat[g_Boss.get_nPat(3)].nDist);
			}

			if (CurTime - g_sBossMissile.F_getTime(2) > g_sBossMissile.F_getTime(1))
			{
				g_sBossMissile.F_setTime(CurTime,2);
				nFireMissileCount = 0;

				for (i = 0; i < 30; i++)    // 30개의 미사일을 검색하는 부분
				{
					if (g_sBossMissile.get(i).getLi())/*g_sBossMissile.get(i).setN(0,3)*/
					{
						g_sBossMissile.get(i).setXY(1,g_Boss.getXY(1) + 6);
						g_sBossMissile.get(i).setXY(2,g_Boss.getXY(2) + 2);
						g_sBossMissile.get(i).setLi(1);
						g_sBossMissile.get(i).set_nDir(DIRECT(DOWN_LEFT - nFireMissileCount)); // 방향설정: 5, 4, 3
						g_sBossMissile.get(i).setTime(CurTime,3);
						nFireMissileCount++;

						if (nFireMissileCount > 2) // 3발 이상 발사 되지 못하게 하는 부분
							break;
					}
				}
			}
		}

		// Note: 보스와 주인공 미사일의 충돌 체크 (1)
		for (i = 0; i < HERO_MISSILE; i++)
		{
			if (g_PlayerMissile[i].getLi())
			{
				if (Collision(g_Boss.getXY(1), g_Boss.getXY(2), g_Boss.getXY(1) + 16, g_Boss.getXY(2) + 2,
					g_PlayerMissile[i].getXY(1), g_PlayerMissile[i].getXY(2), g_PlayerMissile[i].getXY(1), g_PlayerMissile[i].getXY(2)))
				{
					g_PlayerMissile[i].setLi(0);
					g_Boss.setLi(g_Boss.getLi() - 1);
						//nLife--;


					if (g_Boss.getLi() < 1)
					{
						g_nGameState = STOP;
						return;
					}

				}
			}
		}

		// Note: 보스 미사일 이동
		for (i = 0; i < 30; i++)
		{
			if (g_sBossMissile.get(i).getLi())
			{
				if (CurTime - g_sBossMissile.get(i).getTime(3) > g_sBossMissile.getTime(1))
				{
					g_sBossMissile.get(i).setTime(CurTime,3);
					switch (g_sBossMissile.get(i).get_nDir())
					{
					case DOWN_RIGHT:
						nSignX = 1;
						nSignY = 1;
						break;
					case DOWN:
						nSignX = 0;
						nSignY = 1;
						break;
					case DOWN_LEFT:
						nSignX = -1;
						nSignY = 1;
						break;
					}

					g_sBossMissile.get(i).setXY(1,g_sBossMissile.get(i).getXY(1) + nSignX);
					g_sBossMissile.get(i).setXY(2,g_sBossMissile.get(i).getXY(2) + nSignY);
				}

				// Note: 경계 영역 충돌 체크
				if (g_sBossMissile.get(i).getXY(1)< 2 || g_sBossMissile.get(i).getXY(1)+ 1 > 59 ||
					g_sBossMissile.get(i).getXY(2) > 22)
				{
					g_sBossMissile.get(i).setLi(0);
				}
			}
		}

		 BossMissileCollsion();

		//------------------------ 주인공과 보스 미사일 충돌 체크 부분   ---------------------------//
		for (i = 0; i < 30; i++)
		{
			if (g_sBossMissile.get(i).getLi())
			{
				if (Collision(g_sBossMissile.get(i).getXY(1), g_sBossMissile.get(i).getXY(2), g_sBossMissile.get(i).getXY(1) + 1, g_sBossMissile.get(i).getXY(2),
					g_Player.getXY(1), g_Player.getXY(2), g_Player.getXY(1) + 4, g_Player.getXY(2)))
				{
					g_sBossMissile.get(i).setLi(0);

					g_Player.setLi(g_Player.getLi() - 1);
					//	nLife--;
					
					if (g_Player.getLi() < 1)
					{
						g_nGameState = STOP;
						return;
					}
				}
			}
		}

		//------------------------ 주인공 미사일 이동  --------------------------//
		for (i = 0; i < HERO_MISSILE; i++)
		{
			if (g_PlayerMissile[i].getLi())
			{
				if (CurTime - g_PlayerMissile[i].getTime(2) > MISSILE_MOVE_TIME)
				{
					g_PlayerMissile[i].setTime(CurTime,2);
					g_PlayerMissile[i].setXY(2,g_PlayerMissile[i].getXY(2)-1);//nY--;

					// 경계 영역 충돌 처리
					if (g_PlayerMissile[i].getXY(2) < 2)
					{
						g_PlayerMissile[i].setLi(0);
					}
				}
			}
		}

		 PlayerMissileBossCollision();
		break;

	case STOP:
		if (g_Player.getLi() < 1)
		{
			g_nGameState = FAILED;
			break;
		}

		if (g_Boss.getLi() < 1)
		{
			g_UpdateOldTime = CurTime;
			g_nGameState = SUCCESS;
			g_nTotalGrade += g_nGrade;
			g_nTotalGrade += 100; // 보스를 이겼으므로

		}
		break;

	case SUCCESS:
		if (CurTime - g_UpdateOldTime > 3000)
		{
			g_UpdateOldTime = CurTime;
			++g_nStage;
			g_nStage = g_nStage % 3; 	// 스테이지가 3개밖에 없어서.	
			Init();
			g_nGameState = READY;
		}
		break;
	}
}

void C_So_Manage::Render()
{
	C_Screen* screen=new C_Screen;

	int i;
	char string[100];
	ScreenClear();

	switch( g_nGameState )
	{
	case INIT :  //시간차로 
				if( g_nStage == 0 )
				screen->InitScreen();
				break;

	case READY :
		        screen->ReadyScreen();
				sprintf( string, "%d Stage", g_nStage + 1);
				ScreenPrint( 23, 9, string);
				break;

	case RUNNING :					
		screen->ScreenLine();

				 for( i = 0; i < 100 ; i++ )
				 {	
					if( g_EnemyMissile[i].getLi() )
					{			
					   ScreenPrint( g_EnemyMissile[i].getXY(1), g_EnemyMissile[i].getXY(2), "↓" );
					}
				 }

				 for( i = 0 ; i < g_nEnemyIndex ; i++ )
				 {
					if( g_Enemy[i].getXY(2) )  //숫자 기억이..
					{
					   ScreenPrint( g_Enemy[i].getXY(1), g_Enemy[i].getXY(2), g_EnemyType[g_Enemy[i].get_nPat(1)] );
					 }
				 }

			 for( i = 0 ; i < HERO_MISSILE ; i++ )
				 {
					if( g_PlayerMissile[i].getLi() == 1 )
						ScreenPrint( g_PlayerMissile[i].getXY(1), g_PlayerMissile[i].getXY(2), "$" );
				 }

				 ScreenPrint( g_Player.getXY(1), g_Player.getXY(2), "#-@-#" );

				 sprintf( string, "stage : %d ", g_nStage + 1 );
				 ScreenPrint( 63, 3, string );
				 sprintf( string, "주인공 HP : %d", g_Player.getLi() );
				 ScreenPrint( 63, 5, string );
				 sprintf( string, "점수: %d", g_nGrade );
				 ScreenPrint( 63, 7, string );
				 break;

	case PLAYER_TALK:
		screen->PlayerTalk();
				break;

	case BOSS_TALK:
		screen->BossTalk();
				break;

	case BOSS_WAR:
		screen->ScreenLine();
				ScreenPrint( g_Boss.getXY(1), g_Boss.getXY(2)  , " /| ━ // ━ |＼"); 
				ScreenPrint( g_Boss.getXY(1), g_Boss.getXY(2)+1, "//q ●    ● p＼");
				ScreenPrint( g_Boss.getXY(1), g_Boss.getXY(2)+2, "  (┗┻━┻┛)"); 				

				for( i = 0 ; i < 30 ; i++ )
				{
					if( g_sBossMissile.get(i).getLi() )
					{
						ScreenPrint( g_sBossMissile.get(i).getXY(1), g_sBossMissile.get(i).getXY(2), "↓" );
					}
				}

				// 주인공 미사일 렌더링
				for( i = 0 ; i < HERO_MISSILE ; i++ )
				{
					if( g_PlayerMissile[i].getLi() )
						ScreenPrint( g_PlayerMissile[i].getXY(1), g_PlayerMissile[i].getXY(2), "$" );
				}

				ScreenPrint( g_Player.getXY(1), g_Player.getXY(2), "#-@-#" );
				
				sprintf( string, "보스 HP : %d", g_Boss.getLi() );
				ScreenPrint( 63, 3, string );
				sprintf( string, "주인공 HP : %d", g_Player.getLi() );
				ScreenPrint( 63, 5, string );
				break;

	 case SUCCESS :				
		 screen->SuccessScreen();
				sprintf( string, "%d", g_nStage + 1 );
				ScreenPrint( 19, 7, string );
				sprintf( string, "총점: %d", g_nTotalGrade );
				ScreenPrint( 18, 17, string );				
				break;

	case FAILED :				
		screen->FailureScreen();

				break;

	case RESULT :
		screen->ResultScreen();
				sprintf( string, "%d", g_nTotalGrade );
				ScreenPrint( 48, 13, string );
				break;

	}

	ScreenFlipping();
}

void C_So_Manage::Release()
{	
     int i, nFileCount;

     nFileCount = sizeof( g_strFileName ) / sizeof( char * );
     for( i = 0; i < nFileCount ; i++ )
		  free( g_PatInfo[i].pPat );
	
     free( g_PatInfo );

	 nFileCount = sizeof( g_strBossFileName ) / sizeof( char * );
	 for( i = 0 ; i < nFileCount ; i++ )
		 free( g_BossPatInfo[i].pPat );

	 free( g_BossPatInfo );

	 
}