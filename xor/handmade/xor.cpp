#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>

using namespace std;
#include "../../include/AI.h"

int main()
{
	AI neuralNet = AI("xor.txt");
	int input;
	cout << "Enter first bit : ";
	cin >> input;
	neuralNet.neurons[0][0] = (double)input;
	cout << "Enter second bit : ";
	cin >> input;
	neuralNet.neurons[0][1] = (double)input;
	neuralNet.calculateOutput();
	cout << "Neuron 0 of hidden layer : " << neuralNet.neurons[1][0] << '\n';
	cout << "Neuron 1 of hidden layer : " << neuralNet.neurons[1][1] << '\n';
	cout << "Output neuron : " << neuralNet.neurons[2][0] << '\n';
	cout << "Result of XOR : " << (int)(neuralNet.neurons[2][0] + 0.5) << '\n';
	return 0;
}
