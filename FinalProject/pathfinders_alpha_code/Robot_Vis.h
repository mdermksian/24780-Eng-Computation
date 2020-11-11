/*
Robot Visualization 

Mike Turski - 11/20/19

*/

#ifndef __ROBOT_VIS__IS__INCLUDED__
#define __ROBOT_VIS__IS__INCLUDED__

#include <vector>
#include "fssimplewindow.h"

const double YsPi = 3.1415927;

class CameraObject
{
public:
	double x, y, z;
	double h, p, b;

	double fov, nearZ, farZ;

	CameraObject();

	void Initialize(void);

	void SetUpCameraProjection(void);

	void SetUpCameraTransformation(void);

	void GetForwardVector(double& vx, double& vy, double& vz);
};

class OrbitingViewer
{
public:
	double h, p;
	double dist;
	double focusX, focusY, focusZ;

	OrbitingViewer();

	void Initialize(void);

	void SetUpCamera(CameraObject& camera);
};

class Robot {
public:
	const double PI = 3.141592;
	const double cellWidth = 10;
	const double moveSpeed = 1.5;
	const double h = 4;
	const double t = 0.7;
	const double w = 6;
	const double z = 1.3;

	int index, x, y;
	double orientation, X, Y;

	bool atGoal;

	std::vector<std::pair<int, int>> traj;

	CameraObject camera;
	OrbitingViewer orbit;

	int bitmap[60][60];

	Robot();

	~Robot();

	void UpdateTraj(const std::vector<std::pair<int, int>> trajIn);

	void UpdateBitMap(const int bitmap[60][60]);

	void Move(void);

	void CheckGoal(void);

	void Draw(void);

	void DrawWheelL(const double& xIn, const double& yIn, const double& orientation);

	void DrawWheelR(const double& xIn, const double& yIn, const double& orientation);

	void DrawLidar(const double& x, const double& y, const double& q);

	void DrawTraj();

	void DrawBitMap();

	void UpdateStart(const int& xIn, const int& yIn);
};

#endif // !__GUI__IS__INCLUDED__