#ifndef __GUI_H_IS_INCLUDED__
#define __GUI_H_IS_INCLUDED__


static int winWid = 800;
static int winHei = 600;
static int numElements = 7;

#include <vector>
#include <string>
#include <time.h>


class Element
{
public:
    int w = winWid -600; // 600x600 bitmap for viewport
    int x = winWid- w;
    int h = winHei/numElements;
    bool userClick(int mx, int my, int preslb, int prevlb, int y) ;
    void drawBoundaries(int y);
};

class HelpButton: public Element
{
public:
    void Draw(void);
    void Action(void);
    int elementIndex = 6;
    int y = elementIndex*h;
    int state = 0;
    //std::string help_message = "Welcome to the PathFinder Application.\nHow to use PathFinder:\n1. To start building a custom environment from scratch, right click on the “Obstacle” button on the top right of the window.  This will allow you to draw a 2D representation of the environment that you would like to simulate.\n2.  Simply click and drag within the boundaries of the workspace to create obstacles.\n3.  Once you are finished with setting the workspace environment, click on the obstacle button again.\n4.  Now you are ready to establish the “start” and “end” locations in your environment.  Click on the “Start” button, then click any free location in your workspace to establish it as the start location for the simulation.\n5.  Click on the “Start” button again.  Now you should see that the “Start” button has changed to an “End” button.  Click on any free location in your workspace to establish it as the end location for the simulation.  Click the “End” button once you are finished setting the end location.\n6.  Now you are ready to start the simulation.  Click the “Play” button to begin a 3D rendering of a mobile robot navigating through the workspace from your start location to your end location.\n7.  If you would like to pause the simulation, click the “Stop” button.  To resume, click the “Play” button again.\n8.  If you would like to save your environment to a text file, click on the “Save” button.  You will be prompted to create a filename, then your file will save in the directory under that name.\n9.  If you would like to import a saved environment to the application, click on the “Import” button.  You will be prompted to type the filename you wish to access, then that file will create your environment.\n10.  If you need to erase obstacles, click on the erase button.  Then you can click the obstacles you would like to erase, and those obstacles will be converted to free spaces.\n11.  If you need help, well, you already pressed the help button, so you know how to do that already.";
};

class EraseButton: public Element
{
public:
    void Draw(void);
    void Action(void);
    int elementIndex = 5;
    int y = elementIndex*h;
    int state=0;
};

class ObstacleButton: public Element
{
public:
    void Draw(void);
    void Action(void);
    int elementIndex = 0;
    int y = elementIndex*h;
    int center_x = (x+x+w)/2;
    int center_y = (y+y+h)/2;
    int state = 0;
};

class StartEndButton: public Element
{
public:
    void Draw(void);
	std::vector<int> Action(int bitmap[60][60], int mx, int my, int lb, int dx, int dy);
    int state = 0; // 0 is setting start, 1 is setting end
    int elementIndex = 1;
    int y = elementIndex*h;
    int center_x = (x+x+w)/2;
    int center_y = (y+y+h)/2;
};

class StopPlayButton: public Element
{
public:
    int state = 0 ;
    int elementIndex = 2;
    int y = elementIndex*h;
    void Draw(void);
    void Action(void);
    int center_x = (x+x+w)/2;
    int center_y = (y+y+h)/2;
};

class ImportButton: public Element
{
public:
    int elementIndex = 3;
    int y = elementIndex*h;
    void Draw(void);
    void Action(int bitmap[60][60]);
    int center_x = (x+x+w)/2;
    int center_y = (y+y+h)/2;
};

class SaveButton: public Element
{
public:
    int elementIndex = 4;
    int y = elementIndex*h;
    void Draw(void);
    void Action(int bitmap [60][60]);
    int center_x = (x+x+w)/2;
    int center_y = (y+y+h)/2;
};

class TextInput
{
public:
    TextInput();
    TextInput(std::string seedPrompt);
    ~TextInput();
    std::string prompt;
    std::string input;
    bool Run(void);
    bool Show(void);
};


class GUI
{
public:
    GUI();
    ObstacleButton *obst;
    StartEndButton *flag;
    StopPlayButton *play;
    ImportButton *import;
    SaveButton *save;
    EraseButton *erase;
    HelpButton *help;
    
    int dx, dy, m, n;
	int prevlb;
	int startX, startY, endX, endY;
    void Draw(int bitmap[60][60]); //Draws the Menu and Window
    void Initialize(int m_, int n_);
    void Run(int bitmap[60][60]);
    // int handleKeyPress(int key);
};

#endif
