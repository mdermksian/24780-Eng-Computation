/**
 * Implementation of the Simulation class
 */
#include "simulation.h"
#include "fssimplewindow.h"
#include <stdio.h>

Simulation::Simulation() {
    wipeBitmap();
    gui = new GUI();
}

Simulation::~Simulation() {
}

// void Simulation::clear(void) {
//     if(nullptr != bitmap) {
//         delete [] bitmap;
//         bitmap = nullptr;
//     }
// }

// int **Simulation::getBitmap(void) {
//     return bitmap;
// }

void Simulation::wipeBitmap(void) {
    for(int i = 0; i < BMP_W; ++i) {
        for(int j = 0; j < BMP_H; ++j) {
            bitmap[i][j] = 0;
        }
    }
}

void Simulation::setBitmap(int bmp[BMP_W][BMP_H]) {
    for(int i = 0; i < BMP_W; ++i) {
        for(int j = 0; j < BMP_H; ++j) {
            bitmap[i][j] = bmp[i][j];
        }
    }
}

void Simulation::setBitmap(int value, int x, int y) {
    bitmap[x][y] = value;
}

void Simulation::setState(int newState) {
    state = newState;
}

int Simulation::getState(void) {
    return state;
}

void Simulation::run(void) {
    FsOpenWindow(0,0,800,600,1);
    gui->Initialize(BMP_W, BMP_H);
    while(1) {
        FsPollDevice();
        auto key = FsInkey();
        // int newState = gui->handleKeyPress(key);
        int lb,mb,rb,mx,my;
        FsGetMouseEvent(lb,mb,rb,mx,my);
        // if(newState != 0){
        //     setState(newState);
        // }
        if(getState() == -1) {
            break;
        }
        gui->Run(bitmap);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        draw();
        FsSwapBuffers();
        FsSleep(10);
    }
}

void Simulation::draw(void) {
    gui->Draw(bitmap);
    // obstacle.draw();
    // robot.draw();
    // pathfinder.draw();
}