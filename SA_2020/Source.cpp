// Space Action

#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<vector>
#include<ctime>

//

using namespace std;
HWND win = GetConsoleWindow();
HDC hdc = GetDC(win);

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
COORD coord;

COLORREF color_black = RGB(0, 0, 0);
COLORREF color_red = RGB(255, 0, 0);
COLORREF color_green = RGB(0, 255, 0);
COLORREF color_blue = RGB(0, 0, 255);
COLORREF color_white = RGB(255, 255, 255);
COLORREF color_yellow = RGB(255, 255, 0);
COLORREF color_cyan = RGB(0, 255, 255);
COLORREF color_magenta = RGB(255, 0, 255);

HBRUSH black = CreateSolidBrush(color_black);
HBRUSH red = CreateSolidBrush(color_red);
HBRUSH green = CreateSolidBrush(color_green);
HBRUSH blue = CreateSolidBrush(color_blue);
HBRUSH white = CreateSolidBrush(color_white);
HBRUSH yellow = CreateSolidBrush(color_yellow);
HBRUSH cyan = CreateSolidBrush(color_cyan);
HBRUSH magenta = CreateSolidBrush(color_magenta);

int a, b, c, d, e, f, i, j, size_1, size_2, direct, lvl = 0;

float tngns, cosns, sinus, p, q, t;

const int
width = GetSystemMetrics(SM_CXSCREEN) - 20,
height = GetSystemMetrics(SM_CYSCREEN) - 70,
rocket_depth = 10,
shell_depth = 5,
bonus_depth = 8,
rocket_speed = 5,
shell_speed = 20,
bonus_speed = 2,
expl_rad_MyR = 60,
expl_rad_EnR = 30,
expl_rad_Bon = 10;

int rar_X = 20;
int rar_Y = 10;
int Arr_X[7] = { -3 * rar_X, -2 * rar_X, -rar_X, 0, rar_X, 2 * rar_X, 3 * rar_X };
int Arr_Y[7] = { rar_Y, -rar_Y, -2 * rar_Y, -3 * rar_Y, -2 * rar_Y, -rar_Y, rar_Y };
int Arr_DX[7] = { -16, -12, -6, 0, 6, 12, 16 };
int Arr_DY[7] = { -12, -16, -19, -20, -19, -16, -12 };

bool
GameOver = false;

class XY
{
public:
	XY()
	{
		X = 0;
		Y = 0;
	}
	XY(int X, int Y)
	{
		this->X = X;
		this->Y = Y;
	}
	int X, Y;
	friend XY operator + (XY A, XY B)
	{
		return XY(A.X + B.X, A.Y + B.Y);
	}
	XY operator += (XY A)
	{
		return *this = *this + A;
	}
};

// тхцспш
class Figure
{
public:
	virtual void draw() = 0;
	virtual void logic() = 0;
};


// йпюдпюрш
class Square : public Figure
{
public:
	XY pos, speed;
	int depth = 20; // рнкыхмю
	bool
		destroy = false, // смхврнфем
		lim_left = false, // кебши йпюи
		lim_right = false, // опюбши йпюи
		lim_up = false, // бепумхи йпюи
		lim_down = false; // мхфмхи йпюи
	HBRUSH brush = white; // жбер

	// дбхфемхе
	void draw()
	{
		pos += speed;
		SelectObject(hdc, brush);
		Rectangle(hdc, pos.X - depth, pos.Y - depth, pos.X + depth, pos.Y + depth);
	}

	// йпюи йюпрш
	void lim()
	{
		lim_left = pos.X < 0;
		lim_right = pos.X > width;
		lim_up = pos.Y < 0;
		lim_down = pos.Y > height;
	}

	// онбедемхе мю йпюч йюпрш
	virtual void limit() = 0;

	// наыее онбедемхе бяеу йбюдпюрнб
	void common_square()
	{
		limit();
		draw();
		lim();
	}
};


// пюйерш
class Rocket : public Square
{
public:
	// хяйкчвемхе бшкерю
	void limit()
	{
		if (lim_left && speed.X < 0 || lim_right && speed.X > 0)	speed.X = 0;
		if (lim_up && speed.Y < 0 || lim_down && speed.Y > 0)		speed.Y = 0;
	}

	// наыее онбедемхе бяеу пюйер
	void common_rocket()
	{
		common_square();
	}
};


