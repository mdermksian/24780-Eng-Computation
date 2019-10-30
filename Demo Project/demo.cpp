#include <stdio.h>
#include <string.h>
#include <chrono>
#include "fssimplewindow.h"
#include "yssimplesound.h"
#include "ysglfontdata.h"

using namespace std;

const int window_height = 768;
const int window_width = 768;
const int sample_bank_length = 8192;

void ToScreenCoords(int &sx, int &sy, int x, int y) {
    sx = x + window_width/2;
    sy = -(y + window_height/2) + window_height;
}

void DrawWave(int x[], int y[]) {
    glColor3ub(0, 255, 0);
    glBegin(GL_POINTS);
    for(int i = 0; i<sample_bank_length; ++i){
        int sx, sy;
        ToScreenCoords(sx, sy, x[i], y[i]);
        glVertex2i(sx, sy);
    }
    glEnd();
}

void DrawBackground(void) {
    glColor3ub(0,0,0);
    glBegin(GL_QUADS);
    glVertex2i(0,0);
    glVertex2i(window_width, 0);
    glVertex2i(window_width, window_height);
    glVertex2i(0, window_height);
    glEnd();
}

void DisplayText(char text1[], char text2[], char text3[], int ms){

    int l[3] = {0, 0, 0};
    int nl = 0;
    
    for(int i = 0; text1[i] != 0; ++i){
        l[0] = i;
    }
    for(int i = 0; text2[i] != 0; ++i){
        l[1] = i;
    }
    for(int i = 0; text3[i] != 0; ++i){
        l[2] = i;
    }
    for(int i = 0; i < 3; ++i){
        if(l[i] > 0){
            ++nl;
        }
    }

    int elapsed = 0;
    auto begin = chrono::high_resolution_clock::now();
    while(elapsed < ms){
        FsPollDevice();
        int key = FsInkey();
        if( key == FSKEY_ESC ) {
            break;
        }
        auto cur = chrono::high_resolution_clock::now();
        auto dur = cur - begin;
        elapsed = (int)chrono::duration_cast<chrono::milliseconds>(dur).count();
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        DrawBackground();
        glColor3ub(0, 255, 0);
        glRasterPos2i(window_width/2 - l[0]/2*16, window_height/2 - nl*20/2);
        YsGlDrawFontBitmap16x20(text1);
        if(l[1]){
            glRasterPos2i(window_width/2 - l[1]/2*16, window_height/2 - nl*20/2 + 25);
            YsGlDrawFontBitmap16x20(text2);
        }
        if(l[2]){
            glRasterPos2i(window_width/2 - l[2]/2*16, window_height/2 - nl*20/2 + 50);
            YsGlDrawFontBitmap16x20(text3);
        }
        FsSwapBuffers();
        FsSleep(25);
    }
}

int PlaySound(char fName[]){
    YsSoundPlayer player;
    YsSoundPlayer::SoundData wav;
    player.MakeCurrent();
    player.Start();
    
    int curNumSample = 0;
    int lastNumSample = 0;
    int sampleBank_x[sample_bank_length];
    int sampleBank_y[sample_bank_length];
    if( YSOK != wav.LoadWav(fName) ) {
        printf("Couldn't load file");
        return 1;
    }
    int num_channels = wav.GetNumChannel();
    int play_back_rate = wav.PlayBackRate();

    player.PlayOneShot(wav);

    auto begin = chrono::high_resolution_clock::now();

    while(YSTRUE==player.IsPlaying(wav)){
        FsPollDevice();
        int key = FsInkey();
        if( key == FSKEY_ESC ) {
            break;
        }
        auto cur = chrono::high_resolution_clock::now();  
        auto dur = cur - begin;
        double s = (double)std::chrono::duration_cast<std::chrono::milliseconds>(dur).count()/1000.0;
        if(s<0){ s = 0; }

        long long int curNumSample = wav.SecToNumSample(s, play_back_rate);
        long long int pastNumSample = (curNumSample - sample_bank_length) < 0 ? 0 : (curNumSample - sample_bank_length) ;
        
        for( int i = pastNumSample; i < curNumSample; ++i) {
            int idx = i - pastNumSample;
            sampleBank_x[idx] = wav.GetSignedValueRaw(0, i)/100;
            sampleBank_y[idx] = wav.GetSignedValueRaw(1, i)/100;
        }
        
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        DrawBackground();
        DrawWave(sampleBank_x, sampleBank_y);
        FsSwapBuffers();
        player.KeepPlaying();
        FsSleep(1);
    }

    player.End();

    return 0;
}

int main(void)
{
    FsChangeToProgramDir();
    FsOpenWindow(0, 0, window_width, window_height, 1);
    
    DisplayText("Hey everyone!","","", 3000);
    DisplayText("Did you know that many audio files","have two channels?","", 4000);
    DisplayText("There's L & R channels that you can ","map to X & Y to draw things with sound","", 5000);
    DisplayText("For example, here I'm drawing and","moving a line with just a single note","", 5000);
    PlaySound("./l-r.wav");
    DisplayText("","","", 1000);
    DisplayText("You can even draw and animate shapes!", "", "", 1500);
    PlaySound("./cube.wav");
    DisplayText("Or even better...", "", "", 2000);
    DisplayText("Make visual music!", "--------", "Enjoy the show", 3000);
    PlaySound("./planets_edit.wav");

    return 0;
}