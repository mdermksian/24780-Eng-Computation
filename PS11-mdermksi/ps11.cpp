#include <stdio.h>
#include <math.h>
#include "fssimplewindow.h"

const double YsPi=3.1415927;

double MeasureCornerAngle(int nCoord,const double coord[],int index)
{
	if(index < 1 || index>nCoord-2){
		return 0.0;
	}

	double vector1[2] = {coord[2*(index)] - coord[2*(index-1)], coord[2*(index)+1] - coord[2*(index-1)+1]};
	double vector2[2] = {coord[2*(index+1)] - coord[2*(index)], coord[2*(index+1)+1] - coord[2*(index)+1]};
	double dot = vector1[0]*vector2[0] + vector1[1]*vector2[1];
	double len_v1 = sqrt(pow(vector1[0], 2) + pow(vector1[1], 2));
	double len_v2 = sqrt(pow(vector2[0], 2) + pow(vector2[1], 2));
	double angle = acos(dot/(len_v1*len_v2));
	return angle;
}

void DrawLineSegments(int nCoord,const double coord[])
{

	int color_ind = 0;
	int r[3] = {255, 0, 0};
	int g[3] = {0, 255, 0};
	int b[3] = {0, 0, 255};

	for(int i = 0; i < nCoord-1; ++i) {
		if(MeasureCornerAngle(nCoord, coord, i)>60*YsPi/180){
			glColor3ub(r[color_ind], g[color_ind], b[color_ind]);
			++color_ind;
		}else{
			glColor3ub(0,0,0);
			color_ind = 0;
		}
		glBegin(GL_LINES);
		glVertex2d(coord[2*i], coord[2*i+1]);
		glVertex2d(coord[2*(i+1)], coord[2*(i+1)+1]);
		glEnd();
	}
}

int main(void)
{
	const int nCoord=16;
	const double coord[nCoord*2] =
	{
		747.000000, 171.000000,
		297.000000, 171.000000,
		279.441871, 172.729325,
		262.558491, 177.850842,
		246.998679, 186.167735,
		233.360390, 197.360390,
		  6.167735, 426.998679,
		 24.875801, 439.499009,
		 69.875801, 394.499009,
		 88.583867, 406.999340,
		265.180195, 229.180195,
		271.999340, 223.583867,
		279.779246, 219.425421,
		288.220936, 216.864662,
		297.000000, 216.000000,
		747.000000, 216.000000
	};

	for(int i=1; i<nCoord-1; i++)
	{
		printf("Corner %d  Angle %lf\n",i,MeasureCornerAngle(nCoord,coord,i)*180.0/YsPi);
	}

	FsOpenWindow(16,16,800,600,1);

	int key=FSKEY_NULL;
	while(FSKEY_ESC!=key)
	{
		FsPollDevice();
		key=FsInkey();

		glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
		DrawLineSegments(nCoord,coord);
		FsSwapBuffers();

		FsSleep(20);
	}

	return 0;
}
