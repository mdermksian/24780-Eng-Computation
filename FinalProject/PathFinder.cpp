// ECFinalProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <list>
#include <utility>
#include <stack> 
#include <set>


using namespace std;

class Nodes
{
public:
    int x, y, parentx, parenty, obstacle;
    double local_dist, global_dist, sum_dist;

    int Obstacles(int grid[][60], int row, int col)
    {
        // Returns 1 if the node is not blocked
        if (grid[row][col] == 1)
            return (1);
        else
            return (0);
    }
    double CalculateDistance(int xi, int yi, int xe, int ye)
    {
        return sqrtf((xe - xi) * (xe - xi) + (ye - yi) * (ye - yi));
    }
    void FindTrajectory(int grid[60][60], int startx, int starty, int destx, int desty)
    {
    
        Nodes node[60][60];
        std::list<std::pair<int, int>> Neighbours;
        bool Explored[60][60];
        bool ReachedTarget = false;

        //******Setup of Initial Node values**************************//
        int i, j;
        for (int i = 0; i < 60; i++)///
        {
            for (int j = 0; j < 60; j++)
            {
                node[i][j].x = i;
                node[i][j].y = j;
                node[i][j].obstacle = Obstacles(grid, i, j);
                node[i][j].local_dist = INFINITY;
                node[i][j].global_dist = INFINITY;
                node[i][j].sum_dist = INFINITY;
                node[i][j].parentx = -1;
                node[i][j].parenty = -1;
                Explored[i][j] = false;

            }
        }

        //*****Set starting point of Path Finder***//
        i = startx;
        j = starty;
        node[i][j].local_dist = 0;
        node[i][j].global_dist = 0;
        node[i][j].sum_dist = 0;
        node[i][j].parentx = i;
        node[i][j].parenty = j;

        Neighbours.push_back(make_pair(i, j));

        printf("startx = %d, starty = %d, destx = %d, desty = %d", startx, starty, destx, desty);
        printf("\nnode[%d][%d](x ,y) = %d,%d", startx, starty, node[i][j].x, node[i][j].y);
      

        while (!Neighbours.empty() && ReachedTarget == false)
        {
           
            //Explored.push_back(node[i][j]);//closed
            
            auto pair = *Neighbours.begin();

            i = pair.first;
            j = pair.second;

            Neighbours.erase(Neighbours.begin());//open

            Explored[i][j] = true;

            double newlocal_dist, newglobal_dist, newsum_dist;

            printf("\nnode[i][j](x ,y) = %d,%d", node[i][j].x, node[i][j].y);

            ///Neighbour Left
            if (node[i - 1][j].x == destx && node[i - 1][j].y == desty)
            {
                node[i - 1][j].parentx = i;
                node[i - 1][j].parenty = j;
                GetTrajectory(node, destx, desty);
                ReachedTarget = true;
                return;
            }
            else if (i > 0 && node[i - 1][j].obstacle == 1 && Explored[i - 1][j] == false)
            {
                newlocal_dist = node[i][j].local_dist + 1.0;
                newglobal_dist = CalculateDistance(node[i - 1][j].x, node[i - 1][j].y, destx, desty);
                newsum_dist = newlocal_dist + newglobal_dist;

                if (node[i - 1][j].sum_dist == INFINITY || node[i - 1][j].sum_dist > newsum_dist)
                {
                    printf("Added nodenode[i - 1][j]");
                    Neighbours.push_back(make_pair(i - 1, j));
                    node[i - 1][j].local_dist = newlocal_dist;
                    node[i - 1][j].global_dist = newglobal_dist;
                    node[i - 1][j].sum_dist = newsum_dist;
                    node[i - 1][j].parentx = i;
                    node[i - 1][j].parenty = j;
                }
            }

            ///Neighbour Right
            if (node[i + 1][j].x == destx && node[i + 1][j].y == desty)
            {
                node[i + 1][j].parentx = i;
                node[i + 1][j].parenty = j;
                GetTrajectory(node, destx, desty);
                ReachedTarget = true;
                return;
            }
            else if (i < 60 && node[i + 1][j].obstacle == 1 && Explored[i + 1][j] == false)
            {
                newlocal_dist = node[i][j].local_dist + 1.0;
                newglobal_dist = CalculateDistance(node[i + 1][j].x, node[i + 1][j].y, destx, desty);
                newsum_dist = newlocal_dist + newglobal_dist;

                if (node[i + 1][j].sum_dist == INFINITY || node[i + 1][j].sum_dist > newsum_dist)
                {
                    printf("Added nodenode[i + 1][j]");
                    Neighbours.push_back(make_pair(i + 1, j));
                    node[i + 1][j].local_dist = newlocal_dist;
                    node[i + 1][j].global_dist = newglobal_dist;
                    node[i + 1][j].sum_dist = newsum_dist;
                    node[i + 1][j].parentx = i;
                    node[i + 1][j].parenty = j;
                }
            }
            ///Neighbour Up
            if (node[i][j + 1].x == destx && node[i][j + 1].y == desty)
            {
                node[i][j + 1].parentx = i;
                node[i][j + 1].parenty = j;
                GetTrajectory(node, destx, desty);
                ReachedTarget = true;
                return;
            }
            else if (j < 60 && node[i][j + 1].obstacle == 1 && Explored[i][j + 1] == false)
            {
                newlocal_dist = node[i][j].local_dist + 1.0;
                newglobal_dist = CalculateDistance(node[i][j + 1].x, node[i][j + 1].y, destx, desty);
                newsum_dist = newlocal_dist + newglobal_dist;

                if (node[i][j + 1].sum_dist == INFINITY || node[i][j + 1].sum_dist > newsum_dist)
                {
                    printf("Added nodenode[i][j + 1]");
                    Neighbours.push_back(make_pair(i, j + 1));
                    node[i][j + 1].local_dist = newlocal_dist;
                    node[i][j + 1].global_dist = newglobal_dist;
                    node[i][j + 1].sum_dist = newsum_dist;
                    node[i][j + 1].parentx = i;
                    node[i][j + 1].parenty = j;
                }
            }


            if (node[i][j - 1].x == destx && node[i][j - 1].y == desty)
            {
                node[i][j - 1].parentx = i;
                node[i][j - 1].parenty = j;
                GetTrajectory(node, destx, desty);
                ReachedTarget = true;
                return;

            }
            else if (j > 0 && node[i][j - 1].obstacle == 1 && Explored[i][j - 1] == false)
            {
                newlocal_dist = node[i][j].local_dist + 1.0;
                newglobal_dist = CalculateDistance(node[i][j - 1].x, node[i][j - 1].y, destx, desty);
                newsum_dist = newlocal_dist + newglobal_dist;

                if (node[i][j - 1].sum_dist == INFINITY || node[i][j - 1].sum_dist > newsum_dist)
                {
                    printf("Added nodenode[i][j - 1]");
                    Neighbours.push_back(make_pair(i, j - 1));
                    node[i][j - 1].local_dist = newlocal_dist;
                    node[i][j - 1].global_dist = newglobal_dist;
                    node[i][j - 1].sum_dist = newsum_dist;
                    node[i][j - 1].parentx = i;
                    node[i][j - 1].parenty = j;
                }
            }

        }
        if (ReachedTarget == false)
        {
            printf("Failed to find the Destination Cell\n");
        }
    }

   list<pair<int,int>> GetTrajectory(Nodes node[60][60], int destx, int desty)
    {
        int xcoord = destx;
        int ycoord = desty;
        std::list<std::pair<int, int>> Path;

        while (!(node[xcoord][ycoord].parentx == xcoord && node[xcoord][ycoord].parenty == ycoord))
        {
            Path.push_front(make_pair(xcoord, ycoord));
            int temp_row = node[xcoord][ycoord].parentx;
            int temp_col = node[xcoord][ycoord].parenty;
            xcoord = temp_row;
            ycoord = temp_col;
        }

        Path.push_front(make_pair(xcoord, ycoord));

        printf("\nThe Path is ");
        for (auto p : Path)
        {
            cout << "->" <<"("<< p.first<<","<< p.second<<")" ;
            //printf("-> (%d,%d) ", p.first, p.second);
        }
        return Path;
    }
};



int main()
{
    /*int grid[10][10] =
    {
        { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
        { 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
        { 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
        { 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
        { 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
        { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
        { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
        { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };*/

    // start is (8, 0);

    // Destination is (0, 0);

    //Nodes pf;

    //pf.FindTrajectory(grid, 8, 0, 0, 0);

    return(0);
}