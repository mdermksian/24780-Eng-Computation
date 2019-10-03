#include <time.h>
#include <math.h>
#include "fssimplewindow.h"

double const MD_PI = 3.14156265;

void DrawClock( int h, int m, int s )
{
    glColor3ub(0,0,0);

    //Draw outer circle
    glBegin(GL_LINE_STRIP);
    for( double i = 0; i <= 2*MD_PI; i += 0.01 ) {
        double x = 250 * cos(i);
        double y = 250 * sin(i);
        glVertex2d(x + 400, y + 300);
    }
    glEnd();

    //Draw hour/minute lines
    for( int i = 0; i < 60; ++i ) {
        double angle = (MD_PI / 30) * i;
        int length = (i%5 == 0) ? 40 : 10;
        glBegin(GL_LINES);
        int x1 = (240 - length)*cos(angle);
        int y1 = (240 - length)*sin(angle);
        int y2 = 240*sin(angle);
        int x2 = 240*cos(angle);
        glVertex2i(x1 + 400, y1 + 300);
        glVertex2i(x2 + 400, y2 + 300);
        glEnd();
    }

    //Draw hands
    double m_angle = (MD_PI/30) * double(m) - (MD_PI/2);
    double h_angle = ((MD_PI/6) * double(h)) + ((MD_PI/6/60) * double(m)) - (MD_PI/2);
    double s_angle = (MD_PI/30) * double(s) - (MD_PI/2);
    int m_length = 220;
    int h_length = 180;
    int s_length = 230;
    int x1 = 0;
    int y1 = 0;
    int m_x2 = m_length*cos(m_angle);
    int m_y2 = m_length*sin(m_angle);
    int h_x2 = h_length*cos(h_angle);
    int h_y2 = h_length*sin(h_angle);
    int s_x2 = s_length*cos(s_angle);
    int s_y2 = s_length*sin(s_angle);
    glBegin(GL_LINES);
    glVertex2i(x1+400,y1+300);
    glVertex2i(m_x2+400,m_y2+300);
    glVertex2i(x1+400,y1+300);
    glVertex2i(h_x2+400,h_y2+300);
    glColor3ub(255,0,0);
    glVertex2i(x1+400,y1+300);
    glVertex2i(s_x2+400,s_y2+300);
    glEnd();
     
}

int main( void )
{
    int h, m, s;
    FsOpenWindow(0,0,800,600,1);
    while(1){
        FsPollDevice();
        auto key = FsInkey();
        if(key == FSKEY_ESC) {
            break;
        }
        struct tm *localTime;
        time_t t = time(NULL);
        localTime = localtime(&t); 
        h = localTime->tm_hour;
        m = localTime->tm_min;
        s = localTime->tm_sec;
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        DrawClock(h, m, s);
        FsSwapBuffers();

        FsSleep(50);
    }
    return 0;
}