// лнъ пюйерю
class MyRocket : public Rocket
{
public:
	bool
		restart, // пеярюпр
		fire, // нцнмэ
		arm; // ярхкэ нпсфхъ
	int
		life, // нярюкняэ фхгмеи
		power, // нцмебюъ лныэ
		score, // смхврнфемн бпюцнб
		counter; // яйнпнярпекэмнярэ

	MyRocket() :
		restart(true),
		fire(false),
		arm(false),
		life(3),
		power(1),
		score(0),
		counter(0)
	{}

	// сопюбкемхе
	void input()
	{
		speed = XY();
		fire = false;

		if (GetAsyncKeyState(VK_LEFT))	speed.X -= rocket_speed;
		if (GetAsyncKeyState(VK_RIGHT))	speed.X += rocket_speed;
		if (GetAsyncKeyState(VK_UP))	speed.Y -= rocket_speed;
		if (GetAsyncKeyState(VK_DOWN))	speed.Y += rocket_speed;
		if (GetAsyncKeyState(VK_SPACE))
		{
			counter++;
			if (counter == 10)
			{
				counter = 0;
				fire = true;
			}
		}
		if (_kbhit()) switch (_getch())
			{
			case (VK_ESCAPE):
				GameOver = true;
				break;
			case (VK_TAB):
				system("pause");
				system("cls");
				break;
			default:
				break;
			}
	}

	// хмрептеия
	void output()
	{
		coord.X = 0;
		coord.Y = 50;
		SetConsoleCursorPosition(handle, coord);

		cout << " LIFE = " << life << " \n";
		cout << " POWER = " << power << " \n";
		if (arm)	cout << " ARM = GREEN\n";
		else		cout << " ARM = RED  \n";
		cout << " SCORE = " << score << "\n\n";
		cout << " LEVEL = " << lvl << "\n";
	}

	// онбедемхе лнеи пюйерш
	void logic()
	{
		common_rocket();
		input();
		output();
	}

	// еякх бдпсц лнч пюйерс смхврнфхкх
	void regress()
	{
		life--;
		if (power != 1)
			power--;
		pos.X = width / 2;
		pos.Y = 3 * height / 4;
	}

	// еякх бдпсц ъ смхврнфхк бпюфеяйсч пюйерс
	void progress()
	{
		score++;
		if (score % 10 == 0)
			life++;
	}

	// еякх бдпсц ъ онилюк анмся
	void bonus(bool col)
	{
		if (col && arm || !col && !arm)	power++;
		else if (arm)	arm = false;
		else			arm = true;
	}
};


// бпюфеяйхе пюйерш
class EnemyRocket : public Rocket
{
public:
	int
		counter, // люмебпхпнбюмхе
		count_round, // яйнпнярпекэмнярэ
		sum; // ямюпъднб б нвепедх
	bool
		fire, // нцнмэ
		end_round; // оепегюпъдйю

	EnemyRocket() :
		counter(0),
		count_round(0),
		sum(0),
		fire(false),
		end_round(true)
	{}

	// меопедяйюгселнярэ
	void behavior()
	{
		counter++;
		fire = false;
		if (counter == 50)
		{
			counter = 0;
			if (rand() % 2)
				switch (rand() % 8)
				{
				case 0: speed = XY(-rocket_speed, 0);				break;
				case 1: speed = XY(rocket_speed, 0);				break;
				case 2: speed = XY(0, -rocket_speed);				break;
				case 3: speed = XY(0, rocket_speed);				break;
				case 4: speed = XY(-rocket_speed, -rocket_speed);	break;
				case 5: speed = XY(-rocket_speed, rocket_speed);	break;
				case 6: speed = XY(rocket_speed, -rocket_speed);	break;
				case 7: speed = XY(rocket_speed, rocket_speed);		break;
				}
			end_round = rand() % 2;
		}

		if (lim_left)
			switch (rand() % 3)
			{
			case 0: speed = XY(rocket_speed, -rocket_speed);	break;
			case 1: speed = XY(rocket_speed, 0);				break;
			case 2: speed = XY(rocket_speed, rocket_speed);		break;
			}

		if (lim_right)
			switch (rand() % 3)
			{
			case 0: speed = XY(-rocket_speed, -rocket_speed);	break;
			case 1: speed = XY(-rocket_speed, 0);				break;
			case 2: speed = XY(-rocket_speed, rocket_speed);	break;
			}

		if (lim_up)
			switch (rand() % 3)
			{
			case 0: speed = XY(-rocket_speed, rocket_speed);	break;
			case 1: speed = XY(0, rocket_speed);				break;
			case 2: speed = XY(rocket_speed, rocket_speed);		break;
			}


		if (pos.Y > height / 2)
			switch (rand() % 3)
			{
			case 0: speed = XY(-rocket_speed, -rocket_speed);	break;
			case 1: speed = XY(0, -rocket_speed);				break;
			case 2: speed = XY(rocket_speed, -rocket_speed);	break;
			}
	}

