/*
Robot and Trajectory Visualizer Pseudocode

- Mike Turski
*/
#include <stdio.h>
#include <vector>
#include "fssimplewindow.h"
#include <math.h>
using namespace std;

const double YsPi = 3.1415927;

#pragma region "3d Classes"

class CameraObject
{
public:
	double x, y, z;
	double h, p, b;

	double fov, nearZ, farZ;

	CameraObject() {
		Initialize();
	}

	void Initialize(void) {
		x = 0;
		y = 0;
		z = 0;
		h = 0;
		p = 0;
		b = 0;

		fov = YsPi / 6.0;  // 30 degree
		nearZ = 0.1;
		farZ = 20000.0;
	}

	void SetUpCameraProjection(void) {
		int wid, hei;
		double aspect;

		FsGetWindowSize(wid, hei);
		aspect = (double)wid / (double)hei;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(fov * 180.0 / YsPi, aspect, nearZ, farZ);
	}

	void SetUpCameraTransformation(void) {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotated(-b * 180.0 / YsPi, 0.0, 0.0, 1.0);  // Rotation about Z axis
		glRotated(-p * 180.0 / YsPi, 1.0, 0.0, 0.0);  // Rotation about X axis
		glRotated(-h * 180.0 / YsPi, 0.0, 1.0, 0.0);  // Rotation about Y axis
		glTranslated(-x, -y, -z);
	}

	void GetForwardVector(double& vx, double& vy, double& vz) {
		vx = -cos(p) * sin(h);
		vy = sin(p);
		vz = -cos(p) * cos(h);
	}
};

class OrbitingViewer
{
public:
	double h, p;
	double dist;
	double focusX, focusY, focusZ;

	OrbitingViewer() {
		Initialize();
	}

	void Initialize(void) {
		h = 0;
		p = 0;
		dist = 50.0;
		focusX = 0.0;
		focusY = 0.0;
		focusZ = 0.0;
	}

	void SetUpCamera(CameraObject& camera) {
		camera.h = h;
		camera.p = p;
		camera.b = 0.0;

		double vx, vy, vz;
		camera.GetForwardVector(vx, vy, vz);
		camera.x = focusX - vx * dist;
		camera.y = focusY - vy * dist;
		camera.z = focusZ - vz * dist;
	}
};


#pragma endregion "3d Classes"

class Point {
public:
	int x, y;

	Point() {
		x = 0;
		y = 0;
	}

	Point(const int& xIn, const int& yIn) {
		x = xIn;
		y = yIn;
	}
};

class Drawable {
public:
	int x, y;

	/*virtual void Draw(void);*/
};

class Robot : Drawable {
public:
	const double PI = 3.141592;
	const double cellWidth = 10;
	const double moveSpeed = 1;
	const double h = 4;
	const double t = 0.7;
	const double w = 6;
	const double z = 1.3;

	int index;
	double orientation, angle1, angle2, X, Y;

	bool atGoal;

	vector<Point> traj;

	Robot(const int& xIn, const int& yIn, const vector<Point> trajIn) {
		x = xIn;
		y = yIn;
		traj = trajIn;

		X = x * cellWidth + (cellWidth / 2.0);
		Y = y * cellWidth + (cellWidth / 2.0);
		orientation = 0;
		index = 0;
		atGoal = false;
	}

	~Robot() {
		// Clear traj vector
		//delete(&traj);
	}

	void UpdateTraj(const vector<Point> trajIn) {
		traj = trajIn;
	}

	void Move(void) {
		// Update robot position according to current trajectory
		if (atGoal == true) {
			orientation += 3;
		}
		else {
			double Xnext, Ynext;
			Xnext = traj[index].x * cellWidth + (cellWidth / 2.0);
			Ynext = traj[index].y * cellWidth + (cellWidth / 2.0);

			orientation = atan2(Y - Ynext, X - Xnext) * 180 / PI;

			if (X < Xnext) {
				X += moveSpeed;
				if (X > Xnext) {
					X = Xnext;
				}
			}
			else if (X > Xnext) {
				X -= moveSpeed;
				if (X < Xnext) {
					X = Xnext;
				}
			}
			if (Y < Ynext) {
				Y += moveSpeed;
				if (Y > Ynext) {
					Y = Ynext;
				}
			}
			else if (Y > Ynext) {
				Y -= moveSpeed;
				if (Y < Ynext) {
					Y = Ynext;
				}
			}

			if (Y == Ynext && X == Xnext) {
				if (index < traj.size()) {
					index++;
				}
			}
		}
	}

