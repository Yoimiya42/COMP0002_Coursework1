#include "graphics.h"
#include "robot.h"

#include <stdio.h>
#include <stdlib.h>

#include <time.h>

const int WIDTH = 1500;  
const int HEIGHT = 800;   
const int TILE_SIZE = 50;

int r = 255, g = 0, b = 0;
const int step = 5;  

int ARENA_WIDTH ;  // 
int ARENA_HEIGHT;  // 500-1000


int TILE_ROWS; //ARENA_HEIGHT / TILE_SIZE;  // 10-20
int TILE_COLS; //ARENA_WIDTH / TILE_SIZE;  // 15-30

int total_markers = 0;

typedef struct tile{
   int row;
   int col;

   int is_wall;
   int is_obstacle;
   int is_marker;
   int is_empty;
   int is_visited;
   
   int is_visited_to_home;
   int is_home;
} Tile;

Tile tiles[18][30] = {0};


int randomNum(int min, int max)
{
   return rand() % (max-min+1) + min;
}

// randomly generate the size of the arena and tiles in the rows and columns 
 void randomArena()
{
    srand(time(0));
    ARENA_WIDTH = randomNum(15, (WIDTH-300) / TILE_SIZE) * TILE_SIZE;    // 750-1200
    ARENA_HEIGHT = randomNum(10, HEIGHT / TILE_SIZE) * TILE_SIZE;  // 500-800
    TILE_ROWS = ARENA_HEIGHT / TILE_SIZE;  //10-16
    TILE_COLS = ARENA_WIDTH / TILE_SIZE;  //15-24
}

void drawObstacles(int row, int col)
{
     displayImage("obstacle.png", col * TILE_SIZE, row * TILE_SIZE);     
}

void drawMarker(int row, int col)
{
     setColour(gray);
     fillRect(col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE,TILE_SIZE);
}


//Stage 7 : Build walls with irregular shapes
void drawWall(int row, int col)
{
    tiles[row][col].is_wall = 1;
    setColour(red);
    fillRect(col * TILE_SIZE,row * TILE_SIZE, TILE_SIZE, TILE_SIZE);
    displayImage("wall.png", col * TILE_SIZE, row * TILE_SIZE);
}

#define PROB 25    // Initial probability
void buildExtraWalls(int row, int col, int prob, int direction)
{
   if (prob<=0) return;
   
   drawWall(row, col);

   int new_r=row, new_c=col;
   switch(direction)
   {
    case 0:  new_r = row + 1; break; // generate wall below
    case 1:  new_r = row - 1; break; // generate wall above
    case 2:  new_c = col + 1; break; // generate wall to the right
    case 3:  new_c = col - 1; break; // generate wall to the left
   }
   if(rand() % 100 < prob)
   {
      buildExtraWalls(new_r, new_c, prob-5, direction);
   }
}

void buildWall(int row, int col)
{
   if(row == 0)                               // at the top
   {
      buildExtraWalls(row, col, PROB, 0);
   }
   else if(row == TILE_ROWS - 1)               //at the bottom
   {             
      buildExtraWalls(row, col, PROB, 1);
   }
   else if(col == 0)                          //at the left
   {                        
      buildExtraWalls(row, col, PROB, 2);
   }
   else if(col == TILE_COLS - 1)              //at the right
   {             
      buildExtraWalls(row, col, PROB, 3);
   }
} 

// check if the tile is enclosed by walls
int enclosedByWalls(int row, int col)
{
    int counter = 0;
    if (tiles[row][col-1].is_wall) counter++;   //left
    if (tiles[row][col+1].is_wall) counter++;   //right
    if (tiles[row-1][col].is_wall) counter++;   //up
    if (tiles[row+1][col].is_wall) counter++;   //down
     
    return counter >= 3;
}

