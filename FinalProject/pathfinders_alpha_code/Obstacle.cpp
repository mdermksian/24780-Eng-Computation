#include<iostream>
#include "fssimplewindow.h"
#include<stdio.h>
#include"Obstacle.h"
#include <time.h>
#include <stdlib.h>

void set_within_bounds(int& a, int& b)
{
	if (a > 600)
	{
		a = 600;
	}
	if (b > 600)
	{
		b = 600;
	}
	if (a < 0)
	{
		a = 0;
	}
	if (b < 0)
	{
		b = 0;
	}
}

void Obstacles::Set_xy(int a, int b)
{
	x = a;
	y = b;
}

void Obstacles::SetHeightWidth(float a, float b)
{
	w = a;
	h = b;
}

void Obstacles::Draw()
{
	glColor3f(r,g,b);
	glBegin(GL_QUADS);
	glVertex2f(x ,y);
	glVertex2f(x ,y+h);
	glVertex2f(x+w ,y+h);
	glVertex2f(x+ w,y);
	glEnd();
}

void Obstacles::Predefined()
{
	int lb, mb, rb, mx, my;
	FsGetMouseEvent(lb, mb, rb, mx, my);
	if (lb > 0)
	{
		Set_xy(mx, my);
		Draw();
	}

}

void Obstacles::mapToBitMap(int bitmap[60][60])
{
	
	for (int i = 0;i<fabs(w); i+=1)
	{
		for (int j =0; j<fabs(h); j+=1)
		{
			srand(i*j);
			depth = rand() % 8 + 1;
			bitmap[(int)(x + ((float)w / fabs(w)) * i)][(int)(y + ((float)h / fabs(h)) * j)] = depth;
		}
	}
}

void Obstacles::setObstacle(int bitmap[60][60])
{
	int lb, mb, rb, mx, my;
	FsGetMouseEvent(lb, mb, rb, mx, my);
	if (lb > 0&&(mx<600&&my<600))
	{

		set_within_bounds(mx, my);
		if(state == false)
		{
			intialize(mx, my);
		}
		else
		{
			generate(mx, my);

			mapToBitMap(bitmap);
		}
	}
	else 
	{
		if (state == true)
		{
			state = false;
		}
		else
		{
			// Do nothing
		}
	}
}

void Obstacles::intialize(int mx, int my)
{
	state = true;
	std::cout << roundf(float(mx / 10))<< "  "<< roundf(float(my / 10));
	Set_xy(roundf(float(mx / 10)), roundf(float(my / 10)));
	SetHeightWidth(0, 0);
	//printf("Creating\n");
}

void Obstacles::generate(int mx, int my)
{
	float dx = mx - 10 * x;
	float dy = my - 10 * y;
	SetHeightWidth((float(dx / 10)), (float(dy / 10)));
}

//#include<iostream>
//#include "fssimplewindow.h"
//#include<stdio.h>
//#include"Obstacle.h"
//#include<time.h>
//void set_within_bounds(int& a, int& b)
//{
//	if (a > 600)
//	{
//		a = 600;
//	}
//	if (b > 600)
//	{
//		b = 600;
//	}
//	if (a < 0)
//	{
//		a = 0;
//	}
//	if (b < 0)
//	{
//		b = 0;
//	}
//}
//
//void Obstacles::Set_xy(int a, int b)
//{
//	x = a;
//	y = b;
//}
//
//void Obstacles::SetHeightWidth(float a, float b)
//{
//	w = a;
//	h = b;
//}
//
//void Obstacles::Draw()
//{
//	glColor3f(r, g, b);
//	glBegin(GL_QUADS);
//	glVertex2f(x, y);
//	glVertex2f(x, y + h);
//	glVertex2f(x + w, y + h);
//	glVertex2f(x + w, y);
//	glEnd();
//}
//
//void Obstacles::Predefined()
//{
//	int lb, mb, rb, mx, my;
//	FsGetMouseEvent(lb, mb, rb, mx, my);
//	if (lb > 0)
//	{
//		Set_xy(mx, my);
//		Draw();
//	}
//
//}
//
//void Obstacles::mapToBitMap(int bitmap[60][60])
//{
//
//	for (int i = 0;i < fabs(h); i += 1)
//	{
//		for (int j = 0; j < fabs(w); j += 1)
//		{
//			bitmap[(int)ceilf(y + ((float)h / fabs(h)) * i)][(int)ceilf(x + ((float)w / fabs(w)) * j)] = depth;
//		}
//	}
//}
//
//void Obstacles::setObstacle(int bitmap[60][60])
//{
//	int lb, mb, rb, mx, my;
//	FsGetMouseEvent(lb, mb, rb, mx, my);
//	if (lb > 0 && (mx > 0 && mx < 600) && (my > 0 && my < 600))
//	{
//		set_within_bounds(mx, my);
//		if (state == false)
//		{
//			intialize(mx, my);
//			srand(time(NULL));
//			depth = rand() % 4 + 5;
//		}
//		else
//		{
//			generate(mx, my);
//
//			mapToBitMap(bitmap);
//		}
//	}
//	else
//	{
//		if (state == true)
//		{
//			state = false;
//		}
//		else
//		{
//			//
//
//		}
//	}
//}
//
//void Obstacles::intialize(int mx, int my)
//{
//	state = true;
//	std::cout << roundf(float(mx / 10)) << "  " << roundf(float(my / 10));
//	Set_xy(ceilf(float(mx / 10)), ceilf(float(my / 10)));
//	SetHeightWidth(0, 0);
//	//printf("Creating\n");
//}
//
//void Obstacles::generate(int mx, int my)
//{
//	float dx = mx - 10 * x;
//	float dy = my - 10 * y;
//	SetHeightWidth((float(dx / 10)), (float(dy / 10)));
//}