	void CheckGoal(void) {
		// Check to see if robot is at the end of its trajectory
		if (index == traj.size()) {
			atGoal = true;
		}
		else {
			atGoal = false;
		}
	}

	void Draw(void) {
		// Draw the robot and traj
		#pragma region "Draw Body"
		glColor3ub(140, 100, 100);
		glBegin(GL_QUADS);

		double trX = X + w * 0.5 * cos(orientation * PI / 180) - h * 0.5 * sin(orientation * PI / 180);
		double tlX = X - w * 0.5 * cos(orientation * PI / 180) - h * 0.5 * sin(orientation * PI / 180);
		double brX = X + w * 0.5 * cos(orientation * PI / 180) + h * 0.5 * sin(orientation * PI / 180);
		double blX = X - w * 0.5 * cos(orientation * PI / 180) + h * 0.5 * sin(orientation * PI / 180);

		double trY = Y + w * 0.5 * sin(orientation * PI / 180) + h * 0.5 * cos(orientation * PI / 180);
		double tlY = Y - w * 0.5 * sin(orientation * PI / 180) + h * 0.5 * cos(orientation * PI / 180);
		double brY = Y + w * 0.5 * sin(orientation * PI / 180) - h * 0.5 * cos(orientation * PI / 180);
		double blY = Y - w * 0.5 * sin(orientation * PI / 180) - h * 0.5 * cos(orientation * PI / 180);

		glVertex3d(trX, trY, z - t);
		glVertex3d(tlX, tlY, z - t);
		glVertex3d(blX, blY, z - t);
		glVertex3d(brX, brY, z - t);

		glVertex3d(trX, trY, z);
		glVertex3d(tlX, tlY, z);
		glVertex3d(blX, blY, z);
		glVertex3d(brX, brY, z);

		glVertex3d(trX, trY, z - t);
		glVertex3d(tlX, tlY, z - t);
		glVertex3d(tlX, tlY, z);
		glVertex3d(trX, trY, z);

		glVertex3d(brX, brY, z - t);
		glVertex3d(blX, blY, z - t);
		glVertex3d(blX, blY, z);
		glVertex3d(brX, brY, z);

		glVertex3d(tlX, tlY, z);
		glVertex3d(blX, blY, z);
		glVertex3d(blX, blY, z - t);
		glVertex3d(tlX, tlY, z - t);

		glVertex3d(trX, trY, z - t);
		glVertex3d(brX, brY, z - t);
		glVertex3d(brX, brY, z);
		glVertex3d(trX, trY, z);

		glEnd();
		#pragma endregion "Draw Body"

		DrawWheelL(trX, trY, orientation);
		DrawWheelL(tlX, tlY, orientation);
		DrawWheelR(brX, brY, orientation);
		DrawWheelR(blX, blY, orientation);

		double fX = X - w * 0.3 * cos(orientation * PI / 180);
		double fY = Y - w * 0.3 * sin(orientation * PI / 180);

		DrawLidar(fX, fY, orientation);

		glColor3ub(100, 0, 0);
		glBegin(GL_LINES);
		glVertex3d(X, Y, z);
		glVertex3d(X - w * 0.5 * cos(orientation * PI / 180), Y - w * 0.5 * sin(orientation * PI / 180), z);
		glEnd();
	}

