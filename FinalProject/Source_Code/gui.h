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
    bool confirm = false;
    // int handleKeyPress(int key);
};

#endif
