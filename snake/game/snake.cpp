#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <utility>
#include <termios.h>

#define X_MAP 15
#define Y_MAP 15

using namespace std;

unsigned int delay = 200000;


vector<vector<char>> map(Y_MAP, vector<char>(X_MAP, 0)); // 0:nothing / 1:snake / 2:apple / 3:border
vector<pair<char, char>> snake(X_MAP*Y_MAP);
int snakeHead = 0;
int snakeLength = 0;

char direction = 0;

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
	map[randomY][randomX] = 2;
}

void initMap()
{
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



int main()
{
	int x = 3;
	int y = 3;
	time_t currentTime = time(NULL);
	srand((unsigned)time(NULL));
	enableRawMode();
	initMap();
	snake[0] = {3, 3};
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
		read(STDIN_FILENO, &input, 1);
		switch (input)
		{
		case 'd':
			if (direction != 2)
				direction = 0;
			break;
		case 'z':
			if (direction != 3)
				direction = 1;
			break;
		case 'q':
			if (direction != 0)
				direction = 2;
			break;
		case 's':
			if (direction != 1)
				direction = 3;
			break;
		default:
			break;
		}	
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
			break;

		if (map[y][x] == 2)
		{
			isAppleEaten = true;
			placeApple();
			snakeLength++;
			delay -= 2000;
		}

		map[y][x] = 1;
		snakeHead ++;
		if (snakeHead == X_MAP*Y_MAP)
			snakeHead = 0;
		snake[snakeHead] = {x, y};

		drawMap();
		usleep(delay);
	}
	disableRawMode();

	return 0;
}