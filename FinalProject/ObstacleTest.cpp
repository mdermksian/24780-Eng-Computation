#include <iostream>
#include<stdio.h>
#include"fssimplewindow.h"
#include"Obstacle.h"
void draw_bitmap(int bitmap[60][60])
{
	for (int i = 0; i < 60; i++)
		for (int j = 0; j < 60; j++)
		{
			if (bitmap[i][j] == 0)
			{
				glColor3f(1, 1, 1);
				glBegin(GL_QUADS);
				glVertex2f(i * 10, j * 10);
				glVertex2f(i * 10, j * 10 + 10);
				glVertex2f(i * 10 + 10, j * 10 + 10);
				glVertex2f(i * 10 + 10, j * 10);
				glEnd();
			}
			else
			{
				glColor3f(0, 0, 0);
				glBegin(GL_QUADS);
				glVertex2f(i * 10, j * 10);
				glVertex2f(i * 10, j * 10 + 10);
				glVertex2f(i * 10 + 10, j * 10 + 10);
				glVertex2f(i * 10 + 10, j * 10);
				glEnd();
			}
		}
}
void generate_bitmap(int bitmap[60][60])
{
	for (int i = 0; i < 60; i++)
		for (int j = 0; j < 60; j++)
			bitmap[i][j] = 0;
}

int main()
{
	FsOpenWindow(100, 100, 600, 600, 1);
	Obstacles ob;
	int bitmap[60][60];
	generate_bitmap(bitmap);
	while (FSKEY_ESC != FsInkey())
	{
		ob.setObstacle(bitmap);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		FsPollDevice();
		draw_bitmap(bitmap);
		FsSwapBuffers();
		FsSleep(25);
	}
	for (int i = 0; i < 60; i++)
	{
		for (int j = 0; j < 60; j++)
			std::cout << bitmap[j][i] << " ";
		std::cout << "\n";
	}

}