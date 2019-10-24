#include <stdio.h>
#include "fssimplewindow.h"

void DrawBitMap(char pattern[]){
    glBegin(GL_QUADS);
    for(int i = 0; pattern[i]!=0; ++i){
        int x = i % 16;
        int y = i/16;
        if('1' == pattern[i]){
            glColor3ub(0,0,0);
        } else {
            glColor3ub(255,255,255);
        }
        glVertex2i(x*16, y*16);
        glVertex2i(x*16+16, y*16);
        glVertex2i(x*16+16, y*16+16);
        glVertex2i(x*16, y*16+16);
    }
    glEnd();
}

char getCharFromPattern(int mx, int my, char pattern[]){
    int x = mx/16;
    int y = my/16;
    int i = y*16 + x;
    return pattern[i];
}

int main(void){
    char pattern[] = {
        "1111111111111111"
        "1.1.1.1.1.1.1.1."
        ".1.1.1.1.1.1.1.1"
        "1.1.1.1.1.1.1.1."
        ".1.1.1.1.1.1.1.1"
        "1.1.1.1.1.1.1.1."
        ".1.1.1.1.1.1.1.1"
        "1.1.1.1.1.1.1.1."
        ".1.1.1.1.1.1.1.1"
        "1.1.1.1.1.1.1.1."
        ".1.1.1.1.1.1.1.1"
        "1.1.1.1.1.1.1.1."
        ".1.1.1.1.1.1.1.1"
        "1.1.1.1.1.1.1.1."
        ".1.1.1.1.1.1.1.1"
        "1.1.1.1.1.1.1.1."
    };

    FsOpenWindow(0, 0, 300, 300, 0);

    int lb,mb,rb,mx,my;
    while(FSKEY_ESC != FsInkey()){
        FsPollDevice();
        if(FSMOUSEEVENT_MOVE == FsGetMouseEvent(lb,mb,rb,mx,my)){
            if(mx<16*16 && my<16*16){
                printf("%d %d %c\n", mx, my, getCharFromPattern(mx, my, pattern));
            }
        }
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        DrawBitMap(pattern);
        glFlush();
        FsSleep(10);
    }
}