#ifndef ARENA_H
#define ARENA_H

extern const int WIDTH;
extern const int HEIGHT;
extern const int TILE_SIZE;

extern int ARENA_WIDTH;
extern int ARENA_HEIGHT;

extern int TILE_ROWS;  
extern int TILE_COLS; 

extern int total_markers;

typedef struct tile{
   int row;
   int col;
   // five status of a tile
   int is_wall;
   int is_obstacle;
   int is_marker;
   int is_empty;
   int is_visited;

   int is_visited_to_home;
   int is_home;
} Tile;

extern Tile tiles[18][30];

int randomNum(int min, int max);
void randomArena();

void drawObstacles(int row, int col);
void drawMarker(int row, int col);

void drawWall(int row, int col);
void buildExtraWalls(int row, int col, int prob, int direction);
void buildWall(int row, int col);
int enclosedByWalls(int row, int col);

void initialiseTiles();
void initHome();
void show_total_markers();

void drawBackground();

void drawPath(int row, int col);

void redrawMarker(int x,int y,int markers);

void showRemainingMarkers();



#endif // ARENA_H
