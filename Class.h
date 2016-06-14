#pragma once
#include"base.h"
class s
{
public:
	virtual int Init()=0;
	virtual int Run() = 0;
};
//��Ӿ��� Ŭ����
class C_Pat
{
	//�ܺο��� ���� �ޱ� ������
public:
	DIRECT nDirect;    // �̵� ����
	int nStep;         // ����    ���Ϸ� �ҷ��� ��. ������ ��ŭ ������ ���� ���°�.
	clock_t MoveTime;  // �̵� �ð� ����
	int nDist;         // �̵� �Ÿ�
};
class C_PatInfo
{
public:
	//�ܺο��� ���� �ޱ� ������
	int nCount;    //����
	int nX0, nY0;  //x,y,��ǥ
	C_Pat *pPat;   //pat�� ������
	
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
//�θ�Ŭ����
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
	int nLife;    //�����
public:
	void setLi(int n);
	int getLi();
};
class C_nPat //���ο� ���� �߰�
{
private:
	int nPatType;
	int nPatStep;
	int nPatIndex;
public:
	void set_nPat(int n,int i);  //private������ ����
	int get_nPat(int i);		 //private ������ ����
};
class C_Clock
{
private:
	clock_t MoveTime;  //�̵� �ð� ����
	clock_t OldTime;   //���� �̵� �ð�
	clock_t OldMoveTime;  //
	clock_t MissileFireTime;  //
	clock_t MissileOldTime;  //
	clock_t FireTime;   //�̻��� �߻� �ð� ����
	clock_t OldFireTime;  //���� �̻��� �߻� �ð�
public:
	void M_setTime(int t, int i);  //�̵��ð��� �����Ѵ�.
	void setTime(int t, int i);    //�ð����� �����´�.
	void F_setTime(int t, int i);  //�߻� �ð����� �����´�
	int M_getTime( int i);        //�̻��� �ð��� �����´�.
	int getTime(int i);           //�ð����� �����´�.
	int F_getTime( int i);        //�߻� �ð����� �����´�.
};
//sogo�ڽ�Ŭ����
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
//snake�ڽ� Ŭ����
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

