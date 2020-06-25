#include <unistd.h>
#include <string>
#include <vector>
#include <map>

#include "random.hpp"
#include "ncurses.h"

#define COLOR_DARKRED 8
#define COLOR_GREY    9

using std::string;
using Random = effolkronium::random_static;

bool gameOver;
int width, height, cursorX, cursorY;
string word;
unsigned int numMines;

class MSGrid {
private:
	std::vector<std::vector<char>> m_grid;
	int m_height;
	int m_width;
	std::map<char, char> cIndicators {
		{'t','1'},
		{'y','2'},
		{'u','3'},
		{'i','4'},
		{'o','5'},
		{'p','6'},
		{'[','7'},
		{']','8'}
	};
	std::map<char, char> flaggedChars {
		{'g','t'},
		{'h','y'},
		{'j','u'},
		{'k','i'},
		{'l','o'},
		{';','p'},
		{'\'','['},
		{'#',']'},
		{'f','?'},
		{'F','*'}
	};
	std::map<char, char> unflaggedChars {
			{'t','g'},
			{'y','h'},
			{'u','j'},
			{'i','k'},
			{'o','l'},
			{'p',';'},
			{'[','\''},
			{']','#'},
			{'?','f'},
			{'*','F'}
		};
public:
	void init(int i_height, int i_width){
		m_height = i_height; m_width = i_width;
		m_grid.reserve(i_height);
		for (int i {0}; i < i_height; ++i)
			m_grid.push_back(std::vector<char>(i_width,'?'));
	}
	void addMine(int Y, int X){
		m_grid[Y][X] = '*';
		//uncover(Y,X);//DEBUG
	}
	void toggleFlag(int Y, int X){
		std::map<char,char>::iterator it = flaggedChars.find(m_grid[Y][X]); // @suppress("Invalid arguments")
		std::map<char,char>::iterator ti = unflaggedChars.find(m_grid[Y][X]); // @suppress("Invalid arguments")
		if (it != flaggedChars.end())
			m_grid[Y][X] = it->second; // @suppress("Field cannot be resolved")
		else if (ti != unflaggedChars.end())
			m_grid[Y][X] = ti->second; // @suppress("Field cannot be resolved")
	}
	void addIndicator(int Y, int X, int amount){
		switch(amount){
		case 1:
			m_grid[Y][X] = 't';
			break;
		case 2:
			m_grid[Y][X] = 'y';
			break;
		case 3:
			m_grid[Y][X] = 'u';
			break;
		case 4:
			m_grid[Y][X] = 'i';
			break;
		case 5:
			m_grid[Y][X] = 'o';
			break;
		case 6:
			m_grid[Y][X] = 'p';
			break;
		case 7:
			m_grid[Y][X] = '[';
			break;
		case 8:
			m_grid[Y][X] = ']';
			break;
		default:
			break; //do nothing if 0 (input is expected to be 0-8 only)
		}
		//uncover(Y,X); //DEBUG

	}
	void uncover(int Y, int X){
		std::map<char,char>::iterator it = cIndicators.find(m_grid[Y][X]); // @suppress("Invalid arguments")
		if (m_grid[Y][X] == '*')
			gameOver = true;//DEBUG: changed to false
		else if (it != cIndicators.end())
			m_grid[Y][X] = it->second; // @suppress("Field cannot be resolved")
		else if (m_grid[Y][X] == '?'){
			m_grid[Y][X] = ' ';
			if (Y != height-1)
				uncover(Y+1,X);
			if (Y != 0)
				uncover(Y-1,X);
			if (X != width-1)
				uncover(Y,X+1);
			if (X != 0)
				uncover(Y,X-1);
			return;
		}
	}
	char getValue(int Y, int X){
		return m_grid[Y][X];
		//uncover(Y,X); //DEBUG
	}
};

//#include <iostream> //DEBUG
MSGrid mapp;
int nearMines(const int i, const int j);

void setup(){
	//height = 24;//DEBUG
	//width = 90;//DEBUG
	gameOver = false;
	numMines = (width * height) / 10;
	cursorX = width / 2;
	cursorY = height / 2;
	int tempX,tempY;

	//---- Generate map ----
	//create mines
	unsigned int n {0};
	while (n < numMines){
		tempX = Random::get(1,width-2);
		tempY = Random::get(1,height-2);
		if (mapp.getValue(tempY,tempX) != '*'){
			mapp.addMine(tempY, tempX);
			++n;
		}
		//std::cout << numMines << i << ' ' << mineX[i] << mineY[i] << "  ";//DEBUG
	}
	//create indicators
	for (int i {0}; i < height; ++i){
			for (int j {0}; j < width; ++j){
				mapp.addIndicator(i,j,nearMines(i,j));
			}
	}

	//std::cout << "Line 45";//DEBUG
}

int nearMines(const int i, const int j){
	int out {0};
//	for (unsigned int it {0}; it < numMines; ++it){
	if (mapp.getValue(i,j) == '*')
		return 0;
	if (i != 0)
		if (mapp.getValue(i-1,j) == '*')   {++out;}
	if (i != 0 && j != width-1)
		if (mapp.getValue(i-1,j+1) == '*') {++out;}
	if (i != 0 && j != 0)
		if (mapp.getValue(i-1,j-1) == '*') {++out;}
	if (i != height-1)
		if (mapp.getValue(i+1,j) == '*')   {++out;}
	if (j != 0)
		if (mapp.getValue(i,j-1) == '*')   {++out;}
	if (i != height-1 && j != 0)
		if (mapp.getValue(i+1,j-1) == '*') {++out;}
	if (j != width-1)
		if (mapp.getValue(i,j+1) == '*')   {++out;}
	if (i != height-1 && j != width-1)
		if (mapp.getValue(i+1,j+1) == '*') {++out;} //this section could be optimised but notlikeblob says no
	return out;
}

