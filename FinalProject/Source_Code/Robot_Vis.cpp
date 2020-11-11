#include "Robot_Vis.h"
#include<iostream>
#define GL_SILENCE_DEPRECATION
#define _CRT_SECURE_NO_DEPRECATE
const int WallD = 10;

void walls::draw_Floor()
{

	glGenTextures(1, &this->texId);  // Reserve one texture identifier
	glBindTexture(GL_TEXTURE_2D, this->texId);  // Making the texture identifier current (or bring it to the deck)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D
	(GL_TEXTURE_2D,
		0,    // Level of detail
		GL_RGBA,
		this->png.wid,
		this->png.hei,
		0,    // Border width, but not supported and needs to be 0.
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		this->png.rgba);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor4d(1.0, 1.0, 1.0, 1);

	glEnable(GL_TEXTURE_2D);  // Begin using texture mapping


	glBindTexture(GL_TEXTURE_2D, this->texId);

	glBegin(GL_QUADS);

	glTexCoord2d(0.0, 0.0);
	glVertex3d(0,0, -0.7);

	glTexCoord2d(1.0, 0.0);
	glVertex3d(0,600, -0.7);

	glTexCoord2d(1.0, 1.0);
	glVertex3d(-600,600, -0.7);

	glTexCoord2d(0.0, 1.0);
	glVertex3d(-600,0,-0.7);

	glEnd();
	glDisable(GL_TEXTURE_2D);  // End using texture mapping

}

void walls::draw_Wall()
{

	glGenTextures(1, &this->texId);  // Reserve one texture identifier
	glBindTexture(GL_TEXTURE_2D, this->texId);  // Making the texture identifier current (or bring it to the deck)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D
	(GL_TEXTURE_2D,
		0,    // Level of detail
		GL_RGBA,
		this->png.wid,
		this->png.hei,
		0,    // Border width, but not supported and needs to be 0.
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		this->png.rgba);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor4d(1.0, 1.0, 1.0, alpha);

	glEnable(GL_TEXTURE_2D);  // Begin using texture mapping


	glBindTexture(GL_TEXTURE_2D, this->texId);

	glBegin(GL_QUADS);

	glTexCoord2d(0.0, 0.0);
	glVertex3i(-x1,y1, 0);

	glTexCoord2d(1.0, 0.0);
	glVertex3i(-x2,y2,0);

	glTexCoord2d(1.0, 1.0);
	glVertex3i(-x2, y2,40);

	glTexCoord2d(0.0, 1.0);
	glVertex3i(-x1,y1,40);

	glEnd();
	glDisable(GL_TEXTURE_2D);  // End using texture mapping
	
}

void DrawCube(double x0, double y0, double z0, double x1, double y1, double z1, int b)
{
	glBegin(GL_QUADS);

	glColor3ub(193 + b, 155 + b, 120 + b);
	glVertex3d(x0, y0, z0);
	glVertex3d(x1, y0, z0);
	glVertex3d(x1, y0, z1);
	glVertex3d(x0, y0, z1);

	glVertex3d(x0, y1, z0);
	glVertex3d(x1, y1, z0);
	glVertex3d(x1, y1, z1);
	glVertex3d(x0, y1, z1);

	glColor3ub(173 + b, 135 + b, 98 + b);
	glVertex3d(x0, y0, z0);
	glVertex3d(x1, y0, z0);
	glVertex3d(x1, y1, z0);
	glVertex3d(x0, y1, z0);

	glVertex3d(x0, y0, z1);
	glVertex3d(x1, y0, z1);
	glVertex3d(x1, y1, z1);
	glVertex3d(x0, y1, z1);

	glColor3ub(185 + b, 140 + b, 98);
	glVertex3d(x0, y0, z0);
	glVertex3d(x0, y0, z1);
	glVertex3d(x0, y1, z1);
	glVertex3d(x0, y1, z0);

	glVertex3d(x1, y0, z0);
	glVertex3d(x1, y0, z1);
	glVertex3d(x1, y1, z1);
	glVertex3d(x1, y1, z0);

	glEnd();

	
}

