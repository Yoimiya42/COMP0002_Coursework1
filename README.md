# UCL COMP0002 C Coursework

All 7 required stages(including challenge stage 6 & 7) have been completed , 
and the program can compile and running smoothly in my Windows 11 system laptop.

NOTE: The total runtime is around 1 minute,depending on the arena's size and the complexity of the obstacles. Thank you for your patience and understanding.


## Complication and running the program:

To complie, you should enter the following command:
```
gcc -o robot main.c robot.c arena.c graphics.c
```
To run:
```
./robot.exe | java -jar drawapp-4.0.jar
```


## Essential Files:

-`arena.c`: 
It includes all functions related to drawing, covering both the items to be drawn and the specific algorithms for the drawing process.

Additionally, it initialised fundamental information for the arena, such as the random numbers of tiles, which tile is wall and which tile is obstacle or marker... 

-`robot.c`:
All functions related to the robot's behaviours are contained within this file.
Two very important functions go() and goHome() integrate and organize all the other functions, respectively enabling the complete processes of collecting markers and returning to home.

-`main.c`
It only contains main() function which serves as the entry point of the program.

-`0.png`, `1.png`, `2.png`,`3.png`
They are used to draw the robot in four different orientations:
Up(North), Right(East), Down(South), Left(West), respectively.


## Features:
-High Randomness. The following tiems are all randomly generated, with substantial variation in each run:
  - the number of tiles in rows and columns
  - irregular walls
  - the number and position of obstacles and markers (each consists of about 10) 
  - the start position and orientation of robot
  - the position of home

-Algorithms simliar to DFS and A*
-Gradient colours are uesd to represent the path traversed by the robot. 