	void DrawWheelL(const double& xIn, const double& yIn, const double& orientation) {
		glColor3ub(20, 20, 20);
		glBegin(GL_LINE_LOOP);

		double ws = 3; // Wheel diameter
		double wt = 1; // Wheel thickness
		ws = ws / 2;
		double zMid = 0.9;
		double r = 0.9;
		int nPoints = 200;
		for (int i = 0; i < nPoints; i++) {
			glVertex3d(xIn + r * cos(i * 2 * PI / nPoints) * cos(orientation * PI / 180), yIn + r * cos(i * 2 * PI / nPoints) * sin(orientation * PI / 180), zMid + r * sin(i * 2 * PI / nPoints));
			glVertex3d(xIn + r * cos(i * 2 * PI / nPoints) * cos(orientation * PI / 180) - wt * 0.5 * sin(orientation * PI / 180), yIn + r * cos(i * 2 * PI / nPoints) * sin(orientation * PI / 180) + wt * 0.5 * cos(orientation * PI / 180), zMid + r * sin(i * 2 * PI / nPoints));
		}
		glEnd();

		glColor3ub(20, 20, 20);
		glBegin(GL_POLYGON);
		for (int i = 0; i < nPoints; i++) {
			glVertex3d(xIn + r * cos(i * 2 * PI / nPoints) * cos(orientation * PI / 180), yIn + r * cos(i * 2 * PI / nPoints) * sin(orientation * PI / 180), zMid + r * sin(i * 2 * PI / nPoints));
		}
		glEnd();

		glColor3ub(20, 20, 20);
		glBegin(GL_POLYGON);
		for (int i = 0; i < nPoints; i++) {
			glVertex3d(xIn + r * cos(i * 2 * PI / nPoints) * cos(orientation * PI / 180) - wt * 0.5 * sin(orientation * PI / 180), yIn + r * cos(i * 2 * PI / nPoints) * sin(orientation * PI / 180) + wt * 0.5 * cos(orientation * PI / 180), zMid + r * sin(i * 2 * PI / nPoints));
		}
		glEnd();

	}

	void DrawWheelR(const double& xIn, const double& yIn, const double& orientation) {
		glColor3ub(20, 20, 20);
		glBegin(GL_LINE_LOOP);

		double ws = 3; // Wheel diameter
		double wt = 1; // Wheel thickness
		ws = ws / 2;
		double zMid = 0.9;
		double r = 0.9;
		int nPoints = 200;
		for (int i = 0; i < nPoints; i++) {
			glVertex3d(xIn + r * cos(i * 2 * PI / nPoints) * cos(orientation * PI / 180), yIn + r * cos(i * 2 * PI / nPoints) * sin(orientation * PI / 180), zMid + r * sin(i * 2 * PI / nPoints));
			glVertex3d(xIn + r * cos(i * 2 * PI / nPoints) * cos(orientation * PI / 180) + wt * 0.5 * sin(orientation * PI / 180), yIn + r * cos(i * 2 * PI / nPoints) * sin(orientation * PI / 180) - wt * 0.5 * cos(orientation * PI / 180), zMid + r * sin(i * 2 * PI / nPoints));
		}
		glEnd();

		glColor3ub(20, 20, 20);
		glBegin(GL_POLYGON);
		for (int i = 0; i < nPoints; i++) {
			glVertex3d(xIn + r * cos(i * 2 * PI / nPoints) * cos(orientation * PI / 180), yIn + r * cos(i * 2 * PI / nPoints) * sin(orientation * PI / 180), zMid + r * sin(i * 2 * PI / nPoints));
		}
		glEnd();

		glColor3ub(20, 20, 20);
		glBegin(GL_POLYGON);
		for (int i = 0; i < nPoints; i++) {
			glVertex3d(xIn + r * cos(i * 2 * PI / nPoints) * cos(orientation * PI / 180) + wt * 0.5 * sin(orientation * PI / 180), yIn + r * cos(i * 2 * PI / nPoints) * sin(orientation * PI / 180) - wt * 0.5 * cos(orientation * PI / 180), zMid + r * sin(i * 2 * PI / nPoints));
		}
		glEnd();
	}

	void DrawLidar(const double& x, const double& y, const double &q) {
		glColor3ub(0, 0, 200);
		glBegin(GL_POLYGON);
		double rad = 0.7;
		double z = 1.7;
		int nPoints = 100;
		for (int i = 0; i < nPoints; i++) {
			glVertex3d(x + rad * cos(i * 2 * PI / nPoints), y + rad * sin(i * 2 * PI / nPoints), z);
			glVertex3d(x + rad * cos(i * 2 * PI / nPoints), y + rad * sin(i * 2 * PI / nPoints), 1);
		}
		glEnd();
	}