	// нвепедэ
	void round()
	{
		if (!end_round)
		{
			count_round++;
			if (count_round == 10)
			{
				count_round = 0;
				fire = true;
				sum++;
				if (sum == 3)
				{
					end_round = true;
					sum = 0;
				}
			}
		}
	}

	// онбедемхе бпюфеяйху пюйер
	void logic()
	{
		common_rocket();
		behavior();
		round();
	}
};


// ямюпъдш
class Shell : public Square
{
public:
	// бшкер
	void limit()
	{
		if (lim_left || lim_right || lim_up || lim_down)
			destroy = true;
	}

	// онбедемхе ямюпъднб
	void logic()
	{
		common_square();
	}
};


// анмсяш
class Bonus : public Square
{
public:
	bool col; // жбер

	Bonus() : col(false) {}

	// бшкер
	void limit()
	{
		if (pos.Y > height)
			destroy = true;
	}

	// онбедемхе анмсянб
	void logic()
	{
		common_square();
	}

	// ашрэ хкх ме ашрэ
	bool create()
	{
		return rand() % 3 == 0;
	}

	// х еякх ашрэ, рн йюйхл
	void arm()
	{
		col = rand() % 2;
	}
};


// йпсц
class Circle : public Figure
{
public:
	XY pos;
	int r, limR;
	bool lim, destroy;
	HBRUSH brush;

	Circle() :
		r(0),
		limR(20),
		lim(false),
		destroy(false),
		brush(white)
	{}

	// дбхфемхе
	void draw()
	{
		if (lim)
		{
			r--;
			SelectObject(hdc, brush);
			Ellipse(hdc, pos.X - r, pos.Y - r, pos.X + r, pos.Y + r);
			if (r == 0)
				destroy = true;
		}
		else
		{
			SelectObject(hdc, brush);
			Ellipse(hdc, pos.X - r, pos.Y - r, pos.X + r, pos.Y + r);
			r++;
			if (r == limR)
				lim = true;
		}
	}

	// онбедемхе
	void logic()
	{
		draw();
	}
};


// йнялня
class Space
{
private:
	MyRocket MyR;
	Shell MySh_red;
	Shell MySh_green;
	EnemyRocket EnR;
	Shell EnSh;
	Bonus Bon;
	Circle Cir;
	vector<Shell> red_shells;
	vector<Shell> my_shells;
	vector<EnemyRocket> enemy_rockets;
	vector<Shell> enemy_shells;
	vector<Bonus> bonuses;
	vector<Circle> circles;
public:
	void new_game()
	{
		ShowWindow(GetConsoleWindow(), SW_SHOWMAXIMIZED);
		SetCursorPos(width * 9 / 10, height * 4 / 5);
		
		MyR.pos = XY(width / 2, 3 * height / 4);
		MyR.depth = rocket_depth;
		MyR.brush = blue;
		MySh_green.speed = XY(0, -shell_speed);
		MySh_green.depth = shell_depth;
		MySh_green.brush = green;

		for (i = 0; i < 7; i++)
		{
			MySh_red.speed = XY(Arr_DX[i], Arr_DY[i]);
			MySh_red.depth = shell_depth;
			MySh_red.brush = red;
			red_shells.push_back(MySh_red);
		}

		Bon.speed = XY(0, bonus_speed);
		srand(time(NULL));
	}

	bool alive()
	{
		return MyR.life > 0;
	}

	void make_EnR()
	{
		if (enemy_rockets.size() == 0)
		{
			lvl++;
			for (i = 0; i < lvl; i++)
			{
				a = rand() % (width);
				b = rand() % (height / 2);
				EnR.pos = XY(a, b);
				EnR.depth = rocket_depth;
				EnR.brush = cyan;
				enemy_rockets.push_back(EnR);
			}
		}
	}

	void logic()
	{
		MyR.logic();
		for (i = 0; i < my_shells.size(); i++)
			my_shells[i].logic();
		for (i = 0; i < enemy_shells.size(); i++)
			enemy_shells[i].logic();
		for (i = 0; i < enemy_rockets.size(); i++)
			enemy_rockets[i].logic();
		for (i = 0; i < bonuses.size(); i++)
			bonuses[i].logic();
		for (i = 0; i < circles.size(); i++)
			circles[i].logic();
	}

