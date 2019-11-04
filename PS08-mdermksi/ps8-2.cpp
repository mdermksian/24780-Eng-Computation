#include <stdio.h>
#include <math.h>
#include "fssimplewindow.h"
#include "yspng.h"

void RemoveLB(char fName[])
{
    for(int i=0; fName[i]!=0; ++i){
        if(fName[i]=='\n'){
            fName[i]=0;
            break;
        }
    }
}

void InvertImage(YsRawPngDecoder &png)
{
    for(int i = 0; i < png.wid*png.hei; ++i) {
        auto pixPtr=png.rgba+i*4;
        pixPtr[0] = 255-pixPtr[0];
        pixPtr[1] = 255-pixPtr[1];
        pixPtr[2] = 255-pixPtr[2];
    }
}

// void FlipImage(YsRawPngDecoder &png)
// {
//     auto rgba_copy = *png.rgba;
//     for(int i=0; i<png.wid*png.hei; ++i){
//         auto pixPtr = png.rgba+i*4;
//     }
// }

void Greyscale(YsRawPngDecoder &png)
{
    for(int i = 0; i < png.wid*png.hei; ++i) {
        auto pixPtr = png.rgba+i*4;
        int r = pixPtr[0];
        int g = pixPtr[1];
        int b = pixPtr[2];
        int average = (r+g+b)/3;
        pixPtr[0] = average;
        pixPtr[1] = average;
        pixPtr[2] = average;
    }
}

int main(void)
{
    char fName[256];
    printf("Enter filename> ");
    fgets(fName,255,stdin);
    RemoveLB(fName);

    YsRawPngDecoder png;

    png.Decode(fName);
    png.Flip();
    printf("WID: %d, HEI: %d",png.wid,png.hei);

    FsOpenWindow(0,0,png.wid+10,png.hei+10,1);
    int lb,mb,rb,mx,my;
    while(1){
        FsPollDevice();
        auto key = FsInkey();
        if(FSKEY_ESC==key){
            break;
        }
        if(FSKEY_I==key){
            InvertImage(png);
        }
        if(FSKEY_G==key){
            Greyscale(png);
        }
        if(FSMOUSEEVENT_MOVE == FsGetMouseEvent(lb,mb,rb,mx,my)){
            if(mx<png.wid && my<png.hei){
                int r,g,b;
                int index = mx + (png.hei - my)*png.wid;
                auto pixPtr = png.rgba+index*4;
                r = pixPtr[0];
                g = pixPtr[1];
                b = pixPtr[2];
                printf("X: %d, Y:%d, rgb: %d %d %d\n",mx, my, r,g,b);
            }
        }
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glRasterPos2i(0,png.hei);
        glDrawPixels(png.wid,png.hei,GL_RGBA,GL_UNSIGNED_BYTE,png.rgba);
        FsSwapBuffers();
        FsSleep(10);
    }

    return 0;
}