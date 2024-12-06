#include "graphics.h"
#include "robot.h"
#include "arena.h"

#include <stdio.h>
#include <stdlib.h>

#include <time.h>




int main()
{
    setWindowSize(WIDTH,HEIGHT);

    // randomly generate the size of the arena and the number of tiles
    randomArena();
   
    //intialise the tiles and home position
    // The number and position of walls, obstacles, and markers are all generated randomly
    drawBackground();

    Robot myRobot;
    Robot* robot_ptr = &myRobot;

    // randomly generate the robot's initial position
    sleep(1000);
    randomInitRobot(robot_ptr);
    sleep(2500);

    // start the robot to explore the arena
    // Level 1 : collect all markers
    go(robot_ptr);
    sleep(2500);
    
    // Level 2 : return all markers to home
    goHome(robot_ptr);
 
    end(robot_ptr);

    return 0;
}