void Draw3DObstacles(int dx, int dz, const int bitmap[60][60])
{
	for (int z = 0; z < dz; ++z)
	{
		for (int x = 0; x < dx; ++x)
		{
			const double x0 = (double)x * WallD;
			const double y0 = 0.0;
			const double z0 = (double)z * WallD;

			const double x1 = x0 + WallD;
			const double y1 = y0 + WallD;
			const double z1 = z0 + WallD;

			auto b = bitmap[x][z];
			if (0 != b)
			{
				DrawCube(-x0, z0 , 0.0 , -x1, z1, b * WallD / 10, b);
			}
		}
	}
}

//void DrawCube(double x0, double y0, double z0, double x1, double y1, double z1)
//{
//	glBegin(GL_QUADS);
//
//	glColor3f(0, 0, 0);
//	glVertex3d(x0, y0, z0);
//	glVertex3d(x1, y0, z0);
//	glVertex3d(x1, y0, z1);
//	glVertex3d(x0, y0, z1);
//
//	glVertex3d(x0, y1, z0);
//	glVertex3d(x1, y1, z0);
//	glVertex3d(x1, y1, z1);
//	glVertex3d(x0, y1, z1);
//
//	glColor3f(0.2, 0.2, 0.2);
//	glVertex3d(x0, y0, z0);
//	glVertex3d(x1, y0, z0);
//	glVertex3d(x1, y1, z0);
//	glVertex3d(x0, y1, z0);
//
//	glVertex3d(x0, y0, z1);
//	glVertex3d(x1, y0, z1);
//	glVertex3d(x1, y1, z1);
//	glVertex3d(x0, y1, z1);
//
//	glColor3f(0.1, 0.1, 0.1);
//	glVertex3d(x0, y0, z0);
//	glVertex3d(x0, y0, z1);
//	glVertex3d(x0, y1, z1);
//	glVertex3d(x0, y1, z0);
//
//	glVertex3d(x1, y0, z0);
//	glVertex3d(x1, y0, z1);
//	glVertex3d(x1, y1, z1);
//	glVertex3d(x1, y1, z0);
//
//	glEnd();
//}
//
//void Draw3DObstacles(int dx, int dz, const int bitmap[60][60])
//{
//	for (int z = 0; z < dz; ++z)
//	{
//		for (int x = 0; x < dx; ++x)
//		{
//			auto b = bitmap[z][x];
//			const double x0 = (double)x * WallD;
//			const double y0 = (double)z * WallD;
//			const double z0 = 0.0;
//
//			const double x1 = x0 + WallD;
//			const double y1 = y0 + WallD;
//			const double z1 = b;
//
//
//			if (1 == b)
//			{
//				DrawCube(x0, y0, z0, x1, y1, z1);
//			}
//		}
//	}
//}

CameraObject::CameraObject() {
	CameraObject::Initialize();
}

void CameraObject::Initialize(void) {
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

void CameraObject::SetUpCameraProjection(void) {
	int wid, hei;
	double aspect;

	FsGetWindowSize(wid, hei);
	aspect = (double)wid / (double)hei;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov * 180.0 / YsPi, aspect, nearZ, farZ);
}

void CameraObject::SetUpCameraTransformation(void) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(-b * 180.0 / YsPi, 0.0, 0.0, 1.0);  // Rotation about Z axis
	glRotated(-p * 180.0 / YsPi, 1.0, 0.0, 0.0);  // Rotation about X axis
	glRotated(-h * 180.0 / YsPi, 0.0, 1.0, 0.0);  // Rotation about Y axis
	glTranslated(-x, -y, -z);
}

void CameraObject::GetForwardVector(double& vx, double& vy, double& vz) {
	vx = -cos(p) * sin(h);
	vy = sin(p);
	vz = -cos(p) * cos(h);
}

OrbitingViewer::OrbitingViewer() {
	OrbitingViewer::Initialize();
}

void OrbitingViewer::Initialize(void) {
	h = 0;
	p = 0;
	dist = 50.0;
	focusX = 0.0;
	focusY = 0.0;
	focusZ = 0.0;
}

void OrbitingViewer::SetUpCamera(CameraObject& camera) {
	camera.h = h;
	camera.p = p;
	camera.b = 0.0;

	double vx, vy, vz;
	camera.GetForwardVector(vx, vy, vz);
	camera.x = focusX - vx * dist;
	camera.y = focusY - vy * dist;
	camera.z = focusZ - vz * dist;
}

