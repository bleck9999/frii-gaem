#include <unistd.h>
/*
notice the lack of iostream, this is because ncurses provides its own
output and input functions.
*/
#include "ncurses.h"
#include "random.hpp"

/* So the story behind snek.cpp
*This was supposed to just be a simple program where I followed a tutorial and made a snek
*BUT OOOOOOOOHHHHH NO
*mr tutorial guy used windows specific code, mainly conio.h
*they half assed linux support with some lines going "if you're on linux do this instead"
*but i really wanted my snek
*so i spent 2/3 hours porting the snek from conio.h to ncurses.h
*with 0 prior knowledge of ncurses
*But I succeeded!
*and this is snek.cpp, with a copy on pastebin linked in the comments of the tutorial.
*all because i wanted a fucking snek on linux.
*/

using Random = effolkronium::random_static;

bool gameOver;
int width, height;
enum directions {
	STOP=0,
	LEFT,
	RIGHT,
	UP,
	DOWN
};
directions dir;
int tailX[200], tailY[200];
int nTail;
int snekX, snekY, fruitX, fruitY, score;

void setup(){
	gameOver = false;
	score = 0;
	snekX = width / 2;
	snekY = height / 2;
	dir = STOP;
	fruitX = Random::get(1, width-2);
	fruitY = Random::get(2, height-3);
}

void draw(){
	move(0, 0);
	for (int i {0}; i < height; ++i){
		for (int j {0}; j < width; ++j){
			if (i == 0 || i == height-2)
				mvaddch(i,j,'#');
			else if ((j == 0 || j == width-1) && i != height-1)
				mvaddch(i,j,'#');
			else if (i == snekY && j == snekX)
				mvaddch(i,j,'O');
			else if (i == fruitY && j == fruitX)
				mvaddch(i,j,'F');
			else if (i == height-1){
				mvprintw(i,0,"Score: %d",score);
			}
			else{
				bool tail{false};
				for (int k {0}; k < nTail; ++k){
					if (j == tailX[k] && i == tailY[k]){
						mvaddch(i,j,'o');
						tail = true;
					}
				}
				if (!tail)
					mvaddch(i,j,' ');
			}
		}
	}
	refresh();
}

void input(){
	switch (getch()){
	case 'a':
		dir = LEFT;
		break;
	case 'd':
		dir = RIGHT;
		break;
	case 'w':
		dir = UP;
		break;
	case 's':
		dir = DOWN;
		break;
	case 'x':
		gameOver = true;
		break;
	}
}

void logic(){
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = snekX;
    tailY[0] = snekY;
    for (int i {1}; i < nTail; ++i)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir)
    {
    case LEFT:
        --snekX;
        break;
    case RIGHT:
        ++snekX;
        break;
    case UP:
        --snekY;
        break;
    case DOWN:
        ++snekY;
        break;
    default:
        break;
    }
    if (snekX >= width-1) snekX = 1; else if (snekX < 1) snekX = width-2;
    if (snekY >= height-2) snekY = 1; else if (snekY < 1) snekY = height-3;

    for (int i = 0; i < nTail; i++){
       if (tailX[i] == snekX && tailY[i] == snekY)
    	   gameOver = true;
    }

    if (snekX == fruitX && snekY == fruitY){
    	score += 1;
    	fruitX = Random::get(1,width-2);
    	fruitY = Random::get(2,height-3);
    	++nTail;
    }

}

int main(){
	initscr(); //initialises ncurses
	cbreak();
	noecho();
	timeout(0);
	getmaxyx(stdscr, height, width);
	setup();

	while (!gameOver) {
		draw();
		input();
		logic();
		usleep(63000);
	}

	endwin();

	return 0;
}
