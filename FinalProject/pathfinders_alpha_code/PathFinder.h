#ifndef __PATHFINDER_H_IS_INCLUDED__
#define __PATHFINDER_H_IS_INCLUDED__

#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <vector>
#include <utility>
#include <stack> 
#include <set>

class Nodes {
public:
	int x, y, parentx, parenty, obstacle;
	double local_dist, global_dist, sum_dist;

	int Obstacles(int grid[][60], int row, int col);

	double CalculateDistance(int xi, int yi, int xe, int ye);

	std::vector<std::pair<int, int>> FindTrajectory(int grid[60][60], int startx, int starty, int destx, int desty);

	std::vector<std::pair<int, int>> GetTrajectory(Nodes node[60][60], int destx, int desty);
};

#endif