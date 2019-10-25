#include <stdio.h>
#include "fssimplewindow.h"

int main(void)
{
	FsOpenWindow(64,16,800,600,1);

	int bx,by,vx,vy,rx,ry,rdx,rdy;

	bx=400;
	by=300;
	vx=5;
	vy=-5;

	rx=300;
	ry=550;
	rdx=80;
	rdy=20;

	for(;;)
	{
		FsPollDevice();

		if(FSKEY_ESC==FsInkey())
		{
			break;
		}

		glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);


		glColor3ub(0,0,0);
		glBegin(GL_QUADS);
		glVertex2i(bx-3,by-3);
		glVertex2i(bx+3,by-3);
		glVertex2i(bx+3,by+3);
		glVertex2i(bx-3,by+3);

		glVertex2i(rx    ,ry);
		glVertex2i(rx+rdx,ry);
		glVertex2i(rx+rdx,ry+rdy);
		glVertex2i(rx    ,ry+rdy);

		glEnd();


		bx+=vx;
		by+=vy;

		if((bx<0 && vx<0) || (800<bx && 0<vx))
		{
			vx=-vx;
		}
		if((by<0 && vy<0))
		{
			vy=-vy;
		}


		if(600<by)
		{
			printf("Miss!\n");
			break;
		}


		int lb,mb,rb;
		FsGetMouseEvent(lb,mb,rb,rx,ry);
		if(rx<0)
		{
			rx=0;
		}
		else if(800<rx)
		{
			rx=800;
		}
		if(ry<500)
		{
			ry=500;
		}
		else if(600<ry)
		{
			ry=600;
		}

		if(rx<=bx && bx<=rx+rdx && ry<=by && by<=ry+rdy && 0<vy)
		{
			vy=-vy;
		}


		FsSwapBuffers();
		FsSleep(25);
	}


}