#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>    //ID: 209161603
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>


#define UP 72  // Up Arrow 
#define DOWN 80 // Down Arrow 
#define LEFT 75  // Left Arrow 
#define RIGHT 77  // Right Arrow 
#define ESC 27  // Escape Key
#define ROWS 25
#define COLS 75
#define SNAKECELL '@'
#define FOOD '$'
#define BOARD '#'
#define SPACE ' '
#define INITSIZE 10
#define MAX_SIZE 20

typedef struct Point {
	int x;
	int y;
}XYPOINT;

void start();
void printMenu();
void printBoard();
void implementaion();
void gotoxy(int y, int x);
int getKey();
bool searchPoint(XYPOINT* food, XYPOINT snake[], int size);
void placingFood(XYPOINT* food, XYPOINT snake[], int size, int* counter);
bool isKeyValid(char c);
void initSnake(XYPOINT snake[], int size, int x, int y);
void snakeMove(XYPOINT snake[], int size, int x, int y);
void eatFood(XYPOINT snake[], int* size, XYPOINT food, int* counter);
bool isLose(XYPOINT snake[], int size, int x, int y);


void main() {
	srand(time(NULL));
	start();
}

//apply the levels (the first one in this case)
void start() {
	int chosenLevel;
	chosenLevel = 4;

	while (chosenLevel < 0 || chosenLevel > 3) {
		printMenu();
		scanf("%d", &chosenLevel);
		switch (chosenLevel)
		{
		case 0:
			break;
		case 1:
			system("cls");
			printBoard();
			implementaion();
			chosenLevel = 4;
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			break;
		}
	}
}
//printing menu
void printMenu() {

	printf("*******************************************************\n");
	printf("* Pick Your Level, to exit press 0\n");
	printf("* 0 - Exit\n");
	printf("* 1 - Basic Level\n");
	printf("* 2 - Intermidiate Level\n");
	printf("* 3 - Advanced Level\n");
	printf("*******************************************************\n");
}
//printing game board
void printBoard() {
	int x, y;
	for (y = 0; y < ROWS; y++) {
		for (x = 0; x < COLS; x++) {
			if (y == 0 || y == ROWS-1 || x == 0 || x == COLS-1) {
				gotoxy(x, y);
				printf("%c", BOARD);
			}
		}
	}
}
//this function applys the game
void implementaion()
{
	XYPOINT snake[MAX_SIZE];
	XYPOINT food;
	int currentSize = INITSIZE;
	char c, lastMove;
	lastMove = 0;
	c = 0;
	int x, y;  // x for coulmn and y for line 
	x = 37; y = 12;
	int foodCounter;
	foodCounter = 0;
	initSnake(snake, currentSize, x, y);
	placingFood(&food, snake, currentSize, &foodCounter);
	// start from the middle of the screen
	gotoxy(x, y);
	c = 0;
		while (c != ESC)
		{

			// only if a key was hit we read what key code it was
			if (_kbhit()) // if any key was hit
				c = getKey();  // change direction
			if (isKeyValid(c)) {
				// sleeps for half a second before continuing to the next command
				Sleep(200); 
				//// make a move in current direction if no change
				switch (c)
				{
				case RIGHT:
					x++;
					break;
				case DOWN:
					y++;
					break;
				case LEFT:
					x--;
					break;
				case UP:
					y--;
					break;
				}
				if (isLose(snake, currentSize, x, y)) {
					system("cls");
					printf("GAME OVER\n");
					c = ESC;
				}
				else {
					snakeMove(snake, currentSize, x, y);
					gotoxy(0, 0);
					lastMove = c;
					if (foodCounter == 0){// thats means the snake ate the food and another one should be printed
						placingFood(&food, snake, currentSize, &foodCounter);
					}
					eatFood(snake, &currentSize, food, &foodCounter);
					if (currentSize == MAX_SIZE) { //if the snake grew to the maximum size, the user wins 
						system("cls");
						printf("YOU WIN!\n");
						c = ESC;
					}
				}
			}
			else {
				c = lastMove;
			}
		}
}
//this function used to check if the key is valid 
bool isKeyValid(char c) {

		if (c == UP || c == DOWN || c == LEFT || c == RIGHT || c == ESC) {
			return true;
		}
		else
			return false;
	}
//all the points are equal to x,y 
void initSnake(XYPOINT snake[], int size, int x, int y) {
	int i;
	for (i = 0; i < size; i++) {
		snake[i].x = x;
		snake[i].y = y;
	}
}

void snakeMove(XYPOINT snake[], int size, int new_x, int new_y) {
	int i;
	//every cell in the array that is no longer necessary- replaced by space
	gotoxy(snake[size - 1].x, snake[size - 1].y);
	printf("%c", SPACE);
	for (i = size-1 ; i > 0; i--) {
		snake[i] = snake[i-1];
	}
	snake[0].x = new_x;
	snake[0].y = new_y;
	//printing the snake (the array)
	for (i = 0; i < size; i++)
	{
		gotoxy(snake[i].x, snake[i].y);
		printf("%c", SNAKECELL);
	}
}
// This function moves the cursor to the line y and column x.
// (0,0) is the upper left corner of the screen.
void gotoxy(int x, int y) {
	printf("\x1b[%d;%df", y + 1, x + 1);
}

// This function gets keyCodes for special keys such as arrows , escape and others
// It returns the Key code such as 72 for arrow up, 27 for Escape... 
int getKey()
{
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}

//this function randomly placing food in the game borders
void placingFood(XYPOINT* food, XYPOINT snake[], int size, int* counter) {
		int i;

		food->y = 1 + rand() % (ROWS - 2);
		food->x = 1 + rand() % (COLS - 2);
		
		//making sure the $ won't placed on the snake
		while (searchPoint(food, snake, size)){
			food->y = 1 + rand() % (ROWS - 2);
			food->x = 1 + rand() % (COLS - 2);
		}
		gotoxy(food->x, food->y);
		printf("%c", FOOD);
		(*counter)++;
}
//looking for certain point in the array 
bool searchPoint(XYPOINT* requestedPoint, XYPOINT snake[], int size) {
	int i;
	for (i = 0; i < size; i++) {
		if (snake[i].x == requestedPoint->x && snake[i].y == requestedPoint->y) {
			return true;
		}
	}
	return false;
}
//checks if the snake ate the $, if it did- the value of counter would decreased so another one will be printed 
void eatFood(XYPOINT snake[], int* size, XYPOINT food, int* counter) {
	if (snake[0].x == food.x && snake[0].y == food.y) {
		(*counter)--;
		(*size) += 1;
	}
}
//checks for lose
bool isLose(XYPOINT snake[], int size, int new_x, int new_y) {
	XYPOINT temPoint;
	temPoint.x = new_x;
	temPoint.y = new_y;
	//checks if the snake collides with itself
	if (searchPoint(&temPoint, snake, size)) {
		return true;
	}
	//checks if the snake coliides with the walls
	else if(new_x == 0 ||new_y == 0 || new_x == COLS-1 || new_y == ROWS-1){
		return true;
	}
	return false;
}

