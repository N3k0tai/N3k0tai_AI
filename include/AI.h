double rangeRand(double minValue, double maxValue)
{
	double tmp = (double)rand() / RAND_MAX;
	return minValue + tmp * (maxValue - minValue);
}

class AI
{
public:
	vector<vector<double>> neurons;
	vector<vector<double>> biases;
	vector<vector<vector<double>>> weights;

	double fitness;

	AI(){}
	~AI(){}

	AI(const char* filename)
	{
		ifstream file(filename);
		int numLayers;
		file >> numLayers;
		for (int i = 0; i < numLayers; i++)
		{
			int numNeurons;
			file >> numNeurons;
			neurons.push_back(vector<double>(numNeurons));

			biases.push_back(vector<double>(numNeurons));
			if (i == 0)
			{
				weights.push_back(vector<vector<double>>(numNeurons, vector<double>(0, 0)));
				continue;
			}

			weights.push_back(vector<vector<double>>(numNeurons, vector<double>(neurons[i-1].size())));


			for (int j = 0; j < numNeurons; j++)
			{
				double value;
				file >> value;
				biases[i][j] = value;
			}

			for (int j = 0; j < numNeurons; j++)
			{
				for (int k = 0; k < neurons[i-1].size(); k++)
				{
					double value;
					file >> value;
					weights[i][j][k] = value;
				}
			}
		}
		file.close();
	}

	AI(const vector<int>& numNeuronsPerLayer)
	{
		int numLayers = numNeuronsPerLayer.size();
		for (int i = 0; i < numLayers; i++)
		{
			int numNeurons = numNeuronsPerLayer[i];
			neurons.push_back(vector<double>(numNeurons));

			biases.push_back(vector<double>(numNeurons));
			if (i == 0)
			{
				weights.push_back(vector<vector<double>>(numNeurons, vector<double>(0, 0)));
				continue;
			}

			weights.push_back(vector<vector<double>>(numNeurons, vector<double>(neurons[i-1].size())));

			for (int j = 0; j < numNeurons; j++)
			{
				biases[i][j] = rangeRand(-10.0, 10.0);
			}

			for (int j = 0; j < numNeurons; j++)
			{
				for (int k = 0; k < neurons[i-1].size(); k++)
				{
					weights[i][j][k] = rangeRand(-10.0, 10.0);
				}
			}
		}
	}

	void saveToFile(const char* filename)
	{
		ofstream file(filename);
		int numLayers = neurons.size();
		file << numLayers << '\n';
		for (int i = 0; i < numLayers; i++)
		{
			int numNeurons = neurons[i].size();
			file << numNeurons << '\n';
			if (i == 0)
				continue;

			for (int j = 0; j < numNeurons; j++)
			{
				double value = biases[i][j];
				file << value << '\n';
			}

			for (int j = 0; j < numNeurons; j++)
			{
				for (int k = 0; k < neurons[i-1].size(); k++)
				{
					double value = weights[i][j][k];
					file << value << '\n';
				}
			}
		}
		file.close();
	}

	double sigmoid(double x)
	{
		return 1.0 / (1 + exp(-x));
	}

	void calculateNeuron(int layer, int number)
	{
		double sum = 0;
		for (int i = 0; i < neurons[layer-1].size(); i++)
			sum += neurons[layer-1][i]*weights[layer][number][i];
		sum += biases[layer][number];
		neurons[layer][number] = sigmoid(sum);
	}

	void calculateOutput()
	{
		for (int i = 1; i < neurons.size(); i++)
			for (int j = 0; j < neurons[i].size(); j++)
				calculateNeuron(i, j);
	}
};


AI mutate(const AI& sourceAI, double factor)
{
	AI tmp = sourceAI;
	for (auto& vec1 : tmp.weights)
		for (auto& vec2 : vec1)
			for (auto& weight : vec2)
				if (rand()%20 == 0)
					weight += rangeRand(-2.0*factor, 2.0*factor);
	for (auto& vec : tmp.biases)
		for (auto& bias : vec)
			if (rand()%20 == 0)
				bias += rangeRand(-2.0*factor, 2.0*factor);
	return tmp;
}

AI crossover(const AI& sourceAI1, const AI& sourceAI2)
{
	AI tmp = sourceAI1;
	for (int i = 0; i < tmp.weights.size(); i++)
	{
		for (int j = 0; j < tmp.weights[i].size(); j++)
		{
			for (int k = 0; k < tmp.weights[i][j].size(); k++)
			{
				int randomBool = rand() % 2;
				if (randomBool == 0)
					tmp.weights[i][j][k] = sourceAI2.weights[i][j][k];
			}
		}
	}

	for (int i = 0; i < tmp.biases.size(); i++)
	{
		for (int j = 0; j < tmp.biases[i].size(); j++)
		{
			int randomBool = rand() % 2;
			if (randomBool == 0)
				tmp.biases[i][j] = sourceAI2.biases[i][j];
		}
	}
	return tmp;
}

AI randomFromModel(const AI& sourceAI)
{
	AI tmp = sourceAI;
	for (auto& vec1 : tmp.weights)
		for (auto& vec2 : vec1)
			for (auto& weight : vec2)
				weight = rangeRand(-10.0, 10.0);
	for (auto& vec : tmp.biases)
		for (auto& bias : vec)
			bias = rangeRand(-10.0, 10.0);
	return tmp;
}


void evolveToNextGen(vector<AI>& population, int numUnchanged, int numMutations, int numCrossovers, int numNew, double mutateFactor)
{
	// numUnchanged + numMutations + numCrossovers + numNew = population size

	sort(population.begin(), population.end(), 
		[](const AI& ai1, const AI& ai2){return ai1.fitness > ai2.fitness;});


	for (int i = numUnchanged; i < numUnchanged + numMutations; i++)
	{
		int index = i % numUnchanged;
		population[i] = mutate(population[index], mutateFactor);
	}
	for (int i = numUnchanged + numMutations; i < numUnchanged + numMutations + numCrossovers; i++)
	{
		int index1 = i % numUnchanged;
		int index2 = (i*2) % numUnchanged;
		population[i] = crossover(population[index1], population[index2]);
	}
	for (int i = numUnchanged + numMutations + numCrossovers; i < population.size(); i++)
	{
		population[i] = randomFromModel(population[0]);
	}
}