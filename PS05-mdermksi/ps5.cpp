#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "fssimplewindow.h"
#include "ysglfontdata.h"

const double MD_PI = 3.14159265;
const double friction = 20;
const int scale = 10; //pixels per inch
const int table_width = 103;
const int table_height = 59;
const double hole_radius = 2.5;
const double ball_radius = 1.5;
const double base_velocity = 400.0;

void InitializePosAndVel(int &hole_x, int &hole_y, double &target_x, double &target_y, double &target_vx,
    double &target_vy, double &cue_x, double &cue_y, double &cue_vx, double &cue_vy)
{
    int effective_width_hole = table_width - 2*hole_radius;
    int effective_height_hole = table_height - 2*hole_radius;
    int effective_width_ball = table_width - 2*ball_radius;
    int effective_height_ball = table_height - 2*ball_radius;
    hole_x = ( effective_width_hole - ( rand() % (effective_width_hole/3) ) + hole_radius ) * scale;
    hole_y = ( rand() % effective_height_hole + hole_radius ) * scale;
    target_x = ( effective_width_ball/3 + ( rand() % (effective_width_ball/3) ) + ball_radius ) * scale;
    target_y = ( rand() % effective_height_ball + ball_radius ) * scale;
    target_vx = 0.0;
    target_vy = 0.0;
    cue_x = ( ( rand() % (effective_width_ball/3) ) + ball_radius ) * scale;
    cue_y = ( rand() % effective_height_ball + ball_radius ) * scale;
    cue_vx = 0.0;
    cue_vy = 0.0;
}

void DrawTable( void )
{
    glColor3ub(80, 161, 76);
    glBegin(GL_QUADS);
    glVertex2i(0,0);
    glVertex2i(0,table_height*scale);
    glVertex2i(table_width*scale,table_height*scale);
    glVertex2i(table_width*scale,0);
    glEnd();
}

void WriteInstructions( void )
{
    char title[] = "-- POOL GOLF --";
    char goal[] = "The goal of this game is to get the red target ball in the hole.";
    char rule1[] = "Each ball successfully pocketed will increase your score by 1.";
    char instruction1[] = "Aim with the mouse and click to shoot.";
    char instruction2[] = "The up and down arrows will increase/lower your power percent.";
    char presskey[] = "- Press enter to begin -";
    glColor3ub(255, 255, 255);
    glRasterPos2i(table_width*scale/3+10, 100);
    YsGlDrawFontBitmap20x32(title);
    glRasterPos2i(120, 160);
    YsGlDrawFontBitmap12x16(goal);
    glRasterPos2i(120, 200);
    YsGlDrawFontBitmap12x16(rule1);
    glRasterPos2i(120, 240);
    YsGlDrawFontBitmap12x16(instruction1);
    glRasterPos2i(120, 280);
    YsGlDrawFontBitmap12x16(instruction2);
    glRasterPos2i(table_width*scale/3 - 30, 400);
    YsGlDrawFontBitmap16x20(presskey);
}

void DrawCircle( int cx, int cy, double rad, int r, int g, int b )
{
    glColor3ub(r,g,b);
    glBegin(GL_TRIANGLE_FAN);
    for( int i = 0; i < 360; i += 15 ) {
        double angle = i * MD_PI / 180;
        double x = cx + rad * cos( angle );
        double y = cy + rad * sin( angle );
        glVertex2d(x, y);
    }
    glEnd();
}

