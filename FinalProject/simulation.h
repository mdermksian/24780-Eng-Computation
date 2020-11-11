/**
 * Created by M. Dermksian 2019-11-16
 * 
 * simulation.h defines a singleton simulation class which is
 * responsible for running the entire application, and storing
 * information about its state other global variables.
 * 
 */

#ifndef __SIMULATION_H_IS_INCLUDED__
#define __SIMULATION_H_IS_INCLUDED__

#include "gui.h" //TO BE CHANGED - IMPORT ALL ACTUAL LIBRARIES ONCE COMPLETE
#include "Obstacle.h"

const int BMP_W = 60;
const int BMP_H = 60;

class Simulation {
private:
    int state;
    GUI *gui;
    // Obstacles obstacles;
    // Robot robot;
    // Pathfinder pathfinder;
public:
    int bitmap[BMP_W][BMP_H];
    Simulation();
    ~Simulation();
    void clear(void);
    int *getBitmap(void);
    void wipeBitmap(void);
    void setBitmap(int bmp[BMP_W][BMP_H]);
    void setBitmap(int value, int x, int y);
    void setState(int newState);
    int getState(void);
    void run(void);
    void draw(void);
};

#endif