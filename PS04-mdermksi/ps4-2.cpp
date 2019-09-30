#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "fssimplewindow.h"

const double scale = 10.0;
const int n_obst = 5;
const double MD_PI = 3.14159265;
const double g = 9.81;
const double dt = 0.025;

void DrawCannon(int x, int y, double angle)
{
    glColor3ub(0, 0, 255);

    glBegin(GL_QUADS);
    glVertex2i(x*scale, (-y+60)*scale);
    glVertex2i(x*scale + 6, (-y+60)*scale);
    glVertex2i(x*scale + 6, (-y+60)*scale - 6);
    glVertex2i(x*scale, (-y+60)*scale - 6);
    glEnd();

    glBegin(GL_LINES);
    glVertex2i(x*scale+3, (-y+60)*scale-3);
    glVertex2d((x+2*cos(angle))*scale+3, (-y+60-2*sin(angle))*scale-3);
    glEnd();

}

void DrawBall(double x, double y, int state)
{
    if(state == 0){
        return;
    }
    double r = 3.0;
    glColor3ub(255, 0, 0);
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i < 360; i+=30) {
        double angle = (double)i*MD_PI/180.0;
        glVertex2d(x*scale+r*cos(angle), -scale*y+r*sin(angle)+60*scale);
    }
    glEnd();
}

void DrawTrajectory(double historic_x[], double historic_y[], int history_count)
{
    glColor3ub(0,0,0);
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < history_count; ++i){
        glVertex2d(scale * historic_x[i], -scale*(historic_y[i] - 60));
    }

    glEnd();
}

void DrawObstacle(int x, int y, int h, int w, int state)
{
    glColor3ub(0, 255, 0);
    if(state == 1){
        glBegin(GL_QUADS);
    } else {
        glBegin(GL_LINE_LOOP);
    }
    glVertex2i(x*scale, (-y+60)*scale);
    glVertex2i(x*scale, (-y-h+60)*scale);
    glVertex2i((x+w)*scale, (-y-h+60)*scale);
    glVertex2i((x+w)*scale, (-y+60)*scale);

    glEnd();
}

void DrawTarget(double x, double y)
{
    glColor3ub(255, 0, 0);
    glBegin(GL_QUADS);
    glVertex2d(x*scale, (-y+60)*scale);
    glVertex2d((x+5)*scale, (-y+60)*scale);
    glVertex2d((x+5)*scale, (-y-5+60)*scale);
    glVertex2d(x*scale, (-y-5+60)*scale);
    glEnd();
}

int CheckCollision(double cb_x, double cb_y, int obst_x[],
    int obst_y[], int obst_h[], int obst_w[], int obst_state[],
    double target_x, double target_y)
{
    //Check cannon ball and obstacles
    for(int i = 0; i < n_obst; ++i){
        if(obst_state[i]){
            if(cb_x >= obst_x[i] && cb_x <= obst_x[i]+obst_w[i] && cb_y >= obst_y[i] && cb_y <= obst_y[i] + obst_h[i]) {
                return i;
            } 
        }
    }

    if(cb_x >= target_x && cb_x <= target_x + 5 && cb_y >= target_y && cb_y <= target_y + 5) {
        return n_obst;
    }

    return -1;
}

int main(void)
{
    srand((int)time(nullptr));
    FsOpenWindow(0,0,800,600,1);

    //Variable initialization
    double target_x = 75.0;
    double target_y = 60.0;
    double target_vy = -10.0;
    int cannon_x = 5;
    int cannon_y = 5;
    double cannon_angle = 30.0 * ( MD_PI / 180.0 );
    double cb_x, cb_y, cb_vx, cb_vy;
    double cb_r = 3;
    int cb_state = 0;
    int obst_x[n_obst], obst_y[n_obst], obst_h[n_obst],
        obst_w[n_obst], obst_state[n_obst];

    double historic_x[512];
    double historic_y[512];
    int history_count = 0;

    for( int i = 0; i < n_obst; ++i ) {
        obst_h[i] = (rand() % 8 + 8);
        obst_w[i] = (rand() % 8 + 8);
        obst_x[i] = rand() % (75 - obst_w[i]) + 5;
        obst_y[i] = rand() % (55 - obst_h[i]) + 5;
        obst_state[i] = 1;
    }

    //Main Game Loop
    while(1) {
        FsPollDevice();

        //Listen for keys & compute positions/velocities
        auto key = FsInkey();
        if(FSKEY_SPACE == key && cb_state == 0) {
            history_count = 0;
            cb_state = 1;
            cb_x = 2*cos(cannon_angle)+5;
            cb_y = 2*sin(cannon_angle)+5;
            cb_vx = 40*cos(cannon_angle);
            cb_vy = 40*sin(cannon_angle);
        } else if(FSKEY_ESC == key) {
            break;
        }

        if(cannon_angle >= (MD_PI/2.0)) {
            cannon_angle = 0.0;
        } else {
            cannon_angle += MD_PI/180.0;
        }

        if(target_y <= 0) {
            target_y = 55;
        } else {
            target_y = target_y + target_vy*dt;
        }

        if(cb_state == 1) {
            historic_x[history_count] = cb_x;
            historic_y[history_count] = cb_y;
            ++history_count;
            cb_x = cb_x+cb_vx*dt;
            cb_y = cb_y+cb_vy*dt;

            cb_vx = cb_vx;
            cb_vy = cb_vy-g*dt;

            if(cb_x > 80 || cb_y < 0) {
                cb_state = 0;
            }
        }

        //Check for collision and handle winning or losing
        int collision_index = CheckCollision(cb_x, cb_y, obst_x, obst_y, 
            obst_h, obst_w, obst_state, target_x, target_y);

        if( collision_index > -1 && collision_index < n_obst) {
            obst_state[collision_index] = 0;
            cb_state = 0;
        } else if( collision_index == n_obst ) {
            printf("YOU WON!");
            break;
        }
        
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        //Draw objects
        for(int i = 0; i < n_obst; ++i) {
            DrawObstacle(obst_x[i], obst_y[i], 
                obst_h[i], obst_w[i], obst_state[i]);
            
        }
        DrawCannon(cannon_x, cannon_y, cannon_angle);
        DrawTrajectory(historic_x, historic_y, history_count);
        DrawBall(cb_x, cb_y, cb_state);
        DrawTarget(target_x, target_y);
        FsSwapBuffers();
        FsSleep(25);
    }

    return 0;
}