#include"Sogomanage.h"


//����ü ����
STAGE_INFO g_StageInfo[] = { { 10, 12 },{ 10, 20 },{ 15, 30 } };

//----1.��ü ����
C_MainObject g_Player;         //�÷��̾�
C_MainObject g_PlayerMissile[HERO_MISSILE];  //�÷��̾� �̻����� �迭��
C_PatInfo *g_PatInfo;                        //�� ������ ����
C_PatInfo *g_BossPatInfo;                   //������ ������ ����
C_Boss g_Boss;                              //����
C_Enemy g_Enemy[50];                        //��
C_BossMissileInfo g_sBossMissile;           //�����̻���
char *g_strFileName[] = { "pat1.txt", "pat2.txt", "pat3.txt", "pat4.txt" };
char *g_strBossFileName[] = { "boss_pat1.txt", "boss_pat2.txt" };
C_MainObject  g_EnemyMissile[100];   // �̻��� ����

C_So_Manage::C_So_Manage()
{
	g_nGameState = INIT;
	g_nStage = -1;
	g_nGrade = 0; // ���� 
	g_nTotalGrade = 0; // ����
}

int C_So_Manage::Init()
{
	ScreenInit();       //â ����.
	int i, j;
	FILE *fp;

	if (g_nStage == -1)    //�� ó�� ������ ���
	{
		g_nStage = 0;      //�ٽ� �� ������ 0���� ����.

		g_nPatCount = sizeof(g_strFileName) / sizeof(char *);               //������ ��������/�Ѱ��� ��������ũ��->���� ����
		g_PatInfo = (C_PatInfo *)malloc(sizeof(C_PatInfo) * g_nPatCount);     //���ϸ޸� ����.  ���ϰ����� ũ�⸸ŭ

																			// ���� ���� �б�
		for (i = 0; i < g_nPatCount; i++)   //���ϰ�����ŭ ������.
		{
			fp = fopen(g_strFileName[i], "r");		//������ ��� ������ ����.
			fscanf(fp, "%d\n", &g_PatInfo[i].nCount); //ù ���� �ؽ��ĸ� �Ű������� �ִ´�.  ���⼱ ������
			fscanf(fp, "%d %d\n", &g_PatInfo[i].nX0, &g_PatInfo[i].nY0);  //���⼱ ���� ��ǥ����
			g_PatInfo[i].pPat = (C_Pat*)malloc(sizeof(C_Pat) * g_PatInfo[i].nCount);  //������ ������ŭ �����Ҵ��Ѵ�.
																				  //�� ������ ���� �� ������ ����Ǵ� ������ ���� ��ǥ�� ������ �����Ѵ�. �� �� ���� ������ŭ �����Ҵ� ��Ų��.->���� ������ ���� �Ȱ��� �� �������� ����.
			for (j = 0; j < g_PatInfo[i].nCount; j++)
				fscanf(fp, "%d %d %d %d\n", &g_PatInfo[i].pPat[j].nDirect, &g_PatInfo[i].pPat[j].nStep, &g_PatInfo[i].pPat[j].MoveTime,
					&g_PatInfo[i].pPat[j].nDist); //������ �� ������ ������ �̵�����,����,�̵��ð�����,�̵��Ÿ��� ���ؿ�.
			fclose(fp);	 //������ �ݴ´�.
		}

		//�� ���� �ʱ�ȭ��Ŵ

		g_nBossPatCount = sizeof(g_strBossFileName) / sizeof(char *);             //������ ��������/�Ѱ��� ��������ũ��->���� ����
		g_BossPatInfo = (C_PatInfo *)malloc(sizeof(C_PatInfo) * g_nBossPatCount); //���ϸ޸� ����. ���ϰ����� ũ�⸸ŭ

																				// ���� ���� ���� �б�
		for (i = 0; i < g_nBossPatCount; i++)
		{
			fp = fopen(g_strBossFileName[i], "r");		//���� ������ ��� �ؽ�Ʈ ����.
			fscanf(fp, "%d\n", &g_BossPatInfo[i].nCount); //���� ������ ������ ����.
			fscanf(fp, "%d %d\n", &g_BossPatInfo[i].nX0, &g_BossPatInfo[i].nY0); //���� ������ �� ��ǥ�� ����
			g_BossPatInfo[i].pPat = (C_Pat*)malloc(sizeof(C_Pat) * g_BossPatInfo[i].nCount);  //������ ������ ������ŭ �����Ҵ��Ѵ�.

			for (j = 0; j < g_BossPatInfo[i].nCount; j++)
				fscanf(fp, "%d %d %d %d\n", &g_BossPatInfo[i].pPat[j].nDirect, &g_BossPatInfo[i].pPat[j].nStep,
					&g_BossPatInfo[i].pPat[j].MoveTime, &g_BossPatInfo[i].pPat[j].nDist);
			//������ �� ������ ������ �̵�����,����,�̵��ð�����,�̵��Ÿ��� ���ؿ�.

			fclose(fp);
		}

	}

	g_nEnemyCount = g_StageInfo[g_nStage].nEnemyCount; // �� ĳ���ͼ� 10
	srand((unsigned int)time(NULL));//�Ź� ����� ���� ����

	int *set_time = new int;
	for (i = 0; i < g_nEnemyCount; i++)
	{
		*set_time= 1000 * (i + 1); // ���� �ð��� ������������ ����	
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

	g_nEnemyIndex = 0; // ���� ĳ������ �ε��� ����
	g_StartTime = clock();
	delete set_time;
	// Note: ���� ����
	g_Boss.set_nPat(0, 1);
	g_Boss.set_nPat(-1, 2);
	g_Boss.set_nPat(0, 3);

	int pat = g_Boss.get_nPat(1);
	g_Boss.setXY(1,g_BossPatInfo[pat].nX0);
	g_Boss.setXY(2,g_BossPatInfo[pat].nY0);
	g_Boss.setLi(g_StageInfo[g_nStage].nBossLife);
	g_Boss.setTime(g_BossPatInfo[pat].pPat[0].MoveTime, 1);
	g_Boss.setTime(clock(), 2);

	// Note: ���� �̻��� ����

	g_sBossMissile.F_setTime(1000, 1);
	g_sBossMissile.F_setTime(clock(), 2);
	g_sBossMissile.M_setTime(90, 1);

	memset(/*g_sBossMissile.sMissile*/g_sBossMissile.get(), 0, sizeof(C_BossMissile) * 30);
	// Note: ���ΰ�

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
	clock_t CurTime;	 //�ð��� ������.
	while (1)
	{
		CurTime = clock();

		if (_kbhit())//� Ű�� ������
		{
			if (g_nGameState == RESULT)
				break;

			nKey = _getch(); //Ű�� �ƽ�Ű��

			switch (nKey)
			{
			case ' ': //�����̽�->����
				if (g_nGameState == INIT && g_nStage == 0)//�� ó�� ����.
				{
					g_nGameState = READY; //���¸� �غ�� ������.
					g_UpdateOldTime = clock();  // ready�� �����ð� ������ �ֱ� ���� 							
				}
				break;

			case 75: // ����
				if (g_nGameState == RUNNING || g_nGameState == BOSS_WAR)  //���� ������ || ������������
				{
					if (CurTime - g_Player.getTime(2) > g_Player.getTime(1))  //���۽ð�-���� �̵� �ð�>�̵� �ð� ����->�ð��� ���缭 �����̰��Ϸ���
					{
						g_Player.setTime(CurTime, 2);
						//OldTime = CurTime;  //��� �̵��� �ð��� ó���ð����� ����.
						if (g_Player.getXY(1) - 1 > 1) //�� ���� �ƴϸ�
						{
							g_Player.setXY(1,(g_Player.getXY(1) - 1));
							//g_Player.nX--;  //��ǥ ����

							PlayerCollision();  //
							break;
						}
					}
				}
				break;

			case 77: // ������
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

			case 's': // �̻��� �߻�
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
			// Note: ���� �̻��ϰ� ���ΰ��� �浹
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

			// Note: ���� �̻��ϰ� ���ΰ� �̻��� �浹
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


// ���� �̻��� ���忡�� ���ΰ��� ���ΰ� �̻��� �浹 üũ

int C_So_Manage::Collision(int nX1, int nY1, int nX2, int nY2, int nX3, int nY3, int nX4, int nY4)
{
	if (nX1 <= nX4 && nX2 >= nX3 && nY1 <= nY4 && nY2 >= nY3)
		return 1;  // �浹
	else
		return 0;  // ���浹
}

void C_So_Manage::PlayerCollision()
{
	int i, j;

	for( i = 0 ; i < g_nEnemyIndex ; i++ )  
	{
		if( g_Enemy[i].getLi())
		{
			// �� ĳ���Ϳ� �浹 üũ
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

	// �� �̻��ϰ� �浹 üũ
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
//// ���ΰ� �̻������忡�� ������ ���� �̻��� �浹
void C_So_Manage::PlayerMissileBossCollision()
{
	int i, j;	
	
	for( i = 0 ; i < HERO_MISSILE ; i++ )
	{
		if( g_PlayerMissile[i].getLi() )
		{
		    // ���ΰ� �̻��ϰ� ���� �浹üũ
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

			// ���ΰ� �̻��ϰ� ���� �̻��ϰ� �浹 üũ
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
	case READY: //�� ��Ȳ�� ready�� 3�� �� running
		if (CurTime - g_UpdateOldTime > 3000)  // 3��->���ð��� 3��
		{
			g_nGameState = RUNNING;  //�������� �ٲ�.
			g_GameStartTime = CurTime;	//������ �ð��� ���۽ð�.

		}
		break;

	case RUNNING:
		// Note: ���� ( ���� �ð��� �����ʹ� ������������ ���ĵǾ� �ִ� )
		for (i = g_nEnemyIndex; i < g_nEnemyCount; i++)
		{
			if (g_Enemy[i].getLi() == 0)
			{
				if (CurTime - g_StartTime >= g_Enemy[i].getAp())  //�Լ� ������ �ð�-�ʱ�ȭ�� ������->INIT���� uPDATE���� �ɸ� �ð�. >= ���� ������ �ð�.-> ������ �ð����� ������ �ð��� ���� ����
				{
					g_Enemy[i].setLi(1); //�� ����.
					g_nEnemyIndex++;      //->++i
				}
				else {			//�ð� ������ ����.				
					break;
				}
			}
		}

		// Note:  �� ĳ���� �̵� �� �̻��� �߻� ���� 
		for (i = 0; i < g_nEnemyIndex; i++) //��� ���� ��� ���峪.
		{
			if (g_Enemy[i].getLi())
			{
				// Note: �� ĳ���� ������Ʈ
				if (CurTime - g_Enemy[i].getTime(3) > g_Enemy[i].getTime(1)) //�Լ� ������ �ð�-�� ���� �ʱ�ȭ ���� �ð�>���� �����̴� �ð�==������ �ð����� ���� ������ �۵�.
				{
					g_Enemy[i].setTime(CurTime,3);  //update
					g_Enemy[i].set_nPat(g_Enemy[i].get_nPat(2) + 1, 2);
					//	nPatStep++;  //0���� ������.

											// ���� ���� �ε����� ����--->���� ������
					if (g_Enemy[i].get_nPat(2) == g_PatInfo[g_Enemy[i].get_nPat(1)].pPat[g_Enemy[i].get_nPat(3)].nStep) //���� ��ġ�Ұܿ�
					{    //������ ��ŭ �ִ��� ���°ǵ� ���������� nPatStep�� �����ϰ� ������ ����ŭ �����Դٸ� �����.
						g_Enemy[i].set_nPat(g_Enemy[i].get_nPat(3) + 1, 3);
						//g_Enemy[i].nPatIndex++;
						if (g_Enemy[i].get_nPat(3) == g_PatInfo[g_Enemy[i].get_nPat(1)].nCount) //������->a�� 3�� ����� 3���� �� ������
						{
							g_Enemy[i].setLi(0);  //������ ǥ��
							g_nDeadEnemy++;     //���� ��� ����

							if (g_nDeadEnemy == g_nEnemyCount)//�÷��̾ �¸�.
							{
								g_nGameState = PLAYER_TALK; //�÷��̾ �¸�.
								g_UpdateOldTime = CurTime;  //������Ʈ �������� �� �ð� ����.
								return;
							}
							else
								continue;  // �����̵��� ����Ǿ� ���� ĳ���͸� ����
						}
						else {
							g_Enemy[i].setTime(g_PatInfo[g_Enemy[i].get_nPat(1)].pPat[g_Enemy[i].get_nPat(3)].MoveTime,1);  //���� �ʾҴٸ� ���Ͽ��� ������ �ð����� ����
							g_Enemy[i].set_nPat(0,2); //�ٽ� �ʱ�ȭ.
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

				// Note: �� �̻��� �߻� �ϴ� �κ� ( ĳ���Ͱ� ��� �ִ� ���� ���� )
				if (CurTime - g_Enemy[i].M_getTime(2) > g_Enemy[i].M_getTime(1))  //�Լ� �����ѽð�-���� �̻��� �� �ð�>�̻��� �߻� �ð�  ==�̻��� �߻� �Ⱓ �����ϰ�
				{
					g_Enemy[i].M_setTime(CurTime,2);  //�� �ð��� �������� ��ü
					for (j = 0; j < 100; j++)
					{
						if (g_EnemyMissile[j].getLi() == 0)  //�̻��� ������ ��
						{
							g_EnemyMissile[j].setXY(1,g_Enemy[i].getXY(1));
							g_EnemyMissile[j].setXY(2,g_Enemy[i].getXY(2)+1);
							g_EnemyMissile[j].setLi(1);
							g_EnemyMissile[j].setTime(rand() % 100 + 80, 1);
							//g_EnemyMissile[j].nLife = 1; //�̻��� ���ܼ� ����� �ο�
						//	g_EnemyMissile[j].MoveTime = rand() % 100 + 80;
							g_EnemyMissile[j].setTime(CurTime,3);
							break;
						}
					}
				}
			}
		}

		//------------------ �� ĳ���� �̵� �� ------------------------------

		for (i = 0; i < g_nEnemyIndex; i++)
		{
			if (g_Enemy[i].getLi())
			{
				// Note: ��ĳ���Ϳ� ���ΰ��� �浹 üũ
				if (Collision(g_Enemy[i].getXY(1), g_Enemy[i].getXY(2), g_Enemy[i].getXY(1) + 1, g_Enemy[i].getXY(2),
					g_Player.getXY(1), g_Player.getXY(2), g_Player.getXY(1) + 4, g_Player.getXY(2)))
				{
					g_Enemy[i].setLi(0);
					g_nGrade += 10; // ����
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

				// Note: �� ĳ���Ϳ� ���ΰ� �̻����� �浹 üũ
				for (j = 0; j < 10; j++)
				{
					if (g_PlayerMissile[j].getLi()) 
					{
						if (Collision(g_Enemy[i].getXY(1), g_Enemy[i].getXY(2), g_Enemy[i].getXY(1) + 1, g_Enemy[i].getXY(2),
							g_PlayerMissile[j].getXY(1), g_PlayerMissile[j].getXY(2), g_PlayerMissile[j].getXY(1), g_PlayerMissile[j].getXY(2)))
						{
							g_Enemy[i].setLi(0);
							g_PlayerMissile[j].setLi(0);
							g_nGrade += 10; // ����
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


		// Note: �� �̻��� �̵� 
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


		// Note:  �� �̻��ϰ� ���ΰ�, ���ΰ� �̻��� �浹 üũ 
		for (i = 0; i < 100; i++)
		{
			if (g_EnemyMissile[i].getLi())
			{
				// Note: �浹 üũ => �� �̻��ϰ� ���ΰ� 
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

				// Note: �浹 üũ => �� �̻��ϰ� ���ΰ� �̻���
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

		//----------------���ΰ� �̻��� �̵� ----------------------------------------------------
		for (i = 0; i < HERO_MISSILE; i++)
		{
			if (g_PlayerMissile[i].getLi())
			{   // ���ΰ� �̻��� �̵�
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

		//------------------���ΰ� �̻��� �� �� ĳ���� �� �� �̻��� �浹 ---------------------

		for (i = 0; i < HERO_MISSILE; i++)
		{
			if (g_PlayerMissile[i].getLi())
			{
				// Note: ���ΰ� �̻��ϰ� �� ĳ������ �浹 üũ
				for (j = 0; j < g_nEnemyIndex; j++)
				{
					if (g_Enemy[j].getLi())
					{
						if (Collision(g_PlayerMissile[i].getXY(1), g_PlayerMissile[i].getXY(2), g_PlayerMissile[i].getXY(1), g_PlayerMissile[i].getXY(2),
							g_Enemy[j].getXY(1), g_Enemy[j].getXY(2), g_Enemy[j].getXY(1) + 1, g_Enemy[j].getXY(2)))
						{
							g_PlayerMissile[i].setLi(0);
							g_Enemy[j].setLi(0);
							g_nGrade += 10; // ����
							g_nDeadEnemy++; // �� ĳ���� �Ѱ� �Ҹ�

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

		// Note: ���ΰ� �̻��ϰ� ���̻����� �浹 üũ
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
		if (CurTime - g_UpdateOldTime > 3000)  // 3��
		{
			g_nGameState = BOSS_TALK;
			g_UpdateOldTime = CurTime;
		}
		break;
	case BOSS_TALK:
		if (CurTime - g_UpdateOldTime > 3000)  // 3��
		{
			g_nGameState = BOSS_WAR;
			g_UpdateOldTime = CurTime;
			memset(g_PlayerMissile, 0, sizeof(C_MainObject)*HERO_MISSILE);
		}
		break;

	case BOSS_WAR:
		// Note: ������ �̵�
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
						g_Boss.set_nPat(g_Boss.get_nPat(1) % g_nBossPatCount,1); // ���� �ݺ�
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

				for (i = 0; i < 30; i++)    // 30���� �̻����� �˻��ϴ� �κ�
				{
					if (g_sBossMissile.get(i).getLi())/*g_sBossMissile.get(i).setN(0,3)*/
					{
						g_sBossMissile.get(i).setXY(1,g_Boss.getXY(1) + 6);
						g_sBossMissile.get(i).setXY(2,g_Boss.getXY(2) + 2);
						g_sBossMissile.get(i).setLi(1);
						g_sBossMissile.get(i).set_nDir(DIRECT(DOWN_LEFT - nFireMissileCount)); // ���⼳��: 5, 4, 3
						g_sBossMissile.get(i).setTime(CurTime,3);
						nFireMissileCount++;

						if (nFireMissileCount > 2) // 3�� �̻� �߻� ���� ���ϰ� �ϴ� �κ�
							break;
					}
				}
			}
		}

		// Note: ������ ���ΰ� �̻����� �浹 üũ (1)
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

		// Note: ���� �̻��� �̵�
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

				// Note: ��� ���� �浹 üũ
				if (g_sBossMissile.get(i).getXY(1)< 2 || g_sBossMissile.get(i).getXY(1)+ 1 > 59 ||
					g_sBossMissile.get(i).getXY(2) > 22)
				{
					g_sBossMissile.get(i).setLi(0);
				}
			}
		}

		 BossMissileCollsion();

		//------------------------ ���ΰ��� ���� �̻��� �浹 üũ �κ�   ---------------------------//
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

		//------------------------ ���ΰ� �̻��� �̵�  --------------------------//
		for (i = 0; i < HERO_MISSILE; i++)
		{
			if (g_PlayerMissile[i].getLi())
			{
				if (CurTime - g_PlayerMissile[i].getTime(2) > MISSILE_MOVE_TIME)
				{
					g_PlayerMissile[i].setTime(CurTime,2);
					g_PlayerMissile[i].setXY(2,g_PlayerMissile[i].getXY(2)-1);//nY--;

					// ��� ���� �浹 ó��
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
			g_nTotalGrade += 100; // ������ �̰����Ƿ�

		}
		break;

	case SUCCESS:
		if (CurTime - g_UpdateOldTime > 3000)
		{
			g_UpdateOldTime = CurTime;
			++g_nStage;
			g_nStage = g_nStage % 3; 	// ���������� 3���ۿ� ���.	
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
	case INIT :  //�ð����� 
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
					   ScreenPrint( g_EnemyMissile[i].getXY(1), g_EnemyMissile[i].getXY(2), "��" );
					}
				 }

				 for( i = 0 ; i < g_nEnemyIndex ; i++ )
				 {
					if( g_Enemy[i].getXY(2) )  //���� �����..
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
				 sprintf( string, "���ΰ� HP : %d", g_Player.getLi() );
				 ScreenPrint( 63, 5, string );
				 sprintf( string, "����: %d", g_nGrade );
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
				ScreenPrint( g_Boss.getXY(1), g_Boss.getXY(2)  , " /| �� // �� |��"); 
				ScreenPrint( g_Boss.getXY(1), g_Boss.getXY(2)+1, "//q ��    �� p��");
				ScreenPrint( g_Boss.getXY(1), g_Boss.getXY(2)+2, "  (����������)"); 				

				for( i = 0 ; i < 30 ; i++ )
				{
					if( g_sBossMissile.get(i).getLi() )
					{
						ScreenPrint( g_sBossMissile.get(i).getXY(1), g_sBossMissile.get(i).getXY(2), "��" );
					}
				}

				// ���ΰ� �̻��� ������
				for( i = 0 ; i < HERO_MISSILE ; i++ )
				{
					if( g_PlayerMissile[i].getLi() )
						ScreenPrint( g_PlayerMissile[i].getXY(1), g_PlayerMissile[i].getXY(2), "$" );
				}

				ScreenPrint( g_Player.getXY(1), g_Player.getXY(2), "#-@-#" );
				
				sprintf( string, "���� HP : %d", g_Boss.getLi() );
				ScreenPrint( 63, 3, string );
				sprintf( string, "���ΰ� HP : %d", g_Player.getLi() );
				ScreenPrint( 63, 5, string );
				break;

	 case SUCCESS :				
		 screen->SuccessScreen();
				sprintf( string, "%d", g_nStage + 1 );
				ScreenPrint( 19, 7, string );
				sprintf( string, "����: %d", g_nTotalGrade );
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