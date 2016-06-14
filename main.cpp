#pragma once
#include"sogomanage.h"
#include"Snakemanage.h"
int select();
int main(void)
{
	int ch = select();
	C_Sn_Manage *Snake_manage = new C_Sn_Manage;
	C_So_Manage *Sogo_manage = new C_So_Manage;
	if (ch == 1)
	{
		bool end = false;
		Snake_manage->Init();
		while (end != true)
		{
			Snake_manage->Run();
			if (Snake_manage->Init() == 1)
				end = true;
		}
		delete Snake_manage;
	}

	if (ch == 2)
	{
		Sogo_manage->Init();
		Sogo_manage->Run();
		Sogo_manage->Release();
		ScreenRelease();
		delete Sogo_manage;
	}
	return 0;
}
int select()
{
	int se = 0;
	std::cout<<1.<<"¹ì°ÔÀÓ"<<endl;
	std::cout << 2. << "SoGo°ÔÀÓ" << endl;
	cin >> se;
	return se;
}