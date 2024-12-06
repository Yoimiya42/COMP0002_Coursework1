#ifndef ROBOT_H
#define ROBOT_H


typedef struct robot{
    int x;
    int y;
    int dir;
    int carried_markers;
} Robot;

extern const int direction;

extern int task_finished;

void drawRobot(Robot *robot);
void randomInitRobot(Robot *robot);
void update(Robot *robot);

int canMoveForward(Robot *robot);
void forward(Robot *robot);
void left(Robot *robot);
void right(Robot *robot);
int markerCount(Robot *robot);
int atMarker(Robot *robot);
void pickMarker(Robot *robot);
void dropMarker(Robot *robot);
int isAtHome(Robot *robot);

int manhattanDistance(int x, int y, int home_x, int home_y);
void go(Robot *robot);

void goHome(Robot *robot);

void end(Robot *robot);



#endif // ROBOT_H