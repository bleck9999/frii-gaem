#include <unistd.h>

#include "ncurses.h"

bool gameOver;
int p1Score, p2Score;
int width,height;
enum directions {
	STOP=0,
	DOWN,
	UP
};
directions dir1, dir2;
int x1, x2, noty1, y2; //fun story: y1 is used by stdlib and for some dumb reason things break if you dare to use y1 
int ballX, ballY, ballVX, ballVY;

int isOnPlayer(int i, int j){
	if (j == x1+1 && (i == noty1-4 || i == noty1-3 || i == noty1-2 || i == noty1-1 || i == noty1-0))
		return 1;
	else if (j == x2-1 && (i == y2-4 || i == y2-3 || i == y2-2 || i == y2-1 || i == y2-0))
		return 2;
	return 0;
}

void setup(){
	gameOver = false;
	dir1 = STOP;
	dir2 = STOP;
	x1 = 2;
	x2 = width - 3;
	noty1 = (height / 2) + 1;
	y2 = (height / 2) + 1;
	ballVX = 0;
	ballVY = 0;
	ballX = width / 2;
	ballY = (height / 2) - 1;
}

void draw(){
	move(0, 0);
	//bool didExec {};
	for (int i {0}; i < height; ++i){
		for (int j {0}; j < width; ++j){
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
			//if (i == noty1 && j == x1){
			//	attron(COLOR_PAIR(3));
			//	mvaddch(i,j,'*');
			//	attroff(COLOR_PAIR(3));
			//}
			else if (i == noty1 && j == x1){
				attron(COLOR_PAIR(1));
				mvaddch(i-4,j,'1');
				mvaddch(i-3,j,'1');
				mvaddch(i-2,j,'1');
				mvaddch(i-1,j,'1');
				mvaddch(i-0,j,'1');
				attroff(COLOR_PAIR(1));
			}
			else if (i == y2 && j == x2){
				attron(COLOR_PAIR(1));
				mvaddch(i-4,j,'2');
				mvaddch(i-3,j,'2');
				mvaddch(i-2,j,'2');
				mvaddch(i-1,j,'2');
				mvaddch(i-0,j,'2');
				attroff(COLOR_PAIR(1));
			}
			else if (i == ballY && j == ballX){
				attron(COLOR_PAIR(1));
				mvaddch(i,j,'B');
				attroff(COLOR_PAIR(1));
			}
			else if (i == height-1){
				mvprintw(i,0,"Player 1: %d points",p1Score);
				mvprintw(i,width-18,"Player 2: %d points",p2Score);
			}
			else {
				attron(COLOR_PAIR(2));
				mvaddch(i,j,' ');
				attroff(COLOR_PAIR(2));
			}
		}
	}

	refresh();
}

void input(){
	switch(getch()){
	case 'w':
		dir1 = UP;
		break;
	case 's':
		dir1 = DOWN;
		break;
	//case 'd':
	//	++x1;
	//	break;
	//case 'a':
	//	--x1;
	//	break;
	case KEY_UP:
		dir2 = UP;
		break;
	case KEY_DOWN:
		dir2 = DOWN;
		break;
	case 'x':
		gameOver = true;
		break;
	default:
		dir1 = STOP; dir2 = STOP;
		break;
	}
}

void logic(){
	if (dir1 == UP){
		if (noty1 <= 5)
			noty1 = 5;
		else
			--noty1; //decreasing y moves it up
		dir1 = STOP;
	}
	else if (dir1 == DOWN){
		if (noty1 >= height-3)
			noty1 = height-3;
		else
			++noty1;
		dir1 = STOP;
	}
	if (dir2 == UP){
		if (y2 <= 5)
			y2 = 5;
		else
			--y2; //decreasing y moves it up
		dir2 = STOP;
	}
	else if (dir2 == DOWN){
		if (y2 >= height-3)
			y2 = height-3;
		else
			++y2;
		dir2 = STOP;
	}

	//that was the easy bit, now its time for b a l l

	if (ballVX == 0 && ballVY == 0){
		if (p1Score > p2Score)
			ballVX = -1;
		else
			ballVX = 1;
		ballVY = 1;
	}
	if ((ballX == 3 && ballY == height-3) ||
			(ballX == width - 4 && ballY == height-3) ||
			(ballX == 2 && ballY == 1) ||
			(ballX == width - 4 && ballY == 1)
			){
		ballVX *= -1;
		ballVY *= -1;
	}
	else if (isOnPlayer(ballY,ballX))
		ballVX *= -1;
	else if (ballY >= height-3 || ballY <= 1)
		ballVY *= -1;
	else if (ballX <= x1){
		++p2Score;
		ballX = width / 2;
		ballY = (height / 2) - 1;
		ballVX = 0;
		ballVY = 0;
	}
	else if (ballX >= x2){
		++p1Score;
		ballX = width / 2;
		ballY = (height / 2) - 1;
		ballVX = 0;
		ballVY = 0;
	}
	ballX += ballVX;
	ballY += ballVY;
	if (p1Score >= 9 || p2Score >= 9)
		gameOver = true;
}

int main(){
	initscr(); //initialises ncurses
	cbreak();
	noecho();
	keypad(stdscr, true);
	timeout(0);
	getmaxyx(stdscr, height, width);
	start_color();
	setup();

	init_pair(1,COLOR_WHITE, COLOR_WHITE);
	init_pair(2,COLOR_BLACK, COLOR_BLACK);
	//init_pair(3,COLOR_RED, COLOR_RED);

	while (!gameOver){
		draw();
		input();
		logic();
		usleep(63000);
	}

	endwin();

	return 0;
}
