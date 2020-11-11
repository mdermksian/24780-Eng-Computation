#ifndef OBSTACLE_H_IS_INCLUDED
#define OBSTACLE_H_IS_INCLUDED

class Obstacles {
protected:
	float x, y;
	float w, h;
	float r = 0, g = 0, b = 0;
	bool state = false;
	int depth;
	int WallD = 10;
public:
	void Set_xy(int a, int b);
	void SetHeightWidth(float a, float b);
	void Predefined();
	void Draw();
	void intialize(int mx, int my);
	void generate(int mx, int my);
	void setObstacle(int bitmap[60][60],bool erase);
	void mapToBitMap(int bitmap[60][60],bool erase);
};
#endif