#include <unistd.h>
/*
notice the lack of iostream, this is because ncurses provides its own
output and input functions.
*/
#include "ncurses.h"
#include "random.hpp"

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
			attron(A_STANDOUT);
			if (i == 0 || i == height-2){
				attron(COLOR_PAIR(1));
				mvaddch(i,j,'#');
				attroff(COLOR_PAIR(1));
			}
			else if ((j == 0 || j == width-1) && i != height-1){
				attron(COLOR_PAIR(1));
				mvaddch(i,j,'#');
				attroff(COLOR_PAIR(1));
			}
			else if (i == snekY && j == snekX){
				attron(COLOR_PAIR(2));
				mvaddch(i,j,'O');
				attroff(COLOR_PAIR(2));
			}
			else if (i == fruitY && j == fruitX){
				attron(COLOR_PAIR(3));
				mvaddch(i,j,'F');
				attroff(COLOR_PAIR(3));
			}
			else if (i == height-1){
				mvprintw(i,0,"Score: %d",score);
			}
			else{
				bool tail{false};
				for (int k {0}; k < nTail; ++k){
					if (j == tailX[k] && i == tailY[k]){
						attron(COLOR_PAIR(2));
						mvaddch(i,j,'o');
						attroff(COLOR_PAIR(2));
						tail = true;
					}
				}
				if (!tail){
					attron(COLOR_PAIR(4));
					mvaddch(i,j,' ');
					attroff(COLOR_PAIR(4));
				}
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
	start_color();
	setup();

	init_pair(1,COLOR_WHITE, COLOR_WHITE);
	init_pair(2,COLOR_GREEN, COLOR_GREEN);
	init_pair(3,COLOR_RED, COLOR_RED);
	init_pair(4,COLOR_BLACK, COLOR_BLACK);

	while (!gameOver) {
		draw();
		input();
		logic();
		usleep(63000);
	}

	endwin();

	return 0;
}
