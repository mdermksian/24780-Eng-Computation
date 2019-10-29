#include <stdio.h>
#include "fssimplewindow.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>

const double MD_PI = 3.1415926;

class GameObject {
public:
	int x, y, w, h;
};

class GameObjectWithVelocity : public GameObject {
public:
	int vx, vy;
	double angle;
};

class Ball : public GameObjectWithVelocity {
public:
	int state;
	Ball();
	void Draw(void) const;
	void Move(void);
};

class GameObjectHitTest : public GameObject {
public:
	int HitTest(Ball &ball) const;
};

int GameObjectHitTest::HitTest(Ball &ball) const {
	if(ball.x>=x && ball.x<=x+w && ball.y>=y && ball.y<=y+h)
	{
		return 1;
	}
	return 0;
}

class Racket : public GameObjectHitTest {
public:
	Racket();
	void Draw(void) const;
};

class Block : public GameObjectHitTest {
public:
	int state;
	Block();
	void Draw(void) const;
};

Ball::Ball() {
	state = 1;
	w = 6;
	h = 6;
	x = 400;
	y = 300;
	angle = (rand()%30+30)*(MD_PI/180);
	vx = rand()%2 ? sqrt(5*5*2)*cos(angle) : -sqrt(5*5*2)*cos(angle);
	vy = -sqrt(5*5*2)*sin(angle);
}

void Ball::Draw(void) const {
	if(state == 0) { return; }
	glColor3ub(0,0,0);
	glBegin(GL_QUADS);
	glVertex2i(x-3,y-3);
	glVertex2i(x+3,y-3);
	glVertex2i(x+3,y+3);
	glVertex2i(x-3,y+3);
	glEnd();
}

void Ball::Move() {
	if(state == 0) { return; }
	x += vx;
	y += vy;
	if((x<0 && vx<0) || (800<x && 0<vx))
	{
		vx=-vx;
	}
	if((y<0 && vy<0))
	{
		vy=-vy;
	}
	if(600<y){
		state = 0;
		printf("Miss!");
	}
}

Racket::Racket() {
	w = 80;
	h = 20;
	x = 300;
	y = 550;
}

void Racket::Draw(void) const {
	glColor3ub(0,0,0);
	glBegin(GL_QUADS);
	glVertex2i(x  , y);
	glVertex2i(x+w, y);
	glVertex2i(x+w, y+h);
	glVertex2i(x  , y+h);
	glEnd();
}

Block::Block(){
	state = 1;
	w = 80;
	h = 20;
}

void Block::Draw(void) const {
	if(state == 0) { return; }
	glColor3ub(0,0,0);
	glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(x+w, y);
	glVertex2i(x+w, y+h);
	glVertex2i(x  , y+h);
	glEnd();

}

int main(void)
{
	srand((int)time(nullptr));
	FsOpenWindow(64,16,800,600,1);

	Ball balls[3];
	Racket racket;
	Block blocks[10];

	for(int i = 0; i < 10; ++i) {
		blocks[i].y = 30;
		blocks[i].x = blocks[i].w*i;
	}

	for(;;)
	{
		FsPollDevice();

		if(FSKEY_ESC==FsInkey())
		{
			break;
		}

		glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

		for(Ball ball : balls){
			ball.Draw();
		}
		racket.Draw();
		for(Block block : blocks){
			block.Draw();
		}

		int hasLost = 1;
		for(Ball &ball : balls){
			ball.Move();
			if(ball.state == 1){
				hasLost = 0;
			}
		}

		if(hasLost){
			break;
		}


		int lb,mb,rb;
		FsGetMouseEvent(lb,mb,rb,racket.x,racket.y);
		if(racket.x<0)
		{
			racket.x=0;
		}
		else if(800<racket.x)
		{
			racket.x=800;
		}
		if(racket.y<500)
		{
			racket.y=500;
		}
		else if(600<racket.y)
		{
			racket.y=600;
		}

		int hasWon = 1;
		for(Block &block : blocks){
			if(block.state == 1){
				hasWon = 0;
				for(Ball &ball:balls){
					if(block.HitTest(ball)){
						block.state = 0;
						ball.vy=-ball.vy;
					}
				}
			}
		}

		if(hasWon == 1){
			break;
		}
		
		for(Ball &ball:balls){
			if(racket.HitTest(ball)){
				ball.vy=-ball.vy;
			}
		}

		FsSwapBuffers();
		FsSleep(25);
	}


}