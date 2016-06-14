#pragma once
#include"base.h"
class s
{
public:
	virtual int Init()=0;
	virtual int Run() = 0;
};
//상속없는 클래스
class C_Pat
{
	//외부에서 값을 받기 떄문에
public:
	DIRECT nDirect;    // 이동 방향
	int nStep;         // 스텝    파일로 불러온 값. 위에서 얼마큼 떨어진 건지 보는것.
	clock_t MoveTime;  // 이동 시간 간격
	int nDist;         // 이동 거리
};
class C_PatInfo
{
public:
	//외부에서 값을 받기 때문에
	int nCount;    //개수
	int nX0, nY0;  //x,y,좌표
	C_Pat *pPat;   //pat의 포인터
	
};
class C_Screen 
{
public:
	void InitScreen();
	void ReadyScreen();
	void SuccessScreen();
	void FailureScreen();
	void PlayerTalk();
	void BossTalk();
	void ScreenLine();
	void ResultScreen();
};
class C_Op
{
public:
	int gotoxy(int x, int y);
	void SetColor(int color);
};
//부모클래스
class C_XY
{
private:
	int x, y;
public:
	void setXY(int xy, int n);
	int getXY(int xy);

};
class C_Normal :public C_XY
{
private:
	int nLife;    //생명력
public:
	void setLi(int n);
	int getLi();
};
class C_nPat //새로운 패턴 추가
{
private:
	int nPatType;
	int nPatStep;
	int nPatIndex;
public:
	void set_nPat(int n,int i);  //private변수값 설정
	int get_nPat(int i);		 //private 변수값 보냄
};
class C_Clock
{
private:
	clock_t MoveTime;  //이동 시간 간격
	clock_t OldTime;   //이전 이동 시각
	clock_t OldMoveTime;  //
	clock_t MissileFireTime;  //
	clock_t MissileOldTime;  //
	clock_t FireTime;   //미사일 발사 시간 간격
	clock_t OldFireTime;  //이전 미사일 발사 시각
public:
	void M_setTime(int t, int i);  //이동시간을 설정한다.
	void setTime(int t, int i);    //시간값을 설정온다.
	void F_setTime(int t, int i);  //발사 시간값을 가져온다
	int M_getTime( int i);        //미사일 시간을 가져온다.
	int getTime(int i);           //시간값을 가져온다.
	int F_getTime( int i);        //발사 시간값을 가져온다.
};
//sogo자식클래스
class C_MainObject :public C_Normal, public C_Clock
{


};
class C_Enemy :public C_nPat, public C_Normal, public C_Clock
{
private:
	int nType;
	clock_t AppearTime;
public:
	void setAp(int t);
	int getAp();
	C_Enemy();
};
class C_Boss :public C_nPat, public C_Normal, public C_Clock
{
public:

};
class C_BossMissile:public C_Normal,public C_Clock
{
private:
	DIRECT nDirect;
public:
	void set_nDir(DIRECT n);
	DIRECT get_nDir()
	{
		return nDirect;
	}
};
class C_BossMissileInfo: public C_Clock  
{
private:
	C_BossMissile sMissile[30];
public:
	C_BossMissile get(int i){return sMissile[i];}
	C_BossMissile *get(){return sMissile;}
	
};
//snake자식 클래스
class C_Body :public C_XY
{
private:
	C_Body*next;
public:
	void p_set(C_Body *p);
	C_Body* p_get();
};
class C_Snake :public C_Body
{
	int speed, length;
	int direction, last_direction;
	int score;
public:
	void set_op(int n, int sl);
	void set_di(int n, int dl);
	int get_op(int sl);
	int get_di(int dl);
	int move_snake(int dir);
	int make_tail(int dir);
};
class C_Show :public C_Op
{
public:
	int show_gameover();
	int show_game_frame(int y);

};