// initialise the status of each tile
void initialiseTiles()                     
{
    srand(time(0));
    for(int r=0; r<TILE_ROWS; r++)
    { 
      for(int c=0; c<TILE_COLS; c++)
      {
        tiles[r][c].row = r;
        tiles[r][c].col = c;
        if(r==0 || r==TILE_ROWS-1 || c==0 || c==TILE_COLS-1)  
           buildWall(r,c);                        // Build walls with irregular shapes
      }
    }
   
    for(int r=0; r<TILE_ROWS; r++)
    {
      for(int c=0; c<TILE_COLS; c++)
      {
        if(!tiles[r][c].is_wall)
        {
            int random=rand() % 100; 
            if (random<3){                                          // obstacle
                tiles[r][c].is_obstacle = 1;
                drawObstacles(r,c);
            }else if (random<8 && !enclosedByWalls(r,c)){           // marker
                tiles[r][c].is_marker = 1;
                drawMarker(r,c);
                total_markers++;
            }else{                                                  // empty
                tiles[r][c].is_empty = 1;  
            }
        }  
      }
    }

}

void initHome()
{
    srand(time(0));
    while (1)
    {
        int r=rand() % TILE_ROWS;
        int c=rand() % TILE_COLS;
        if(tiles[r][c].is_empty && ! enclosedByWalls(r,c))
        {
            tiles[r][c].is_home = 1;
            tiles[r][c].is_empty = 0;
            displayImage("home.png", c*TILE_SIZE, r*TILE_SIZE);
            break;
        }
    }
}  

void show_total_markers()
{
    char total[20];
    snprintf(total, sizeof(total), "Total Markers: %d", total_markers);
    setColour(black);
    drawString(total, ARENA_WIDTH+20, 50);

    char remaining[30]="Remaining Markers: ";
    drawString(remaining, ARENA_WIDTH+20, 100);
    
}

void drawBackground()
{
    background();
    setColour(white);  
    fillRect(0, 0,  WIDTH, HEIGHT); 

    initialiseTiles();
    initHome();

    setColour(black);
    for(int r=0; r<TILE_COLS; r++)             // draw vertical lines
    {
        drawLine(0 + r * TILE_SIZE, 0,  0 + r * TILE_SIZE, ARENA_HEIGHT);
    }
    for(int c=0; c<TILE_ROWS; c++)             // draw horizontal lines
    {
        drawLine(0, 0 + c * TILE_SIZE,  ARENA_WIDTH, 0 + c * TILE_SIZE);
    }

    show_total_markers();                     // show the total number of markers beside the arena
}

// draw the path taken by the robot using gradient colours           
void drawPath(int row, int col)
{  
    if(!tiles[row][col].is_home)             
    {
        if (r == 255 && g < 255 && b == 0)      g += step;       // red to yellow
        else if (g == 255 && r > 0 && b == 0)   r -= step;       // yellow to green
        else if (g == 255 && b < 255 && r == 0) b += step;       // green to cyan
        else if (b == 255 && g > 0 && r == 0)   g -= step;       // cyan to blue
        else if (b == 255 && r < 255 && g == 0) r += step;       // blue to purple
        else if (r == 255 && b > 0 && g == 0)   b -= step;       // purple to red
                    
        background();
        setRGBColour(r, g, b);
        fillRect(col * TILE_SIZE + 1, row * TILE_SIZE + 1, TILE_SIZE - 2, TILE_SIZE - 2);
    }
}

// draw the markers returned to the home position
void redrawMarker(int x,int y,int markers)
{
    background();
    tiles[x][y].is_marker+=markers;
    drawMarker(x,y);
    char str[5];
    snprintf(str,sizeof(str),"%d",tiles[x][y].is_marker);
    setColour(white);
    drawString(str,y*50+TILE_SIZE/2,x*50+TILE_SIZE/2);
}


// update the number of remaining markers in real time
void showRemainingMarkers()
{

    int current_markers = --total_markers;
    char counter[30];
    snprintf(counter, sizeof(counter), "Remaining Markers: %d", current_markers);

    background();
    setColour(white);
    fillRect(ARENA_WIDTH+20, 80, 270, 50);

    setColour(black);
    drawString(counter, ARENA_WIDTH + 20, 100);

}