	void DrawTraj() {
		double Xnext, Ynext;

		glColor3ub(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex3d(0, 0, 0);
		glVertex3d(600, 0, 0);
		glVertex3d(600, 600, 0);
		glVertex3d(0, 600, 0);
		glEnd();
		
		for (int i = 0; i < traj.size(); i++) {
			if (i > index) {
				glColor3ub(255, 0, 0);
			}
			else {
				glColor3ub(150, 150, 150);
			}
				
			glBegin(GL_POLYGON);
			Xnext = traj[i].x * cellWidth + (cellWidth / 2.0);
			Ynext = traj[i].y * cellWidth + (cellWidth / 2.0);
			glVertex3d(Xnext - cellWidth * 0.5, Ynext - cellWidth * 0.5, 0);
			glVertex3d(Xnext - cellWidth * 0.5, Ynext + cellWidth * 0.5, 0);
			glVertex3d(Xnext + cellWidth * 0.5, Ynext + cellWidth * 0.5, 0);
			glVertex3d(Xnext + cellWidth * 0.5, Ynext - cellWidth * 0.5, 0);
			glEnd();
		}			
	}
};

int main(void) {
	#pragma region "3d Setup"
	int terminate = 0;
	CameraObject camera;
	OrbitingViewer orbit;

	camera.z = 0.0;
	camera.y = 0;
	camera.x = 0;
	orbit.dist = 40;
	#pragma endregion "3d Setup"

	// Perform all other Setup

	int bitmap[60][60];

	vector<Point> traj =
	{
		Point(0, 0),
		Point(0, 1),
		Point(0, 2),
		Point(0, 3),
		Point(0, 4),
		Point(0, 5),
		Point(0, 6),
		Point(0, 7),
		Point(0, 8),
		Point(0, 9),
		Point(1, 9),
		Point(2, 9),
		Point(3, 9),
		Point(3, 8),
		Point(3, 7),
		Point(4, 7),
		Point(5, 7),
		Point(6, 7),
		Point(7, 7),
		Point(7, 6),
		Point(7, 5),
		Point(7, 4),
		Point(6, 4),
		Point(5, 4)
	};

	//vector<Point> traj = { Point(0,0) };

	// Vector of all drawables
	//vector<Drawable*> drawables;

	Robot robot = Robot(traj[0].x, traj[0].y, traj);

	FsOpenWindow(16, 16, 800, 600, 1);

	// Main loop
	while (terminate == 0) {
		FsPollDevice();
		int wid, hei;
		FsGetWindowSize(wid, hei);

		int key = FsInkey();
		switch (key)
		{
		case FSKEY_ESC:
			terminate = 1;
			break;
		}

		if (0 != FsGetKeyState(FSKEY_LEFT))
		{
			orbit.h += YsPi / 180.0;
		}
		if (0 != FsGetKeyState(FSKEY_RIGHT))
		{
			orbit.h -= YsPi / 180.0;
		}
		if (0 != FsGetKeyState(FSKEY_UP))
		{
			orbit.p += YsPi / 180.0;
		}
		if (0 != FsGetKeyState(FSKEY_DOWN))
		{
			orbit.p -= YsPi / 180.0;
		}
		if (0 != FsGetKeyState(FSKEY_F))
		{
			orbit.dist = orbit.dist * 0.9;
		}
		if (0 != FsGetKeyState(FSKEY_B))
		{
			orbit.dist = orbit.dist * 1.1;
		}
		orbit.focusX = robot.X;
		orbit.focusY = robot.Y;
		orbit.h = 0.5;
		orbit.p = -1.0;
		orbit.SetUpCamera(camera);
		camera.b = -10;

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glViewport(-100, 0, wid, hei);

		// Set up 3D drawing
		camera.SetUpCameraProjection();
		camera.SetUpCameraTransformation();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1, 1);

		// Take in user Input

		// Generate obstacles/bitmap grid

		// Calculate traj

		// Move robot
		robot.Move();

		// Check if robot is at goal
		robot.CheckGoal();

		// Draw all objects

		robot.DrawTraj();
		robot.Draw();

		// Background Color

		//glColor3ub(60, 60, 60);
		//glBegin(GL_POLYGON);
		//glVertex2i(-100000, -100000);
		//glVertex2i(wid*100, -100000);
		//glVertex2i(wid*100, hei*100);
		//glVertex2i(-100000, hei*100);
		//glEnd();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, (float)wid - 1, (float)hei - 1, 0, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);
				

		//printf("%f %f %f\n", orbit.dist, orbit.h, orbit.p);

		// Determine if robot has reached 
		FsSwapBuffers();
		FsSleep(25);
	}

	return 0;
}