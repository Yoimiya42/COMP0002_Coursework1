#include "graphics.h"
#include "arena.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

typedef struct robot{
    int x;
    int y;
    int dir;
    int carried_markers;
    int home_posx;
    int home_posy;
} Robot;

const int direction[4][2] = {
    {-1,0},  //up -north
    {0,1},   //right -east
    {1,0},   //down -south
    {0,-1}   //left -west
};

int task_finished=0;


void drawRobot(Robot *robot)
{
    foreground();
    clear();
    // draw robot body with 4 distinct images for 4 directions
    char filename[10];
    snprintf(filename, sizeof(filename), "%d.png", (robot->dir) % 4);         
    displayImage(filename, robot->y * TILE_SIZE, robot->x * TILE_SIZE);
    // show the nunber of markers carried
    char show_markers[3];
    snprintf(show_markers, sizeof(show_markers), "%d", robot->carried_markers);
    setColour(white);
    drawString(show_markers, robot->y * TILE_SIZE + 25, robot->x * TILE_SIZE + 25);
}

void randomInitRobot(Robot *robot) 
{
    srand(time(0));
    while (1)
    {
        int r = rand() % TILE_ROWS;
        int c = rand() % TILE_COLS;
        if(tiles[r][c].is_empty && !enclosedByWalls(r,c))    
        {
            robot->x = r;
            robot->y = c;
            robot->dir = rand()%4; 
            robot->carried_markers = 0;
            drawRobot(robot);
            break; 
        }
    }
}

int sleep_time = 25;   // file scope variable
void update(Robot *robot)
{
    drawRobot(robot);
    sleep(sleep_time);
}


int canMoveForward(Robot *robot)
{
    int test_x = robot->x + direction[(robot->dir)%4][0];
    int test_y = robot->y + direction[(robot->dir)%4][1];

    return  ! tiles[test_x][test_y].is_wall && ! tiles[test_x][test_y].is_obstacle && 
            ! tiles[test_x][test_y].is_visited ;
}

void forward(Robot *robot)
{      
        drawPath(robot->x, robot->y);
        robot->x += direction[robot->dir][0];
        robot->y += direction[robot->dir][1];

        update(robot);
}

void left(Robot *robot)
{
    robot->dir = (robot->dir + 3) % 4;
    update(robot);
    sleep(10);
}

void right(Robot *robot)
{
    robot->dir = (robot->dir + 1) % 4;
    update(robot);
    sleep(10);
}

int markerCount(Robot *robot)
{
    return robot->carried_markers;
}

int atMarker(Robot *robot)
{
   return tiles[robot->x][robot->y].is_marker;
}

void pickMarker(Robot *robot)
{
    if (atMarker(robot))
    {
       robot->carried_markers += tiles[robot->x][robot->y].is_marker;
       tiles[robot->x][robot->y].is_marker = 0;
       showRemainingMarkers(); 
    }
}

void dropMarker(Robot *robot)
{   
    tiles[robot->x][robot->y].is_marker += markerCount(robot);
    redrawMarker(robot->x, robot->y, tiles[robot->x][robot->y].is_marker); 
    robot->carried_markers = 0; 
}

int isAtHome(Robot *robot)
{
    return tiles[robot->x][robot->y].is_home;
}


int manhattanDistance(int x, int y, int home_x, int home_y)
{
    return  abs(x - home_x) + abs(y - home_y);
}

// Depth First Search: move the robot along a path until it reaches a dead end
//                     backtrack to previous position and explore other directions
void go(Robot *robot)
{  
    if (task_finished)  return;

    tiles[robot->x][robot->y].is_visited = 1;

    if (atMarker(robot))
    {
        pickMarker(robot);
    }

    if (isAtHome(robot))
    { 
       robot->home_posx = robot->x;
       robot->home_posy = robot->y; 
    }

    if (total_markers == 0 && task_finished == 0)
    {   
        update(robot);
        task_finished = 1;
        sleep_time = 100;
        background();
        setColour(black);
        drawString("All markers collected!", ARENA_WIDTH+20, 150);
        drawString("Ready to go home...", ARENA_WIDTH+20, 175);
        sleep(2500);
        return;
    }

    if (task_finished) return;
    for (int i = 0; i < 4; i ++)
    {
        if (canMoveForward(robot))
        {
            forward(robot);
            go(robot);           

            if(task_finished) return;
            // backtracking
            right(robot);
            right(robot);
            forward(robot);    // return to previous position

            right(robot);
            right(robot);      // turn back to original direction
        }
        right(robot);
    }
}

// Calculate the Manhattan distance to 'home' for the four neighboring cells
// Always choose the unvisited cell with shortest diatance to move next
void goHome(Robot *robot)
{
    while (!isAtHome(robot))
    {   
        tiles[robot->x][robot->y].is_visited_to_home = 1;
        int min_distance = INT_MAX;
        int temp;
        int best_dir;
        for(int i = 0; i < 4; i ++)
        {   
            if (tiles[robot->x+direction[i][0]][robot->y+direction[i][1]].is_home)
            {
                best_dir = i;
                break;
            }
            else if ( !tiles[robot->x+direction[i][0]][robot->y+direction[i][1]].is_empty
                   || tiles[robot->x+direction[i][0]][robot->y+direction[i][1]].is_visited_to_home)
                temp= INT_MAX;
            else
                temp = manhattanDistance(robot->x+direction[i][0], robot->y+direction[i][1], robot->home_posx, robot->home_posy);
            if (temp<min_distance && tiles[robot->x+direction[i][0]][robot->y+direction[i][1]].is_empty)
            {
                min_distance = temp;
                best_dir = i;
            } 
        }  
        while (robot->dir != best_dir)
        {
            right(robot);
        }
        forward(robot);
    }
}


void end(Robot *robot)
{  
    if (isAtHome(robot))    
    {
        background();
        setColour(black);
        drawString("Task Completed!", ARENA_WIDTH+20, 250);
        displayImage("victory.png",ARENA_WIDTH+20, 300);

        int i = 0;
        while(i++ < 20)   // alternately display markers, home image and robot image
        {
        dropMarker(robot);
        sleep(1000);

        update(robot);
        sleep(1000);

        displayImage("home.png", robot->home_posy * TILE_SIZE, robot->home_posx * TILE_SIZE);
        sleep(1000);
        }
    }
}