void DrawLine(int mouse_x, int mouse_y, double cue_x, double cue_y, int game_state)
{
    if( game_state == 1 ) {
        return;
    }

    double angle = atan2(cue_y - (double)mouse_y, cue_x - (double)mouse_x);
    double length = 1000.0;
    double far_x = cue_x + length * cos(angle);
    double far_y = cue_y + length * sin(angle);
    glColor3ub( 255, 255, 255 );
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
    glBegin(GL_LINE_STRIP);
    glVertex2d(cue_x, cue_y);
    glVertex2d(far_x, far_y);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

void WriteText( int score, int power )
{
    glColor3ub(255,255,255);
    char score_str[10];
    char power_str[12];
    sprintf(score_str, "Score: %d", score);
    sprintf(power_str, "Power: %d%%", power);
    glRasterPos2i(5,25);
    YsGlDrawFontBitmap16x20(score_str);
    glRasterPos2i(5,table_height*scale-5);
    YsGlDrawFontBitmap16x20(power_str);
}

void MoveBall( double &cx, double &cy, double &vx, double &vy, double dt )
{
    double angle = atan2(vy, vx);
    double velocity = sqrt(pow(vx, 2) + pow(vy, 2));

    //Apply friction and move ball
    if( velocity >= (friction * dt)) {
        velocity = velocity - friction * dt;
    }else if(velocity <= (-1*friction*dt)){
        velocity = velocity + friction * dt;
    } else {
        velocity = 0.0;
    }

    vx = velocity * cos(angle);
    vy = velocity * sin(angle);
    cx = cx + vx * dt;
    cy = cy + vy * dt;
    
    //Check if hitting wall
    if(cx <= ball_radius*scale && vx <= 0) {
        vx = -vx;
    } else if(cy <= ball_radius*scale && vy <=0) {
        vy = -vy;
    } else if(cy >= ((table_height-ball_radius)*scale) && vy >= 0) {
        vy = -vy;
    } else if(cx >= ((table_width-ball_radius)*scale) && vx >= 0) {
        vx = -vx;
    }
}

void CalculateCollision( double cue_x, double cue_y, double &cue_vx, double &cue_vy,
    double target_x, double target_y, double &target_vx, double &target_vy)
{
    double dx = target_x - cue_x;
    double dy = target_y - cue_y;
    double d = sqrt(dx*dx + dy*dy);

    if( d <= ball_radius*2.0*scale) {
        double rvx = target_vx - cue_vx;
        double rvy = target_vy - cue_vy;
        double dot = rvx*dx + dy*rvy;
        if((d<-0.000001 || 0.000001<d) && dot<0.0) {
            double nx = dx/d;
            double ny = dy/d;

            double k1 = target_vx*nx+target_vy*ny;
            double k0 = cue_vx*nx+cue_vy*ny;

            cue_vx=cue_vx + nx*(k1-k0);
            cue_vy=cue_vy + ny*(k1-k0);

            target_vx=target_vx - nx*(k1-k0);
            target_vy=target_vy - ny*(k1-k0);
        }
    }
}

int CheckInHole( int hole_x, int hole_y, int target_x, int target_y )
{
    double dx = hole_x - target_x;
    double dy = hole_y - target_y;
    double dist = sqrt(dx*dx + dy*dy);
    if(dist <= hole_radius*scale)
    {
        return 1;
    }
    return 0;
}

int main ( void )
{
    srand((int)time(nullptr));
    FsOpenWindow(0, 0, table_width*scale, table_height*scale, 1);

    //Initialize positions
    int hole_x, hole_y;
    double target_x, target_y, target_vx, target_vy, cue_x, cue_y, cue_vx, cue_vy;
    InitializePosAndVel(hole_x, hole_y, target_x, target_y, target_vx, 
        target_vy, cue_x, cue_y, cue_vx, cue_vy);
    int game_state = 0;
    int score = 0;
    int power = 50;

    printf("X: %lf, Y: %lf\n", target_x, target_y);

	FsPassedTime();

    while(1) {
        FsPollDevice();
        auto key = FsInkey();
        if( FSKEY_ENTER == key || FSKEY_ESC == key ) {
            break;
        }
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        DrawTable();
        WriteInstructions();
        FsSwapBuffers();
        FsSleep(25);
    }

    while(1){
        FsPollDevice();

        auto key = FsInkey();
        if( FSKEY_ESC == key ) {
            break;
        } else if( FSKEY_UP == key && power < 100 ) {
            power += 10;
        } else if( FSKEY_DOWN == key && power > 0 ) {
            power -= 10;
        }
        int lb,mb,rb,mx,my;
		auto evt=FsGetMouseEvent(lb,mb,rb,mx,my);

        if( evt == FSMOUSEEVENT_LBUTTONDOWN && game_state == 0) {
            game_state = 1;
            double angle = atan2(cue_y - (double)my, cue_x - (double)mx);
            cue_vx = ((double)power * base_velocity / 100.0) * cos(angle);
            cue_vy = ((double)power * base_velocity / 100.0) * sin(angle);
            
        }
        if(game_state == 1 && cue_vx == 0 && cue_vy == 0 && target_vx == 0 && target_vy == 0) {
            game_state = 0;
        }

        auto ms=FsPassedTime();
        double dt = (double)ms/1000.0;
        MoveBall(cue_x, cue_y, cue_vx, cue_vy, dt);
        MoveBall(target_x, target_y, target_vx, target_vy, dt);
        CalculateCollision(cue_x, cue_y, cue_vx, cue_vy, target_x, target_y,
            target_vx, target_vy);
        if(CheckInHole(hole_x, hole_y, target_x, target_y) == 1) {
            ++score;
            InitializePosAndVel(hole_x, hole_y, target_x, target_y, target_vx, 
                target_vy, cue_x, cue_y, cue_vx, cue_vy);
        }
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        DrawTable();
        DrawCircle( hole_x, hole_y, hole_radius*scale, 0, 0, 0 );
        DrawCircle( target_x, target_y, ball_radius*scale, 255, 0, 0 );
        DrawCircle( cue_x, cue_y, ball_radius*scale, 255, 255, 255 );
        DrawLine( mx, my, cue_x, cue_y, game_state );
        WriteText( score, power );
        FsSwapBuffers();
        FsSleep(5);
    }
    return 0;
}