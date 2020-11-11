//
//  gui.cpp
//  Final Project
//
//  Created by Dan Izadi on 11/10/19.
//  Copyright © 2019 CMU. All rights reserved.
//

#define GL_SILENCE_DEPRECATION
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "gui.h"


void Element:: drawBoundaries(int y)
{
    glColor3ub(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x,y);
    glVertex2i(x,y+h);
    glVertex2i(x+w,y+h);
    glVertex2i(x+w,y);
    glEnd();
}

bool Element::userClick(int mx, int my, int preslb, int prevlb, int y)
{
	int lb = (preslb - prevlb) == 1 ? 1 : 0;

    if (lb ==1)
    {
        if (mx > x && my < x+w)
        {
            if (my>y && my< y+h)
            {
                return true;
            }
        }
    }
    return false;
}

void ObstacleButton:: Draw(void)
{
    drawBoundaries(y);
    
    glColor3ub(255*state,0,0);
    glBegin(GL_QUADS);
    glVertex2i(center_x-w/3,center_y-h/3);
    glVertex2i(center_x+w/3,center_y-h/3);
    glVertex2i(center_x+w/3,center_y+h/6);
    glVertex2i(center_x-w/3,center_y+h/6);
    glEnd();
    glRasterPos2i(x+10,y+h-10);
    YsGlDrawFontBitmap8x12("Obstacle");
}

void ObstacleButton:: Action(void)
{
    if (state ==0)
    {
        state = 1;
    }
    else if (state==1)
    {
        state = 0;
    }
}

void StartEndButton:: Draw(void)
{
    drawBoundaries(y);
    
    if (state==1)
    {
        // green flag for setting start point state
        glColor3ub(0,255,0);
    }
    else if (state == 2 || state==3)
    {
        // red flag for setting end point state
        glColor3ub(255,0,0);
    }
    else
    {
        // grey flag otherwise
        glColor3ub(100,100,100);
    }
    
    glBegin(GL_TRIANGLES);
    glVertex2i(center_x-w/6,center_y);
    glVertex2i(center_x+w/6,center_y-h/6);
    glVertex2i(center_x-w/6,center_y-h/3);
    glEnd();
    
    glColor3ub(0,0,0);
    glBegin(GL_LINE_STRIP);
    glVertex2i(center_x-w/6, center_y+h/4);
    glVertex2i(center_x-w/6, center_y-h/4);
    glEnd();
    glRasterPos2i(x+10,y+h-10);
    if (state <2)
    {
        YsGlDrawFontBitmap8x12("Start");
    }
    else if (state==2)
    {
        YsGlDrawFontBitmap8x12("End");
    }
    else
    {
        YsGlDrawFontBitmap8x12("Confirm Start/End");
    }

    
}

std::vector<int> StartEndButton::Action(int bitmap[60][60], int mx, int my, int lb, int dx, int dy)
{
    if (lb ==1)
    {
        int bitX = mx/dx;
		int bitY = my/dy;
        
        if (state == 1)
        {
            printf("Start: global coord (%d,%d) bitmap coord (%d, %d)\n", mx, my, bitX, bitY);
        }
        else if (state==2)
        {
            printf("End: global coord (%d,%d) bitmap coord (%d, %d)\n", mx, my, bitX, bitY);
        }

		return { bitX, bitY, state };
    }
	return { -1, -1, -1 };
}

void StopPlayButton::Draw()
{
    drawBoundaries(y);
    
    if (state < 1) // assuming 2 is the state when its playing change if needed
    {
        glColor3ub(0,0,0);
        glBegin(GL_TRIANGLES);
        glVertex2i(center_x-w/6,center_y+h/6);
        glVertex2i(center_x+w/6,center_y);
        glVertex2i(center_x-w/6,center_y-h/6);
        glEnd();
        glRasterPos2i(x+10,y+h-10);
        YsGlDrawFontBitmap8x12("Play");
    }
    else
    {
        glColor3ub(0,0,0);
        glBegin(GL_QUADS);
        glVertex2i(center_x-w/6,center_y-h/6);
        glVertex2i(center_x+w/6,center_y-h/6);
        glVertex2i(center_x+w/6,center_y+h/6);
        glVertex2i(center_x-w/6,center_y+h/6);
        glEnd();
        glRasterPos2i(x+10,y+h-10);
        YsGlDrawFontBitmap8x12("Stop");
    }
}

void StopPlayButton::Action(void)
{
    if (state==0)
    {
        state=1;
    }
    else if (state ==1)
    {
        state=0;
    }
}

void ImportButton::Draw()
{
    drawBoundaries(y);
    
    glColor3ub(0,0,0);
    glBegin(GL_TRIANGLES);
    glVertex2i(center_x,center_y+h/10);
    glVertex2i(center_x+w/6,center_y);
    glVertex2i(center_x,center_y-h/10);
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    glVertex2i(center_x+w/6,center_y);
    glVertex2i(center_x-w/6,center_y);
    glEnd();
    glRasterPos2i(x+10,y+h-10);
    YsGlDrawFontBitmap8x12("Import");
}


