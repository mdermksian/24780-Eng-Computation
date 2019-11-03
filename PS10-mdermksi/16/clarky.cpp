#include <stdio.h>
#include "yspng.h"
#include "yspngenc.h"
#include "fssimplewindow.h"

int main(void)
{
	YsRawPngDecoder png;

	png.Decode("ClarkY.png");
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
			for(int i=0; i<png.wid*png.hei; ++i)
			{
				unsigned char *pixPtr=png.rgba+i*4;
				if(pixPtr[0]<240 ||
				   128<pixPtr[1] ||
				   128<pixPtr[2])
				{
					pixPtr[0]=255;
					pixPtr[1]=255;
					pixPtr[2]=255;
				}
			}

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

