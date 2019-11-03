#include <algorithm>

#include <stdio.h>
#include "yspng.h"
#include "yspngenc.h"
#include "fssimplewindow.h"




void BubbleSort(int n,int v[])
{
	for(int i=0; i<n; ++i)
	{
		for(int j=i+1; j<n; ++j)
		{
			if(v[i]>v[j])
			{
				std::swap(v[i],v[j]);
			}
		}
	}
}



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


void MedianFilter(YsRawPngDecoder &png,float w)
{
	for(int x=0; x<png.wid; ++x)
	{
		for(int y=0; y<png.hei; ++y)
		{
			int ns=0;
			int rs[9],gs[9],bs[9];

			for(int dx=-1; dx<=1; ++dx)
			{
				for(int dy=-1; dy<=1; ++dy)
				{
					int sx=x+dx, sy=y+dy;
					if(0<=sx && sx<png.wid && 0<=sy && sy<png.hei)
					{
						unsigned char *pixPtr=png.rgba+4*(sy*png.wid+sx);
						rs[ns]=pixPtr[0];
						gs[ns]=pixPtr[1];
						bs[ns]=pixPtr[2];
						++ns;
					}
				}
			}

			if(0<ns)
			{
				unsigned char *pixPtr=png.rgba+4*(y*png.wid+x);

				BubbleSort(ns,rs);
				BubbleSort(ns,gs);
				BubbleSort(ns,bs);

				pixPtr[0]=(unsigned char)rs[ns/2];
				pixPtr[1]=(unsigned char)gs[ns/2];
				pixPtr[2]=(unsigned char)bs[ns/2];
			}
		}
	}
}



int main(void)
{
	YsRawPngDecoder png;

	png.Decode("Noise.png");
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
			MedianFilter(png,0.7);
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

