#include <stdio.h>
#include <math.h>
#include "fssimplewindow.h"

double const MD_PI = 3.14156265;

void DrawClock( int h, int m )
{
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
    int m_length = 220;
    int h_length = 180;
    int x1 = 0;
    int y1 = 0;
    int m_x2 = m_length*cos(m_angle);
    int m_y2 = m_length*sin(m_angle);
    int h_x2 = h_length*cos(h_angle);
    int h_y2 = h_length*sin(h_angle);
    glBegin(GL_LINES);
    glVertex2i(x1+400,y1+300);
    glVertex2i(m_x2+400,m_y2+300);
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(x1+400,y1+300);
    glVertex2i(h_x2+400,h_y2+300);
    glEnd();
     
}

int main( void )
{
    printf("Enter a time:\n");
    int h, m;
    scanf("%d %d", &h, &m);
    if ( h > 12 || h < 1 || m > 59 || m < 0 ) {
        printf("I can't draw a time that doesn't exist");
        return 0;
    }
    FsOpenWindow(0,0,800,600,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    DrawClock(h, m);
    glFlush();
    while( (getchar()) != '\n' ); //Found this online, it clears the stdin buffer so that getchar() properly awaits a second input
    getchar();
    return 0;
}