Robot::Robot() {
	x = -1;
	y = -1;

	wall[0].png.Decode("wall2.png");
	wall[1].png.Decode("wall1.png");
	wall[2].png.Decode("wall2.png");
	wall[3].png.Decode("wall3.png");
	wall[4].png.Decode("floor.png");	//floor
	wall[0].x1 = 600;
	wall[1].x1 = 600;
	wall[1].y1 = 600;
	wall[1].y2 = 600;
	wall[2].y2 = 600;
	wall[3].y2 = 600;
	wall[3].x1 = 600;
	wall[3].x2 = 600;
	
	camera = CameraObject();
	orbit = OrbitingViewer();

	camera.z = 0.0;
	camera.y = 0;
	camera.x = 0;
	orbit.dist = 500;

	X = x * cellWidth + (cellWidth / 2.0);
	Y = y * cellWidth + (cellWidth / 2.0);
	orientation = 0;
	index = 0;
	atGoal = false;

	traj = { std::make_pair(-1,-1) };
}

Robot::~Robot() {
	// Clear traj vector
	//delete(&traj);
}

void Robot::UpdateTraj(const std::vector<std::pair<int,int>> trajIn) {
	traj = trajIn;
}

void Robot::UpdateBitMap(const int bitmapIn[60][60]) {
	for (int i = 0; i < 60; ++i) {
		for (int j = 0; j < 60; ++j) {
			bitmap[i][j] = bitmapIn[i][j];
		}
	}
}

