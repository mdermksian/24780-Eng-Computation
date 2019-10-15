#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "fssimplewindow.h"

const double PI = 3.1415627;
const double dt = 0.025;
const double g = 9.8;
void PhysicalCoordToScreenCoord(int &sx,int &sy,double px,double py)
{
	sx=(int)(px*10.0);
	sy=600-(int)(py*10.0);
}

void DrawCircle(int cx,int cy,int rad,int fill)
{

    if(0!=fill)
    {
        glBegin(GL_POLYGON);
    }
    else
    {
        glBegin(GL_LINE_LOOP);
    }

    int i;
    for(i=0; i<64; i++)
    {
        double angle=(double)i*PI/32.0;
        double x=(double)cx+cos(angle)*(double)rad;
        double y=(double)cy+sin(angle)*(double)rad;
        glVertex2d(x,y);
    }

    glEnd();
}

void DrawRect(int x1,int y1,int x2,int y2,int fill)
{
    if(0!=fill)
    {
        glBegin(GL_QUADS);
    }
    else
    {
        glBegin(GL_LINE_LOOP);
    }

    glVertex2i(x1,y1);
    glVertex2i(x2,y1);
    glVertex2i(x2,y2);
    glVertex2i(x1,y2);

    glEnd();
}

void DrawLine(int x1,int y1,int x2,int y2)
{
    glBegin(GL_LINES);

    glVertex2i(x1,y1);
    glVertex2i(x2,y2);

    glEnd();
}

class Artillery {
    public:
        double x, y, angle;
        void Initialize(void);
        void Draw(void);
};

class CannonBall {
    public:
        double x, y, vx, vy;
        int state = 0;
        void Initialize(void);
        void Draw(void);
        void Move(void);
        void Fire(Artillery artillery);
};

class Obstacle {
    public:
        double x, y, w, h;
        int state;
        void Generate(void);
        void Draw(void);
        int CheckHitByCannonBall(double mx, double my);
};

class Target {
    public:
        double x, y, w, h;
        void Initialize(void);
        void Draw(void);
        void Move(void);
        int CheckHitByCannonBall(double mx, double my);
};

void Artillery::Initialize(void) {
    x = 5.0;
    y = 5.0;
    angle = PI/6.0;
}

void Artillery::Draw(void) {
    angle += PI/180.0;
    if (PI/2.0 < angle) {
        angle = 0.0;
    }
    int sx,sy;
	PhysicalCoordToScreenCoord(sx,sy,x,y);

    glColor3ub(0,0,255);
    DrawRect(sx-5,sy-5,sx+5,sy+5,1);

	int vx,vy;
	PhysicalCoordToScreenCoord(vx,vy,x+3.0*cos(angle),y+3.0*sin(angle));

    DrawLine(sx,sy,vx,vy);
}

void CannonBall::Draw(void) {
	int sx,sy;
	PhysicalCoordToScreenCoord(sx,sy,x,y);

    glColor3ub(255,0,0);
    DrawCircle(sx,sy,5,1);
}

void CannonBall::Move(void) {
    if (state == 1) {
        x += vx*dt;
        y += vy*dt;

        vy -= g*dt;
        if(y<0.0 || x<0.0 || 80.0<x) {
            state=0;
        }
    }
}

void CannonBall::Fire(Artillery artillery) {
    if(state == 0) {
        state = 1;
        x = artillery.x;
        y = artillery.y;
        vx = 40.0*cos(artillery.angle);
        vy = 40.0*sin(artillery.angle);
    }
}

void Obstacle::Generate(void) {
    state=1;
    x = (double)(10+rand()%70);
    y = (double)(rand()%60);
    w = (double)(8+rand()%8);
    h = (double)(8+rand()%8);

    // The following if-statements forces the effective size of the
    // obstacle to be between 8x8 and 15x15.
    if(80.0<x+w)
    {
        x=80.0-w;
    }
    if(60.0<y+h)
    {
        y=60.0-h;
    }
}

void Obstacle::Draw(void) {
    glColor3ub(0,255,0);
    int x1,y1,x2,y2;
    PhysicalCoordToScreenCoord(x1,y1,x,y);
    PhysicalCoordToScreenCoord(x2,y2,x+w,y+h);
    DrawRect(x1,y1,x2,y2,state);
}

