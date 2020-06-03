#include <iostream>
#include <string>
#include <cstdlib>
#include "random.hpp"


using Random = effolkronium::random_static;
using std::string ;

//global variables because fun
const char p1Mark {'O'};
const char p2Mark {'X'};

void printGrid(const char (&array)[9]){
	std::cout << "\nCurrent grid: \n";
	std::cout << ' ' << array[0] << '|' << array[1] << '|' << array[2] << ' '  << '\n';
	std::cout << "-------\n";
	std::cout << ' ' << array[3] << '|' << array[4] << '|' << array[5] << ' '  << '\n';
	std::cout << "-------\n";
	std::cout << ' ' << array[6] << '|' << array[7] << '|' << array[8] << ' '  << '\n';
}

void getChoice(char (&array)[9], const char mark){ //getchoice and userchoice have been condensed
	bool valid {false}; //mainly for my sanity
	int choice {}; //also getchoice2 is completely redundant as all that changes is one literal
	do {
		std::cout << "\nEnter your choice (1-9): ";
		std::cin >> choice;
		if (std::cin.fail()){ //additional error handling for non-int input
			std::cout << "\nInvalid number entered!";
			std::cin.ignore(32768,'\n');
		}
		else if (choice < 1 || choice > 9)
			std::cout << "\nNumber must be between 1 and 9.";
		else if (array[choice-1] != ' ')
			std::cout << "\nThat square is taken.";
		else
			valid = true;
	} while (!valid);
	array[choice-1] = mark;
}

void cpuChoice(char (&array)[9],const char mark){
	int choice {};
	do {
		choice = Random::get(0,8);
	} while (array[choice] != ' ');
	array[choice] = mark;
}

void displayWinner(const char mark){
	std::cout << "\nThe winner is: ";
	if (mark == p1Mark)
		std::cout << "Player 1!";
	else
		std::cout << "Player 2!";
/*
	I have decided against having a press enter to continue
	this is a console app, run it from the console you peasant
*/
}

void checkWinner(const char (&array)[9]){
	if (array[0] == array[1] && array[1] == array[2] && array[0] != ' ')
		displayWinner(array[0]);
	else if (array[3] == array[4] && array[4] == array[5] && array[3] != ' ')
		displayWinner(array[3]);
	else if (array[6] == array[7] && array[7] == array[8] && array[6] != ' ')
		displayWinner(array[6]);
	else if (array[0] == array[3] && array[3] == array[6] && array[0] != ' ')
		displayWinner(array[6]);
	else if (array[7] == array[1] && array[1] == array[4] && array[1] != ' ')
		displayWinner(array[1]);
	else if (array[8] == array[5] && array[5] == array[2] && array[2] != ' ')
		displayWinner(array[2]);
	else if (array[0] == array[4] && array[4] == array[8] && array[0] != ' ')
		displayWinner(array[0]);
	else if (array[4] == array[6] && array[6] == array[2] && array[6] != ' ')
		displayWinner(array[6]);
}


int main()
{
	char grid[9] {' ',' ',' ',' ',' ',' ',' ',' ',' '};

	unsigned int pc {}; //playercount
	do {
		std::cout << "\nHow many players (1/2)? ";
		std::cin >> pc;
		if (std::cin.fail()){
			std::cout << "\nThat is not a valid number!";
			std::cin.ignore(32768,'\n');
		}
	} while (!pc || pc > 2);

	while (true){ //exiting is handled by displayWinner(), called by checkWinner()
		printGrid(grid);
		getChoice(grid, p1Mark);
		checkWinner(grid);
		if (pc == 2)
			getChoice(grid, p2Mark);
		else
			cpuChoice(grid, p2Mark);
		checkWinner(grid);
	}

	return 1;
}
