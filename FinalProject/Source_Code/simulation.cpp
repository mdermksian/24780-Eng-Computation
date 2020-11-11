/**
 * Implementation of the Simulation class
 */
#include "simulation.h"
#include "fssimplewindow.h"
#include <stdio.h>
//#include "gui.h"

void draw_bitmap(const int bitmap[60][60], const int sX, const int sY, const int eX, const int eY)
{
	for (int i = 0; i < 60; i++) {
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

	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex2f(sX * 10, sY * 10);
	glVertex2f(sX * 10, sY * 10 + 10);
	glVertex2f(sX * 10 + 10, sY * 10 + 10);
	glVertex2f(sX * 10 + 10, sY * 10);
	glEnd();

	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex2f(eX * 10, eY * 10);
	glVertex2f(eX * 10, eY * 10 + 10);
	glVertex2f(eX * 10 + 10, eY * 10 + 10);
	glVertex2f(eX * 10 + 10, eY * 10);
	glEnd();


}

Simulation::Simulation() {
    wipeBitmap();
    gui = new GUI();
	// New //
	gui->Initialize(BMP_W, BMP_H);
	ob.intialize(BMP_W, BMP_H);
	robot = new Robot();

	startX = -1;
	startY = -1;
	endX = -1;
	endY = -1;

	needsUpdate = true;

	pathfinder = new Nodes();
}

Simulation::~Simulation() {
}

// void Simulation::clear(void) {
//     if(nullptr != bitmap) {
//         delete [] bitmap;
//         bitmap = nullptr;
//     }
// }

// int **Simulation::getBitmap(void) {
//     return bitmap;
// }

void Simulation::wipeBitmap(void) {
    for(int i = 0; i < BMP_W; ++i) {
        for(int j = 0; j < BMP_H; ++j) {
            bitmap[i][j] = 0;
        }
    }
}

void Simulation::setBitmap(int bmp[BMP_W][BMP_H]) {
    for(int i = 0; i < BMP_W; ++i) {
        for(int j = 0; j < BMP_H; ++j) {
            bitmap[i][j] = bmp[i][j];
        }
    }
}

void Simulation::setBitmap(int value, int x, int y) {
    bitmap[x][y] = value;
}

void Simulation::setState(int newState) {
    state = newState;
}

int Simulation::getState(void) {
    return state;
}

void Simulation::run(void) {
    FsOpenWindow(0,0,800,600,1);
    while(1) {
        FsPollDevice();
        auto key = FsInkey();

		if (key == FSKEY_ESC) {
			break;
		}
        gui->Run(bitmap);
		startX = gui->startX;
		startY = gui->startY;

		if (robot->x == -1 && robot->y == -1 && startX != -1 && startY != -1) {
			robot->UpdateStart(startX, startY);
			printf("set robot to %d %d\n", robot->x, robot->y);
		}
		endX = gui->endX;
		endY = gui->endY;

		if (gui->obst->state == 1) {
			FsPollDevice();
			ob.setObstacle(bitmap, false);
		}

		if (gui->erase->state == 1) {
			printf("test");
			FsPollDevice();
			ob.setObstacle(bitmap, true);
		}

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		if (startX != -1 && startY != -1 && endX != -1 && endY != -1 && gui->play->state == 1 ) { //&& needsUpdate == true
			int curx, cury;		
			//curx = robot->traj[robot->index].first;
			//cury = robot->traj[robot->index].second;

			//printf("%d   , %d\n", curx, cury);
			traj = pathfinder->FindTrajectory(bitmap, robot->x, robot->y, endX, endY);
			//robot->index = 0;
			
			robot->UpdateTraj(traj);
			robot->UpdateBitMap(bitmap);
			//needsUpdate = false;
		}
        draw();
        FsSwapBuffers();
        FsSleep(10);
    }
}

void Simulation::draw(void) {
	if (startX == -1 || startY == -1 || endX == -1 || endY == -1 || gui->play->state == 0) {
		draw_bitmap(bitmap,startX,startY,endX,endY);
	}
	else {
		FsPollDevice();
		if (0 != FsGetKeyState(FSKEY_UP))
		{
			robot->orbit.dist = robot->orbit.dist * 0.9;
		}
		if (0 != FsGetKeyState(FSKEY_DOWN))
		{
			robot->orbit.dist = robot->orbit.dist * 1.1;
		}
		robot->Draw();
	}
	gui->Draw(bitmap);

}