	void destroy()
	{
		for (i = 0; i < my_shells.size(); i++)
			if (my_shells[i].destroy)
				my_shells.erase(my_shells.begin() + i--);

		for (i = 0; i < enemy_rockets.size(); i++)
			if (enemy_rockets[i].destroy)
				enemy_rockets.erase(enemy_rockets.begin() + i--);

		for (i = 0; i < enemy_shells.size(); i++)
			if (enemy_shells[i].destroy)
				enemy_shells.erase(enemy_shells.begin() + i--);

		for (i = 0; i < bonuses.size(); i++)
			if (bonuses[i].destroy)
				bonuses.erase(bonuses.begin() + i--);

		for (i = 0; i < circles.size(); i++)
			if (circles[i].destroy)
				circles.erase(circles.begin() + i--);
	}

	void collide_MyR_EnR()
	{
		for (i = 0; i < enemy_rockets.size(); i++)
		{
			a = MyR.pos.X;
			b = MyR.pos.Y;
			c = enemy_rockets[i].pos.X;
			d = enemy_rockets[i].pos.Y;
			if (a - rocket_depth < c + rocket_depth && a + rocket_depth > c - rocket_depth &&
				b - rocket_depth < d + rocket_depth && b + rocket_depth > d - rocket_depth)
			{
				MyR.regress();
				enemy_rockets.erase(enemy_rockets.begin() + i--);

				Cir.pos = XY(a, b);
				Cir.limR = expl_rad_MyR;
				Cir.brush = yellow;
				circles.push_back(Cir);
			}
		}
	}

	void collide_MyR_EnSh()
	{
		for (i = 0; i < enemy_shells.size(); i++)
		{
			a = MyR.pos.X;
			b = MyR.pos.Y;
			c = enemy_shells[i].pos.X;
			d = enemy_shells[i].pos.Y;
			if (a - rocket_depth < c + shell_depth && a + rocket_depth > c - shell_depth &&
				b - rocket_depth < d + shell_depth && b + rocket_depth > d - shell_depth)
			{
				MyR.regress();
				enemy_shells.erase(enemy_shells.begin() + i--);
				Cir.pos = XY(a, b);
				Cir.limR = expl_rad_MyR;
				Cir.brush = yellow;
				circles.push_back(Cir);
			}
		}
	}

	void collide_MyR_Bon()
	{
		for (i = 0; i < bonuses.size(); i++)
		{
			a = MyR.pos.X;
			b = MyR.pos.Y;
			c = bonuses[i].pos.X;
			d = bonuses[i].pos.Y;
			if (a - rocket_depth < c + bonus_depth && a + rocket_depth > c - bonus_depth &&
				b - rocket_depth < d + bonus_depth && b + rocket_depth > d - bonus_depth)
			{
				MyR.bonus(!bonuses[i].col);
				bonuses.erase(bonuses.begin() + i--);
				Cir.pos = XY(c, d);
				Cir.limR = expl_rad_Bon;
				Cir.brush = white;
				circles.push_back(Cir);
			}
		}
	}

	void collide_MySh_EnR()
	{
		i = 0;
		while (i < my_shells.size())
		{
			j = 0;
			while (j < enemy_rockets.size())
			{
				a = my_shells[i].pos.X;
				b = my_shells[i].pos.Y;
				c = enemy_rockets[j].pos.X;
				d = enemy_rockets[j].pos.Y;
				if (a - shell_depth < c + rocket_depth && a + shell_depth > c - rocket_depth &&
					b - shell_depth < d + rocket_depth && b + shell_depth > d - rocket_depth)
				{
					my_shells.erase(my_shells.begin() + i);
					enemy_rockets.erase(enemy_rockets.begin() + j);
					MyR.progress();
					if (Bon.create())
					{
						Bon.arm();
						Bon.pos = XY(c, d);
						Bon.depth = bonus_depth;
						if (Bon.col)	Bon.brush = red;
						else			Bon.brush = green;
						bonuses.push_back(Bon);
					}
					Cir.pos = XY(c, d);
					Cir.limR = expl_rad_EnR;
					Cir.brush = yellow;
					circles.push_back(Cir);
					i--;
					break;
				}
				j++;
			}
			i++;
		}
	}

	void collide()
	{
		collide_MyR_EnR();
		collide_MyR_EnSh();
		collide_MyR_Bon();
		collide_MySh_EnR();
	}

