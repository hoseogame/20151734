#pragma once
#include <iostream>
//#include <windows.h>
#include <conio.h>
#include <ctime>
#include <cstring>
#include "stdafx.h"
#include "Screen.h"
#include<stdlib.h>

using namespace std;

#define HERO_MISSILE  5           
#define MISSILE_MOVE_TIME 100   
#define EXT_KEY			0xffffffe0	//확장키 인식값 
#define KEY_LEFT		0x4b
#define KEY_RIGHT		0x4d
#define KEY_UP			0x48
#define KEY_DOWN		0x50

typedef enum _DIRECT {
	UP, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT,
	LEFT, UP_LEFT
} DIRECT;
typedef enum _GAME_STATE {
	INIT, READY, RUNNING, PLAYER_TALK, BOSS_TALK, BOSS_WAR,
	STOP, SUCCESS, FAILED, RESULT
} GAME_STATE;
typedef struct _STAGE_INFO
{
	int nEnemyCount;
	int nBossLife;//보스 생명력
} STAGE_INFO;

enum {
	BLACK,      /*  0 : 까망 */
	DARK_BLUE,    /*  1 : 어두운 파랑 */
	DARK_GREEN,    /*  2 : 어두운 초록 */
	DARK_SKY_BLUE,  /*  3 : 어두운 하늘 */
	DARK_RED,    /*  4 : 어두운 빨강 */
	DARK_VOILET,  /*  5 : 어두운 보라 */
	DARK_YELLOW,  /*  6 : 어두운 노랑 */
	GRAY,      /*  7 : 회색 */
	DARK_GRAY,    /*  8 : 어두운 회색 */
	BLUE,      /*  9 : 파랑 */
	GREEN,      /* 10 : 초록 */
	SKY_BLUE,    /* 11 : 하늘 */
	RED,      /* 12 : 빨강 */
	VOILET,      /* 13 : 보라 */
	YELLOW,      /* 14 : 노랑 */
	WHITE,      /* 15 : 하양 */
};