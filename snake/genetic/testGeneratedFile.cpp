#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <utility>
#include <termios.h>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <string>

#define X_MAP 15
#define Y_MAP 15

using namespace std;
#include "../../include/AI.h"

unsigned int delay = 100000;

vector<vector<char>> map(Y_MAP, vector<char>(X_MAP, 0)); // 0:nothing / 1:snake / 2:apple / 3:border
vector<pair<char, char>> snake(X_MAP*Y_MAP);

int appleX, appleY;
int x, y;

struct termios originalTerminalSettings;

void disableRawMode()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTerminalSettings);
}

void enableRawMode()
{
	tcgetattr(STDIN_FILENO, &originalTerminalSettings);

	struct termios newTerminalSettings = originalTerminalSettings;
	newTerminalSettings.c_lflag &= ~(ECHO | ICANON);
	newTerminalSettings.c_cc[VMIN] = 0;
	newTerminalSettings.c_cc[VTIME] = 0;

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &newTerminalSettings);
}

void placeApple()
{
	int randomX = 0;
	int randomY = 0;
	while (map[randomY][randomX] != 0)
	{
		randomX = rand() % X_MAP;
		randomY = rand() % Y_MAP;
	}
	appleX = randomX;
	appleY = randomY;
	map[randomY][randomX] = 2;
}

void initMap()
{
	for (int i = 0; i < Y_MAP; i++)
		for (int j = 0; j < X_MAP; j++)
			map[i][j] = 0;

	for (int i = 0; i < Y_MAP; i++)
	{
		map[i][0] = 3;
		map[i][X_MAP-1] = 3;
	}

	for (int i = 0; i < X_MAP; i++)
	{
		map[0][i] = 3;
		map[Y_MAP-1][i] = 3;
	}
	map[3][3] = 1;
	placeApple();
}

void drawMap()
{
	system("clear");
	for (int i = 0; i < Y_MAP; i++)
	{
		for (int j = 0; j < X_MAP; j++)
		{
			switch (map[i][j])
			{
			case 0:
				cout << "  ";
				break;
			case 1:
				cout << "[]";
				break;
			case 2:
				cout << "$$";
				break;
			case 3:
				cout << "##";
				break;
			default:
				break;
			}
		}
		cout << '\n';
	}
}

void prepareAI(AI& ai)
{
	if (x < appleX)
		ai.neurons[0][0] = 1.0;
	else
		ai.neurons[0][0] = 0.0;
	if (y > appleY)
		ai.neurons[0][1] = 1.0;
	else
		ai.neurons[0][1] = 0.0;
	if (x > appleX)
		ai.neurons[0][2] = 1.0;
	else
		ai.neurons[0][2] = 0.0;
	if (y < appleY)
		ai.neurons[0][3] = 1.0;
	else
		ai.neurons[0][3] = 0.0;


	if (map[y][x+1] == 1 || map[y][x+1] == 3)
		ai.neurons[0][4] = 1.0;
	else
		ai.neurons[0][4] = 0.0;
	if (map[y-1][x] == 1 || map[y-1][x] == 3)
		ai.neurons[0][5] = 1.0;
	else
		ai.neurons[0][5] = 0.0;
	if (map[y][x-1] == 1 || map[y][x-1] == 3)
		ai.neurons[0][6] = 1.0;
	else
		ai.neurons[0][6] = 0.0;
	if (map[y+1][x] == 1 || map[y+1][x] == 3)
		ai.neurons[0][7] = 1.0;
	else
		ai.neurons[0][7] = 0.0;
}

void readInputFromAI(AI& ai, char* input)
{
	ai.calculateOutput();
	int index = 0;
	double best = ai.neurons[3][0];
	for (int i = 1; i < 4; i++)
	{
		if (ai.neurons[3][i] > best)
		{
			best = ai.neurons[3][i];
			index = i;
		}
	}
	*input = index;
}



int main()
{
	string filename;
	cout << "Enter filename : ";
	cin >> filename;
	
	time_t currentTime = time(NULL);
	srand((unsigned)time(NULL));
	enableRawMode();


	AI snakeAI(filename.c_str());

	x = 3;
	y = 3;
	initMap();
	snake[0] = {3, 3};
	int snakeHead = 0;
	int snakeLength = 0;
	char direction = 0;
	char input = 0;
	bool isAppleEaten = false;
	while (true)
	{
		if (!isAppleEaten)
		{
			int tail = snakeHead - snakeLength;
			if (tail < 0)
				tail += X_MAP*Y_MAP;
			map[snake[tail].second][snake[tail].first] = 0;
		}
		isAppleEaten = false;
		prepareAI(snakeAI);
		readInputFromAI(snakeAI, &input);
		direction = input;

		switch (direction)
		{
		case 0:
			x++;
			break;
		case 1:
			y--;
			break;
		case 2:
			x--;
			break;
		case 3:
			y++;
			break;
		default:
			break;
		}


		if (map[y][x] == 3 || map[y][x] == 1)
		{
			break;
		}

		if (map[y][x] == 2)
		{
			isAppleEaten = true;
			placeApple();
			snakeLength++;
		}

		map[y][x] = 1;
		drawMap();
		snakeHead ++;
		if (snakeHead == X_MAP*Y_MAP)
			snakeHead = 0;
		snake[snakeHead] = {x, y};

		usleep(delay);
	}

	disableRawMode();

	return 0;
}