	void fire()
	{
		if (MyR.fire)
		{
			a = MyR.pos.X;
			b = MyR.pos.Y;
			c = MyR.power;
			if (MyR.arm) // GREEN
				for (i = 0; i < 7; i++)
					switch (c)
					{
					case 1:
						if (i == 3)
						{
							MySh_green.pos = XY(a + Arr_X[i], b + Arr_Y[i]);
							my_shells.push_back(MySh_green);
						}
						break;
					case 2:
						if (i == 2 || i == 4)
						{
							MySh_green.pos = XY(a + Arr_X[i], b + Arr_Y[i]);
							my_shells.push_back(MySh_green);
						}
						break;
					case 3:
						if (i == 2 || i == 3 || i == 4)
						{
							MySh_green.pos = XY(a + Arr_X[i], b + Arr_Y[i]);
							my_shells.push_back(MySh_green);
						}
						break;
					case 4:
						if (i == 1 || i == 2 || i == 4 || i == 5)
						{
							MySh_green.pos = XY(a + Arr_X[i], b + Arr_Y[i]);
							my_shells.push_back(MySh_green);
						}
						break;
					case 5:
						if (i != 0 && i != 6)
						{
							MySh_green.pos = XY(a + Arr_X[i], b + Arr_Y[i]);
							my_shells.push_back(MySh_green);
						}
						break;
					case 6:
						if (i != 3)
						{
							MySh_green.pos = XY(a + Arr_X[i], b + Arr_Y[i]);
							my_shells.push_back(MySh_green);
						}
						break;
					default:
						MySh_green.pos = XY(a + Arr_X[i], b + Arr_Y[i]);
						my_shells.push_back(MySh_green);
						break;
					}
			else // RED
				for (i = 0; i < 7; i++)
					switch (c)
					{
					case 1:
						if (i == 3)
						{
							red_shells[i].pos = XY(a + Arr_X[i], b + Arr_Y[i]);
							my_shells.push_back(red_shells[i]);
						}
						break;
					case 2:
						if (i == 2 || i == 4)
						{
							red_shells[i].pos = XY(a + Arr_X[i], b + Arr_Y[i]);
							my_shells.push_back(red_shells[i]);
						}
						break;
					case 3:
						if (i == 2 || i == 3 || i == 4)
						{
							red_shells[i].pos = XY(a + Arr_X[i], b + Arr_Y[i]);
							my_shells.push_back(red_shells[i]);
						}
						break;
					case 4:
						if (i == 1 || i == 2 || i == 4 || i == 5)
						{
							red_shells[i].pos = XY(a + Arr_X[i], b + Arr_Y[i]);
							my_shells.push_back(red_shells[i]);
						}
						break;
					case 5:
						if (i != 0 && i != 6)
						{
							red_shells[i].pos = XY(a + Arr_X[i], b + Arr_Y[i]);
							my_shells.push_back(red_shells[i]);
						}
						break;
					case 6:
						if (i != 3)
						{
							red_shells[i].pos = XY(a + Arr_X[i], b + Arr_Y[i]);
							my_shells.push_back(red_shells[i]);
						}
						break;
					default:
						red_shells[i].pos = XY(a + Arr_X[i], b + Arr_Y[i]);
						my_shells.push_back(red_shells[i]);
						break;
					}
		}

		for (i = 0; i < enemy_rockets.size(); i++)
			if (enemy_rockets[i].fire)
			{
				a = enemy_rockets[i].pos.X;
				b = enemy_rockets[i].pos.Y;
				c = MyR.pos.X;
				d = MyR.pos.Y;
				p = d - b;
				q = c - a;
				tngns = p / q;
				t = 1 + tngns * tngns;
				cosns = 1 / sqrt(t);
				if (q < 0)
					cosns = -cosns;
				sinus = tngns * cosns;
				
				EnSh.pos = XY(a, b);
				EnSh.speed = XY(shell_speed * cosns, shell_speed * sinus);
				EnSh.depth = shell_depth;
				EnSh.brush = magenta;
				enemy_shells.push_back(EnSh);
			}
	}

	void background()
	{
		SelectObject(hdc, black);
		Rectangle(hdc, 0, 0, 2 * width, 2 * height);
	}

	void All()
	{
		background();
		make_EnR();
		logic();
		destroy();
		collide();
		fire();
	}
};


int main()
{
	Space Action;
	Action.new_game();
	while (!GameOver && Action.alive())
	{
		Action.All();
		Sleep(40);
	}
	system("pause");
	return 0;
}