void Robot::Move(void) {
	// Update robot position according to current trajectory
	if (atGoal == true) {
		orientation += 3;
	}
	else if (index < traj.size()) {
		double Xnext, Ynext;
		Xnext = traj[index].first * cellWidth + (cellWidth / 2.0);
		Ynext = traj[index].second * cellWidth + (cellWidth / 2.0);

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

void Robot::CheckGoal(void) {
	// Check to see if robot is at the end of its trajectory
	if (index == traj.size() && x != 0 && y != 0) {
		atGoal = true;
	}
	else {
		atGoal = false;
	}
}

void Robot::Draw(void) {
	// Draw the robot and traj

	orbit.focusX = -X;
	orbit.focusY = Y;
	orbit.h = 0.5;
	orbit.p = -1.0;
	orbit.SetUpCamera(camera);
	camera.b = -10;


	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glViewport(-100, 0, 800, 600);

	camera.SetUpCameraProjection();
	camera.SetUpCameraTransformation();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 1);

	Move();

	CheckGoal();

	for (int i = 0; i < 4;i++) {
		if (i != 1 && i != 2) {
			wall[i].draw_Wall();
		}
	}
	wall[4].draw_Floor();

	Draw3DObstacles(60, 60, bitmap);

	DrawTraj();

#pragma region "Draw Body"
	glColor3ub(0, 100, 255);
	glBegin(GL_QUADS);

	double trX = X + w * 0.5 * cos(orientation * PI / 180) - h * 0.5 * sin(orientation * PI / 180);
	double tlX = X - w * 0.5 * cos(orientation * PI / 180) - h * 0.5 * sin(orientation * PI / 180);
	double brX = X + w * 0.5 * cos(orientation * PI / 180) + h * 0.5 * sin(orientation * PI / 180);
	double blX = X - w * 0.5 * cos(orientation * PI / 180) + h * 0.5 * sin(orientation * PI / 180);

	trX = -trX;
	tlX = -tlX;
	blX = -blX;
	brX = -brX;
	
	double trY = Y + w * 0.5 * sin(orientation * PI / 180) + h * 0.5 * cos(orientation * PI / 180);
	double tlY = Y - w * 0.5 * sin(orientation * PI / 180) + h * 0.5 * cos(orientation * PI / 180);
	double brY = Y + w * 0.5 * sin(orientation * PI / 180) - h * 0.5 * cos(orientation * PI / 180);
	double blY = Y - w * 0.5 * sin(orientation * PI / 180) - h * 0.5 * cos(orientation * PI / 180);

	glColor3ub(0, 100, 255);
	glVertex3d(trX, trY, z - t);
	glVertex3d(tlX, tlY, z - t);
	glVertex3d(blX, blY, z - t);
	glVertex3d(brX, brY, z - t);

	glColor3ub(0, 100, 255);
	glVertex3d(trX, trY, z);
	glVertex3d(tlX, tlY, z);
	glVertex3d(blX, blY, z);
	glVertex3d(brX, brY, z);

	glColor3ub(0, 70, 220);
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

	DrawWheelL(trX, trY, -orientation);
	DrawWheelL(tlX, tlY, -orientation);
	DrawWheelR(brX, brY, -orientation);
	DrawWheelR(blX, blY, -orientation);

	double fX = X - w * 0.3 * cos(orientation * PI / 180);
	double fY = Y - w * 0.3 * sin(orientation * PI / 180);

	DrawLidar(-fX, fY, orientation);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (float)800 - 1, (float)600 - 1, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	glViewport(0, 0, 800, 600);
	glColor3ub(255, 255, 255);
	glBegin(GL_POLYGON);
	glVertex2i(600, 0);
	glVertex2i(800, 0);
	glVertex2i(800, 600);
	glVertex2i(600, 600);
	glEnd();
}

void Robot::DrawWheelL(const double& xIn, const double& yIn, const double& orientation) {
	glColor3ub(40, 40, 40);
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

	glColor3ub(20, 176, 193);
	glBegin(GL_POLYGON);
	for (int i = 0; i < nPoints; i++) {
		glVertex3d(xIn + r * cos(i * 2 * PI / nPoints) * cos(orientation * PI / 180), yIn + r * cos(i * 2 * PI / nPoints) * sin(orientation * PI / 180), zMid + r * sin(i * 2 * PI / nPoints));
	}
	glEnd();

	glColor3ub(20, 176, 193);
	glBegin(GL_POLYGON);
	for (int i = 0; i < nPoints; i++) {
		glVertex3d(xIn + r * cos(i * 2 * PI / nPoints) * cos(orientation * PI / 180) - wt * 0.5 * sin(orientation * PI / 180), yIn + r * cos(i * 2 * PI / nPoints) * sin(orientation * PI / 180) + wt * 0.5 * cos(orientation * PI / 180), zMid + r * sin(i * 2 * PI / nPoints));
	}
	glEnd();

}

void Robot::DrawWheelR(const double& xIn, const double& yIn, const double& orientation) {
	glColor3ub(40, 40, 40);
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

	glColor3ub(20, 176, 193);
	glBegin(GL_POLYGON);
	for (int i = 0; i < nPoints; i++) {
		glVertex3d(xIn + r * cos(i * 2 * PI / nPoints) * cos(orientation * PI / 180), yIn + r * cos(i * 2 * PI / nPoints) * sin(orientation * PI / 180), zMid + r * sin(i * 2 * PI / nPoints));
	}
	glEnd();

	glColor3ub(20, 176, 193);
	glBegin(GL_POLYGON);
	for (int i = 0; i < nPoints; i++) {
		glVertex3d(xIn + r * cos(i * 2 * PI / nPoints) * cos(orientation * PI / 180) + wt * 0.5 * sin(orientation * PI / 180), yIn + r * cos(i * 2 * PI / nPoints) * sin(orientation * PI / 180) - wt * 0.5 * cos(orientation * PI / 180), zMid + r * sin(i * 2 * PI / nPoints));
	}
	glEnd();
}

void Robot::DrawLidar(const double& x, const double& y, const double& q) {
	glColor3ub(0, 0, 150);
	glBegin(GL_POLYGON);
	double rad = 1;
	double z = 2;
	int nPoints = 100;
	for (int i = 0; i < nPoints; i++) {
		glVertex3d(x + rad * cos(i * 2 * PI / nPoints), y + rad * sin(i * 2 * PI / nPoints), z);
		glVertex3d(x + rad * cos(i * 2 * PI / nPoints), y + rad * sin(i * 2 * PI / nPoints), 1);
	}
	glEnd();
}

void Robot::DrawTraj() {
	double Xnext, Ynext;

	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3d(0, 0, 0);
	glVertex3d(-600, 0, 0);
	glVertex3d(-600, 600, 0);
	glVertex3d(0, 600, 0);
	glEnd();

	for (int i = 0; i < traj.size(); i++) {
		if (i > index) {
			glColor3ub(250, 0, 0);
		}
		else {
			glColor3ub(100, 100, 100);
		}

		double thickness = 0.7;

		glBegin(GL_QUADS);
		Xnext = -(traj[i].first * cellWidth + (cellWidth / 2.0));
		Ynext = traj[i].second * cellWidth + (cellWidth / 2.0);
		glVertex3d(Xnext - cellWidth * 0.5, Ynext - cellWidth * 0.5, 0);
		glVertex3d(Xnext - cellWidth * 0.5, Ynext + cellWidth * 0.5, 0);
		glVertex3d(Xnext + cellWidth * 0.5, Ynext + cellWidth * 0.5, 0);
		glVertex3d(Xnext + cellWidth * 0.5, Ynext - cellWidth * 0.5, 0);

		glVertex3d(Xnext - cellWidth * 0.5, Ynext - cellWidth * 0.5, -thickness);
		glVertex3d(Xnext - cellWidth * 0.5, Ynext + cellWidth * 0.5, -thickness);
		glVertex3d(Xnext + cellWidth * 0.5, Ynext + cellWidth * 0.5, -thickness);
		glVertex3d(Xnext + cellWidth * 0.5, Ynext - cellWidth * 0.5, -thickness);

		glVertex3d(Xnext - cellWidth * 0.5, Ynext - cellWidth * 0.5, 0);
		glVertex3d(Xnext - cellWidth * 0.5, Ynext + cellWidth * 0.5, 0);
		glVertex3d(Xnext - cellWidth * 0.5, Ynext - cellWidth * 0.5, -thickness);
		glVertex3d(Xnext - cellWidth * 0.5, Ynext + cellWidth * 0.5, -thickness);

		glVertex3d(Xnext + cellWidth * 0.5, Ynext - cellWidth * 0.5, 0);
		glVertex3d(Xnext + cellWidth * 0.5, Ynext + cellWidth * 0.5, 0);
		glVertex3d(Xnext + cellWidth * 0.5, Ynext + cellWidth * 0.5, -thickness);
		glVertex3d(Xnext + cellWidth * 0.5, Ynext - cellWidth * 0.5, -thickness);

		glVertex3d(Xnext - cellWidth * 0.5, Ynext - cellWidth * 0.5, 0);
		glVertex3d(Xnext - cellWidth * 0.5, Ynext + cellWidth * 0.5, 0);
		glVertex3d(Xnext + cellWidth * 0.5, Ynext + cellWidth * 0.5, -thickness);
		glVertex3d(Xnext + cellWidth * 0.5, Ynext - cellWidth * 0.5, -thickness);

		glVertex3d(Xnext - cellWidth * 0.5, Ynext - cellWidth * 0.5, 0);
		glVertex3d(Xnext - cellWidth * 0.5, Ynext - cellWidth * 0.5, 0);
		glVertex3d(Xnext + cellWidth * 0.5, Ynext - cellWidth * 0.5, -thickness);
		glVertex3d(Xnext + cellWidth * 0.5, Ynext - cellWidth * 0.5, -thickness);

		glEnd();
	}
}

void Robot::DrawBitMap() {
	for (int i = 0; i < 60; i++) {
		for (int j = 0; j < 60; j++) {
			if (bitmap[i][j] == 0) {
				glColor3f(1, 1, 1);
				glBegin(GL_QUADS);
				glVertex3f(i * 10, j * 10, -0.1);
				glVertex3f(i * 10, j * 10 + 10, -0.1);
				glVertex3f(i * 10 + 10, j * 10 + 10, -0.1);
				glVertex3f(i * 10 + 10, j * 10, -0.1);
				glEnd();
			}
			else {
				glColor3f(0, 0, 0);
				glBegin(GL_QUADS);
				glVertex3f(i * 10, j * 10, -0.1);
				glVertex3f(i * 10, j * 10 + 10, -0.1);
				glVertex3f(i * 10 + 10, j * 10 + 10, -0.1);
				glVertex3f(i * 10 + 10, j * 10, -0.1);
				glEnd();
			}
		}
	}
}

void Robot::UpdateStart(int xIn, int yIn) {
	printf("%d\n",xIn);
	x = xIn;
	y = yIn;
	X = x * cellWidth + (cellWidth / 2.0);
	Y = y * cellWidth + (cellWidth / 2.0);
}

void Robot::UpdatePos(void) {
	x = traj[index].first;
	y = traj[index].second;
}