void ImportButton::Action(int bitmap[60][60])
{
    TextInput input("Input the file name you'd like to open:");
    if(input.Run()){
        FILE* fp = fopen(input.input.c_str(), "r");
        if (fp == NULL) {
            printf("Unable to open file.\n");
            return;
        }
        char line[256];
        int line_num = 0;
        while (fgets(line, 62, fp) != NULL)
        {
            for (int i = 0; i < 60; ++i) {
                bitmap[i][line_num] = line[i] - '0';
            }
            ++line_num;
            if (line_num == 60) {
                break;
            }
        }
        fclose(fp);
        printf("Successfully imported %s .\n", input.input.c_str());
    }
}

void SaveButton::Draw()
{
    drawBoundaries(y);
    
    glColor3ub(0,0,0);
    glBegin(GL_QUADS);
    glVertex2i(center_x-w/4,center_y-h/6);
    glVertex2i(center_x+w/4,center_y-h/6);
    glVertex2i(center_x+w/4,center_y+h/6);
    glVertex2i(center_x-w/4,center_y+h/6);
    glEnd();
    
    glColor3ub(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(center_x-w/4,center_y-h/6);
    glVertex2i(center_x+w/4,center_y-h/6);
    glVertex2i(center_x+w/4,center_y-h/3);
    glVertex2i(center_x-w/4,center_y-h/3);
    glEnd();
    
    glRasterPos2i(x+10,y+h-10);
    YsGlDrawFontBitmap8x12("Save");
}

void SaveButton::Action(int bitmap[60][60])
{
    TextInput input("Input the file name you'd like to save to:");
    if(input.Run()){
        std::ofstream save_file;
        save_file.open(input.input.c_str());

        for (int rows = 0; rows < 60 + 1; rows++)
        {
            for (int columns = 0; columns < 60; columns++)
            {
                //printf("%d, %d \n", rows, columns);
                save_file << bitmap[columns][rows];
            }
            save_file << std::endl;

        }
        save_file.close();
        printf("Save File %s Successful.\n", input.input.c_str());

    }
}

void EraseButton::Draw(void)
{
    drawBoundaries(y);
    
    glColor3ub(255*state, 0,0);
    
    glBegin(GL_LINE_STRIP);
    glVertex2i(x+w-w/3,y+h-h/3);
    glVertex2i(x+w/3,y+h/3);
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    glVertex2i(x+w/3,y+h-h/3);
    glVertex2i(x+w-w/3,y+h/3);
    glEnd();

    glRasterPos2i(x+10,y+h-10);
    YsGlDrawFontBitmap8x12("Erase");
}

void EraseButton::Action(void)
{
    if (state==1)
    {
        state=0;
    }
    else if (state==0)
    {
        state=1;
    }
}

void HelpButton::Draw(void)
{
    drawBoundaries(y);
    glRasterPos2i(x+w/3,y+h/3);
    YsGlDrawFontBitmap16x20("Help");
}

void HelpButton::Action(void)
{
    if (state==0)
    {
        state=1;
        TextInput input("Welcome to the PathFinder Application.\nHow to use PathFinder:\n1. To start building a custom environment from scratch, right click on the Obstacle button \non the top right of the window. This will allow you to draw a 2D representation of the \nenvironment that you would like to simulate.\n2.  Simply click and drag within the boundaries of the workspace to create obstacles.\n3.  Once you are finished with setting the workspace environment, click on \nthe obstacle button again.\n4.  Now you are ready to establish the start and end locations in your environment.  Click \non the Start button, then click any free location in your workspace to establish it as \nthe start location for the simulation.\n5.  Click on the Start button again.  Now you should see that the Start button has changed \nto an End button.  Click on any free location in your workspace to establish it as the end \nlocation for the simulation.  Click the End button once you are finished setting the end \nlocation.\n6.  Now you are ready to start the simulation.  Click the Play button to begin a 3D \nrendering of a mobile robot navigating through the workspace from your start location \nto your end location.\n7.  If you would like to pause the simulation, click the Stop button.  To resume, \nclick the Play button again.\n8.  If you would like to save your environment to a text file, click on the Save button.  \nYou will be prompted to create a filename, then your file will save in the directory under\n that name.\n9.  If you would like to import a saved environment to the application, click on the \nImport button.  You will be prompted to type the filename you wish to access, then that \nfile will create your environment.\n10.  If you need to erase obstacles, click on the erase button.  Then you can click the \nobstacles you would like to erase, and those obstacles will be converted to free spaces.\nYou can the ESC key to return to the application window. ");
        
        input.Show();
    }
    else if (state==1)
    {
        state=0;
    }
}

TextInput::TextInput(){
    input = "";
    prompt = "";
}
TextInput::TextInput(std::string seedPrompt){
    input = "";
    prompt = seedPrompt;
}
TextInput::~TextInput(){
}
bool TextInput::Show(){
    std::string input = "";
    
    while(1){
        FsPollDevice();
        int key = FsInkey();
        if(key == FSKEY_ESC){
            return false;
        }

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glColor3ub(200,200,200);
        glBegin(GL_QUADS);
        glVertex2i(0,0);
        glVertex2i(0,winHei);
        glVertex2i(winWid,winHei);
        glVertex2i(winWid,0);
        glEnd();
        glColor3ub(0,0,0);
        int line = 1;
        auto str = prompt.c_str();
        glRasterPos2d(50,20*line);
        for (int i=0; str[i] != 0 ;i++)
        {
            if (str[i] == '\n'){
                ++line;
                glRasterPos2d(50,20*line);
            } else {
                char temp[] = {str[i],0};
                YsGlDrawFontBitmap8x12(temp);
            }
        }
        //YsGlDrawFontBitmap16x20(prompt.c_str());
 
        FsSwapBuffers();
        FsSleep(10);
    }
}

bool TextInput::Run(void){
    std::string input = "";
    while(1){
        FsPollDevice();
        int key = FsInkey();
        if(key == FSKEY_ESC){
            return false;
        }
        if(key == FSKEY_ENTER){
            this->input = input;
            break;
        }
        if(key == FSKEY_BS && input.length() > 0){
            input.pop_back();
        }
        int c = FsInkeyChar();
        if(c && isprint(c)) {
            input.push_back(c);
        }
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glColor3ub(200,200,200);
        glBegin(GL_QUADS);
        glVertex2i(0,0);
        glVertex2i(0,winHei);
        glVertex2i(winWid,winHei);
        glVertex2i(winWid,0);
        glEnd();
        glColor3ub(0,0,0);
        glRasterPos2d(50,50);
        YsGlDrawFontBitmap16x20(prompt.c_str());
        glRasterPos2d(50,100);
        YsGlDrawFontBitmap16x20(input.c_str());
        switch(time(NULL) % 2){
            case 0:
                YsGlDrawFontBitmap16x20("_");
                break;
            case 1:
                YsGlDrawFontBitmap16x20("|");
                break;
        }
        FsSwapBuffers();
        FsSleep(10);
    }
    return true;
}


GUI::GUI(){
    obst = new ObstacleButton();
    flag = new StartEndButton();
    play = new StopPlayButton();
    import = new ImportButton();
    save = new SaveButton();
    help = new HelpButton();
    erase = new EraseButton();
    
}

void GUI:: Initialize(int m_, int n_)
{
    m = m_;
    n = n_;
    dx = (winWid-200)/n;
    dy = winHei/m;
	prevlb = 0;
	startX = -1;
	startY = -1;
	endX = -1;
	endY = -1;
}

void GUI:: Draw(int bitmap[60][60])
{
    obst->Draw();
    flag->Draw();
    play->Draw();
    import->Draw();
    save->Draw();
    help->Draw();
    erase->Draw();
}

void GUI::Run(int bitmap[60][60])
{
    int preslb,mb,rb,mx,my;
    FsGetMouseEvent(preslb,mb,rb,mx,my);
    // Draw(bitmap);
    if (true == obst->userClick(mx, my, preslb, prevlb, obst->y))
    {
		obst->Action();
        printf("Obstacle Button Pressed\n");
    }
    else if (true == flag->userClick(mx,my, preslb, prevlb, flag->y))
    {
        printf("Start/ End Location Button Pressed\n");
        if (flag->state==0)
        {
            flag->state=1;
        }
        else if (flag->state==1)
        {
            flag->state=2;
        }
        else if (flag->state==3)
        {
            confirm = true;
            flag->state=4;
        }
	}
    else if (true == play->userClick(mx, my, preslb, prevlb, play->y))
    {
        printf("Play/ Stop Button Pressed\n");
        if (flag->state==4)
        {
            if (play->state ==0)
            {
                play->state=1;
            }
            else if (play->state==1)
            {
                play->state=0;
            }
        }
    }
    else if (true == import->userClick(mx, my, preslb, prevlb, import->y))
    {
        printf("Import Button Pressed\n");
        import->Action(bitmap);
    }
    else if (true == save->userClick(mx,my, preslb, prevlb,save->y))
    {
        printf("Save Button Pressed\n");
        save->Action(bitmap);
    }
    else if (true == help->userClick(mx,my, preslb, prevlb,help->y))
    {
        printf("Help Button Pressed\n");
        help->Action();
    }
    else if (true == erase->userClick(mx,my, preslb, prevlb,erase->y))
    {
        printf("Erase Button Pressed\n");
        erase->Action();
    }
    
    
    if (flag->state>0 && flag->state<3)
    {
        if (preslb == 1 && mx >0 && my >0 && mx<winWid-200 && my < winHei)
        {
            std::vector<int> output = flag->Action(bitmap, mx, my, preslb, dx, dy);
			if (output[2] == 1) {
				startX = output[0];
				startY = output[1];
			}
			else if (output[2] == 2) {
				endX = output[0];
				endY = output[1];
			}

        }
    }
    
    if(endX >= 0 && confirm == false)
    {
        flag->state = 3;
    }
    
    if (play->state == 1 || flag->state == 1|| flag->state==2|| erase-> state == 1)
    {
        obst->state=0;
    }
    
    prevlb=preslb;
}