int Obstacle::CheckHitByCannonBall(double mx, double my) {
    int relativeX,relativeY;
    relativeX=mx-x;
    relativeY=my-y;
    if(0<=relativeX && relativeX<w && 0<=relativeY && relativeY<h)
    {
        state = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

void Target::Initialize(void) {
    x = 75.0;
    y = 60.0;
    w = 5.0;
    h = 5.0;
}

void Target::Draw(void) {
    int x1,y1,x2,y2;
	PhysicalCoordToScreenCoord(x1,y1,x,y);
	PhysicalCoordToScreenCoord(x2,y2,x+w,y+h);

    glColor3ub(255,0,0);
    DrawRect(x1,y1,x2,y2,1);
}

void Target::Move(void) {
    y-=10*dt;
	if(0>y)
	{
		y=60.0;
	}
}

int Target::CheckHitByCannonBall(double mx, double my) {
    int relativeX,relativeY;
    relativeX=mx-x;
    relativeY=my-y;
    if(0<=relativeX && relativeX<w && 0<=relativeY && relativeY<h)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// For extra credit >>
void DrawTrajectory(int nPnt,const double pnt[])
{
	if(2<=nPnt)
	{
		glColor3ub(0,0,0);
		glBegin(GL_LINE_STRIP);

		int i;
		for(i=0; i<nPnt; i++)
		{
			int sx,sy;
			PhysicalCoordToScreenCoord(sx,sy,pnt[i*2],pnt[i*2+1]);
			glVertex2i(sx,sy);
		}

		glEnd();
	}
}
// For extra credit <<

const int nObstacle=5;

int main(void)
{
    srand(time(NULL));

    int i,key,nShot;

    Artillery artillery;
    artillery.Initialize();
    Target target;
    target.Initialize();
    CannonBall cannon_balls[5];
    Obstacle obstacles[nObstacle];
    for( Obstacle &obstacle : obstacles) {
        obstacle.Generate();
    }

	// For extra credit >>
	const int maxNumTrj=200;
	int nTrj=0;
	double trj[maxNumTrj*2];
	// For extra credit <<

    FsOpenWindow(16,16,800,600,1);

    FsPollDevice();

    nShot=0;
    while(FSKEY_ESC!=(key=FsInkey()))
    {
        FsPollDevice();

        switch(key)
        {
        case FSKEY_SPACE:
            for( CannonBall &cannon_ball : cannon_balls ) {
                if(cannon_ball.state==0)
                {
                    nShot++;
                    // For extra credit >>
                    nTrj=0;
                    // For extra credit <<
                    cannon_ball.Fire(artillery);
                    break;
                }
            }
            break;
        }

		target.Move();
        for( CannonBall &cannon_ball : cannon_balls ) {
            cannon_ball.Move();
        }
        // For extra credit >>
        // if(nTrj<maxNumTrj)
        // {
        //     trj[nTrj*2  ]=cannon_ball.x;
        //     trj[nTrj*2+1]=cannon_ball.y;
        //     nTrj++;
        // }
        // For extra credit <<

        for( CannonBall &cannon_ball : cannon_balls ) {
            for( Obstacle &obstacle : obstacles ) {
                if( obstacle.state == 1 && obstacle.CheckHitByCannonBall(cannon_ball.x, cannon_ball.y) ) {
                    printf("Hit Obstacle!\n");
                    cannon_ball.state=0;
                }
            }
            if(target.CheckHitByCannonBall(cannon_ball.x,cannon_ball.y)==1)
            {
                printf("Hit Target!\n");
                printf("You fired %d shots.\n",nShot);
                return 0;
            }
        }

        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        artillery.Draw();
        for( Obstacle obstacle : obstacles ) {
            obstacle.Draw();
        }
        for( CannonBall cannon_ball : cannon_balls ) {
            if(cannon_ball.state==1)
            {
                cannon_ball.Draw();
                // For extra credit >>
                //DrawTrajectory(nTrj,trj);
                // For extra credit <<
            }
        }
        target.Draw();
        
        FsSwapBuffers();

        FsSleep(25);
    }

    return 0;
}
