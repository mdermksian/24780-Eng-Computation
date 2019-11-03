#include <stdio.h>
#include "yspng.h"
#include "yspngenc.h"
#include "fssimplewindow.h"


float CurveFunction(float x,float w)
{
	return (2.0f-4.0f*w)*x*x+(4.0f*w-1.0f)*x;
}

unsigned int Clamp(int x)
{
	if(x<0)
	{
		return 0;
	}
	else if(255<x)
	{
		return 255;
	}
	else
	{
		return x;
	}
}

void CurveFilter(YsRawPngDecoder &png,float w)
{
	for(int i=0; i<png.wid*png.hei; ++i)
	{
		unsigned char *pixPtr=png.rgba+i*4;
		float r=(float)pixPtr[0]/255.0f;
		float g=(float)pixPtr[1]/255.0f;
		float b=(float)pixPtr[2]/255.0f;

		r=CurveFunction(r,w);
		g=CurveFunction(g,w);
		b=CurveFunction(b,w);

		int ri=(int)(r*255.0f);
		int gi=(int)(g*255.0f);
		int bi=(int)(b*255.0f);

		pixPtr[0]=Clamp(ri);
		pixPtr[1]=Clamp(gi);
		pixPtr[2]=Clamp(bi);
	}
}

int main(void)
{
	YsRawPngDecoder png;

	png.Decode("BabyBlueJay.png");
	printf("%d %d\n",png.wid,png.hei);

	FsOpenWindow(0,0,1600,800,1);
	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

		if(FSKEY_SPACE==key)
		{
			CurveFilter(png,0.7);
		}
		if(FSKEY_S==key)
		{
			YsRawPngEncoder encoder;
			encoder.EncodeToFile("output.png",png.wid,png.hei,8,6,png.rgba);
		}


		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glRasterPos2i(0,599);
		glDrawPixels(png.wid,png.hei,GL_RGBA,GL_UNSIGNED_BYTE,png.rgba);

		FsSwapBuffers();
		FsSleep(10);
	}
	

	return 0;
}

