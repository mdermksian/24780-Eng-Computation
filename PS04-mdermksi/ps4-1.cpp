#include <stdio.h>
#include "fssimplewindow.h"

void VisualizeArrayContents(int n, int x[], int movedIndex1, int movedIndex2)
{
    int bar_height = 400 / n;
    int max_bar_width = 400/20;
    glBegin(GL_QUADS);
    for( int i = 0; i < n; ++i ){
        if(movedIndex1 == i || movedIndex2 == i){
            glColor3ub(255, 0, 0);
        } else {
            glColor3ub(0, 0, 255);
        }
        glVertex2i(0, i*bar_height);
        glVertex2i(x[i]*max_bar_width, i*bar_height);
        glVertex2i(x[i]*max_bar_width, (i+1)*bar_height);
        glVertex2i(0, (i+1)*bar_height);
    }
    glEnd();
}

void SwapInt(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

void Show(int n, int x[], int toBeMoved1, int toBeMoved2)
{
    FsPollDevice();
    while(FSKEY_NULL==FsInkey()) {
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        VisualizeArrayContents(n,x,toBeMoved1,toBeMoved2);
        FsSwapBuffers();

        FsPollDevice();
        FsSleep(10);
    }
}

void BubbleSort(int n,int x[])
{
    int i,j;
    for(i=0; i<n; i++) {
        for(j=i+1; j<n; j++) {
            if(x[i]>x[j]) {
                Show(n,x,i,j);
                SwapInt(x[i],x[j]);
            }
        }
    }
    Show(n,x,-1,-1);
}

int main(void)
{
    int x[20]={17,10,13,14,15,6,3,2,9,8,18,7,12,11,16,4,1,20,5,19};
    FsOpenWindow(16,16,400,400,1);
    BubbleSort(20,x);
    return 0;
}