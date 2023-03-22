#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>

using namespace std;
#include "../../include/AI.h"

int main()
{
	time_t currentTime = time(NULL);
	srand((unsigned)time(NULL));
	AI model = AI("../handmade/xor.txt");
	vector<AI> population(100);
	for (int i = 0; i < 100; i++)
		population[i] = randomFromModel(model);

	for (int g = 0; g < 100; g++)
	{
		for (int i = 0; i < 100; i++)
		{
			double fitness = 1.0;
			population[i].neurons[0] = {0.0, 0.0};
			population[i].calculateOutput();
			fitness -= (population[i].neurons[2][0] - 0.0) * (population[i].neurons[2][0] - 0.0);

			population[i].neurons[0] = {0.0, 1.0};
			population[i].calculateOutput();
			fitness -= (population[i].neurons[2][0] - 1.0) * (population[i].neurons[2][0] - 1.0);

			population[i].neurons[0] = {1.0, 0.0};
			population[i].calculateOutput();
			fitness -= (population[i].neurons[2][0] - 1.0) * (population[i].neurons[2][0] - 1.0);

			population[i].neurons[0] = {1.0, 1.0};
			population[i].calculateOutput();
			fitness -= (population[i].neurons[2][0] - 0.0) * (population[i].neurons[2][0] - 0.0);
			population[i].fitness = fitness;
		}
		evolveToNextGen(population, 30, 30, 30, 10, 1.0);
	}
	cout << "Test for final neural net :)\n";
	population[0].neurons[0] = {0.0, 0.0};
	population[0].calculateOutput();
	cout << "Prediction for 0 xor 0 : " << (int)(population[0].neurons[2][0] + 0.5) << '\n';
	population[0].neurons[0] = {0.0, 1.0};
	population[0].calculateOutput();
	cout << "Prediction for 0 xor 1 : " << (int)(population[0].neurons[2][0] + 0.5) << '\n';
	population[0].neurons[0] = {1.0, 0.0};
	population[0].calculateOutput();
	cout << "Prediction for 1 xor 0 : " << (int)(population[0].neurons[2][0] + 0.5) << '\n';
	population[0].neurons[0] = {1.0, 1.0};
	population[0].calculateOutput();
	cout << "Prediction for 1 xor 1 : " << (int)(population[0].neurons[2][0] + 0.5) << '\n';
	population[0].saveToFile("save.txt");

	return 0;
}
