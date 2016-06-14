#pragma once

#include"Base.h"
#include"Class.h"

class C_Sn_Manage :public C_Op, public s
{
public:
	
	virtual int Init();
	virtual int Run();
	int make_food();
	int strike_check(int x, int y);
	int make_tail(int dir);
	int move_snake(int dir);
	int free_all();

};
