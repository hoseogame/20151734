#include"Snakemanage.h"

C_Body *snake_head;
C_Body *snake_tail;
C_Snake snake;
C_XY food;
C_Show show;
C_XY box;

char screen[20][35];

int C_Sn_Manage::Init()
{
	srand(time(NULL));

	snake.set_op(25,1);
	snake.set_op(1,2);

	snake.set_di(3, 1);
	snake.set_di(snake.get_di(1),2);

	box.setXY(2, 1);
	box.setXY(2, 2);

	//���ʱⰪ �Է�
	snake_tail = new C_Body;
	snake_tail->setXY(1, 5);
	snake_tail->setXY(2, 3);
	snake_head = snake_tail;

	snake_tail = new C_Body;
	snake_tail->setXY(1,4);
	snake_tail->setXY(2,3);
	snake_tail->p_set(NULL);
	snake_head->p_set(snake_tail);
	return 0;
}
int C_Sn_Manage::Run()
{
	int i;
	char keytemp;
	int return_tmp = 0;

	C_Op::gotoxy(77, 23);
	printf(" \b");
	//abs.getxy(2);
	show.show_game_frame(box.getXY(2));
		//getxy(2));
	make_food();
	for (i = 1; 1; i++)
	{
		if (kbhit())
		{
			keytemp = getche();

			if (keytemp == EXT_KEY)
			{
				printf("\b");
				keytemp = getche();
				switch (keytemp)
				{
				case KEY_UP:
					if (snake.get_di(2) != 4)
						snake.set_di(1, 1);
						//direction = 1;

					break;
				case KEY_LEFT:
					if (snake.get_di(2) != 3)
						snake.set_di(2, 1);
						//direction = 2;
					break;
				case KEY_RIGHT:
					if (snake.get_di(2) != 2)
						snake.set_di(3, 1);

					break;
				case KEY_DOWN:
					if (snake.get_di(2) != 1)
						snake.set_di(4, 1);
					break;
				}
			}
		}
		if (i%(snake.get_op(1)) == 0)
		{
			if (return_tmp == 2)	//������ �þ
			{
				return_tmp = make_tail(snake.get_di(1));
			}
			else {
				fflush(stdin);

				return_tmp = move_snake(snake.get_di(1));	//�ܼ��� �̵�
			}
			snake.set_di(2, snake.get_di(1));
			//last_direction = direction;
			if (return_tmp == 1)		//�浹�Ҷ�
			{
				show.show_gameover();
				free_all();
				return 1;
				//break;
			}
			if (return_tmp == 2)		//���̸� �Ծ�����
			{
				if (snake.get_op(2) % 5 == 0)
					snake.set_op(1, (snake.get_op(1) - 1));
					//speed--;

					if (snake.get_op(1) < 1)
						snake.set_op(1, 1);
					//speed = 1;

					snake.set_op(2, snake.get_op(2) + 1);
				//length++;
				make_food();
				i = 1;	//���ǵ���� ���� �ʱ�ȭ
			}
		}
		C_Op::SetColor(BLACK);
		C_Op::gotoxy(77, 23);
		Sleep(10);
		C_Op::SetColor(BLACK);
		C_Op::gotoxy(77, 23);
	}
}
int C_Sn_Manage::make_food()
{
	
		food.setXY(2,rand() % 20);
		food.setXY(1,rand() % 35);
		while (strike_check(food.getXY(1), food.getXY(2)) == 1)
		{
			food.setXY(2,rand() % 20);
			food.setXY(1,rand() % 35);
		}

		C_Op::gotoxy(77, 23);		//ȭ�� ������ ��������
		fflush(stdout);

		C_Op::SetColor(RED);
		C_Op::gotoxy(food.getXY(1) * 2 + box.getXY(1), food.getXY(2) + box.getXY(2));
		printf("��");
		C_Op::SetColor(BLACK);
		return 0;
	
}
int C_Sn_Manage::strike_check(int x, int y)
{
	C_Body *tmp_body;
	if (x<0 || x>34 || y<0 || y>19)		//�Ӹ��� ���� �δ�ħ
		return 1;
	tmp_body = snake_head;
	while (tmp_body != NULL)
	{
		if (tmp_body->getXY(1)== x&& tmp_body->getXY(2) == y)
			return 1;
		tmp_body=tmp_body->p_get();
	}
	if (food.getXY(1) == x && food.getXY(2) == y)	//���̸� ������
		return 2;
	return 0;
}
int C_Sn_Manage::make_tail(int dir)
{
	int tmp_x, tmp_y;
	int i;
	C_Body *tmp_body;

	tmp_x = snake_head->getXY(1);
	tmp_y = snake_head->getXY(2);
	switch (snake.get_di(1))
	{
	case 1:
		tmp_y--;
		break;
	case 2:
		tmp_x--;
		break;
	case 3:
		tmp_x++;
		break;
	case 4:
		tmp_y++;
		break;
	}
	i = strike_check(tmp_x, tmp_y);
	if (i == 1)	//�浹�Ҷ�
	{
		return 1;
	}





	C_Op::gotoxy(77, 23);		//ȭ�� ������ ��������
	fflush(stdout);

	C_Op::SetColor(GRAY);
	C_Op::gotoxy((snake_head->getXY(1) * 2) + box.getXY(1), snake_head->getXY(2) + box.getXY(2));	//�̵����� �Ӹ���� �ܻ������
	printf("��");
	C_Op::gotoxy(tmp_x * 2 + box.getXY(1), tmp_y + box.getXY(2));	//�Ӹ���� ����Ʈ�ϱ�
	printf("��");
	C_Op::gotoxy(77, 23);		//ȭ�� ������ ��������
	C_Op::SetColor(BLACK);


	tmp_body = new C_Body;	//�Ӹ��κ��� ���ο� ��常���
	tmp_body->setXY(tmp_x,1);
	tmp_body->setXY(tmp_y, 2);
	tmp_body->p_set(snake_head);
	//tmp_body->next = snake_head;
	snake_head = tmp_body;
	delete tmp_body;


	return i;
}
int C_Sn_Manage::move_snake(int dir)
{
	int tmp_x, tmp_y;
	int i;
	C_Body *tmp_body;


	tmp_x = snake_head->getXY(1);
	tmp_y = snake_head->getXY(2);
	switch (snake.get_di(1))
	{
	case 1:
		tmp_y--;
		break;
	case 2:
		tmp_x--;
		break;
	case 3:
		tmp_x++;
		break;
	case 4:
		tmp_y++;
		break;
	}
	i = strike_check(tmp_x, tmp_y);
	if (i == 1)	//�浹�Ҷ�

	{
		return 1;
	}

	C_Op::gotoxy((snake_tail->getXY(1) * 2) + box.getXY(1), snake_tail->getXY(2) + box.getXY(2));	//�̵����� �ܻ������
	printf("  ");



	C_Op::gotoxy(77, 23);		//ȭ�� ������ ��������
	fflush(stdout);

	C_Op::SetColor(GRAY);
	C_Op::gotoxy((snake_head->getXY(1) * 2) + box.getXY(1), snake_head->getXY(2) + box.getXY(2));	//�̵����� �Ӹ���� �ܻ������
	printf("��");
	C_Op::gotoxy(tmp_x * 2 + box.getXY(1), tmp_y + box.getXY(2));	//�Ӹ���� ����Ʈ�ϱ�
	printf("��");
	C_Op::gotoxy(77, 23);		//ȭ�� ������ ��������
	C_Op::SetColor(BLACK);


	tmp_body = new C_Body;//(struct BODY *)malloc(sizeof(struct BODY));	//�Ӹ��κ��� ���ο� ��常���
	tmp_body->setXY(tmp_x,1);
	tmp_body->setXY(tmp_y,2);
	tmp_body->p_set(snake_head);
	snake_head = tmp_body;


	while (tmp_body->p_get() != snake_tail)		//������� �����
		tmp_body = tmp_body->p_get();
	//tmp_body = tmp_body->next;

	tmp_body->p_set(NULL);
	free(snake_tail);
	snake_tail = tmp_body;
	//delete tmp_body;
	return i;
}
int C_Sn_Manage::free_all()
{
	C_Body *tmp1, *tmp2;

	tmp1 = snake_head;
	tmp2 = snake_head->p_get();
	while (tmp2 != NULL)
	{
		delete tmp1;
		tmp1 = tmp2;
		tmp2 = tmp2->p_get();
	}
	
	return 0;
}