void draw(){
	//std::cout << "Line 49";//DEBUG
	//height = 24;//DEBUG
	//width = 90;//DEBUG
	move(0,0);
	for (int i {0}; i < height; ++i){
		for (int j {0}; j < width; ++j){
			if (i == cursorY && j == cursorX){
				attron(COLOR_PAIR(11));
				mvaddch(i,j,'v');
				attroff(COLOR_PAIR(11));
			}
			else {
				switch(mapp.getValue(i,j)){
				case '*': //mine
					/*attron(COLOR_PAIR(11)); //DEBUG
					mvaddch(i,j,'*'); //DEBUG
					attroff(COLOR_PAIR(11)); //DEBUG
					break; //DEBUG */
				case 't': //covered indicators (1-8)
				case 'y':
				case 'u':
				case 'i':
				case 'o':
				case 'p':
				case '[':
				case ']':
				case '?': //covered blank
					attron(COLOR_PAIR(2));
					mvaddch(i,j,'?');
					attroff(COLOR_PAIR(2));
					break;
				case 'g': //flagged indicators (1-8)
				case 'h':
				case 'j':
				case 'k':
				case ';':
				case '\'':
				case '#':
				case 'f': //flagged blank
				case 'F': //flagged mine
					attron(COLOR_PAIR(5));
					mvaddch(i,j,'F');
					attroff(COLOR_PAIR(5));
					break;
				case '1':
					attron(COLOR_PAIR(3));
					mvaddch(i,j,'1');
					attroff(COLOR_PAIR(3));
					break;
				case '2':
					attron(COLOR_PAIR(4));
					mvaddch(i,j,'2');
					attroff(COLOR_PAIR(4));
					break;
				case '3':
					attron(COLOR_PAIR(5));
					mvaddch(i,j,'3');
					attroff(COLOR_PAIR(5));
					break;
				case '4':
					attron(COLOR_PAIR(6));
					mvaddch(i,j,'4');
					attroff(COLOR_PAIR(6));
					break;
				case '5':
					attron(COLOR_PAIR(8));
					mvaddch(i,j,'5');
					attroff(COLOR_PAIR(8));
					break;
				case '6':
					attron(COLOR_PAIR(9));
					mvaddch(i,j,'6');
					attroff(COLOR_PAIR(9));
					break;
				case '7':
					attron(COLOR_PAIR(7));
					mvaddch(i,j,'7');
					attroff(COLOR_PAIR(7));
					break;
				case '8':
					attron(COLOR_PAIR(10));
					mvaddch(i,j,'8');
					attroff(COLOR_PAIR(10));
					break;
				case ' ':
					attron(COLOR_PAIR(1));
					mvaddch(i,j,' ');
					attroff(COLOR_PAIR(1));
				}/*
				attron(COLOR_PAIR(11));
				char m = mapp.getValue(i,j);
				mvaddch(i,j,m);
				attroff(COLOR_PAIR(11)); */
			}
		}
	}
}

void input(){
	switch(getch()){
	case 'w':
	case KEY_UP:
	case 'i':
		if (cursorY != 0)
			--cursorY;
		break;
	case 's':
	case KEY_DOWN:
	case 'k':
		if (cursorY != height-1)
			++cursorY;
		break;
	case 'd':
	case KEY_RIGHT:
	case 'l':
		if (cursorX != width-1)
			++cursorX;
		break;
	case 'j':
	case KEY_LEFT:
	case 'a':
		if (cursorX != 0)
			--cursorX;
		break;
	case 'f':
		mapp.toggleFlag(cursorY, cursorX);
		break;
	case ' ':
	case KEY_ENTER:
		mapp.uncover(cursorY, cursorX);
		break;
	case 'q':
		gameOver = true;
	}
}

int main(){
	initscr(); //initialises ncurses
	cbreak();
	noecho();
	keypad(stdscr, true);
	timeout(0);
	getmaxyx(stdscr, height, width);
	mapp.init(height,width);
	start_color();
	setup();
	init_color(COLOR_DARKRED,400,0,0);
	init_color(COLOR_GREY,400,400,400);

	init_pair(1,COLOR_WHITE, COLOR_WHITE);  //Uncovered empty
	init_pair(2,COLOR_BLACK, COLOR_BLACK);  //Covered
	init_pair(3,COLOR_BLUE, COLOR_WHITE);   //1
	init_pair(4,COLOR_GREEN, COLOR_WHITE);  //2
	init_pair(5,COLOR_RED, COLOR_WHITE);    //3, flag(F)
	init_pair(6,COLOR_MAGENTA, COLOR_WHITE);//4
	init_pair(7,COLOR_BLACK, COLOR_WHITE);  //7
	init_pair(8,COLOR_DARKRED, COLOR_WHITE);//5
	init_pair(9,COLOR_CYAN, COLOR_WHITE);   //6
	init_pair(10,COLOR_GREY,COLOR_WHITE);   //8
	init_pair(11,COLOR_BLACK,COLOR_WHITE);  //Mine(*), cursor(v)


	while (!gameOver){
		draw();
		input();
	//	logic();
	//	usleep(2500);
	}
	getch();
	endwin();

	/*
	setup();//DEBUG
	//std::cout << "Line 115";//DEBUG
	draw();//DEBUG
	getch(); //DEBUG
*/

	return 0;
}
