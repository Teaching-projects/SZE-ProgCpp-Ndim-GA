// NDim-GA.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "NDim-GA.h"


int main()
{
	NDim_GA(1,1,1);
    return 0;
}

NDim_GA::NDim_GA(int dimension, int populationSize, int generationSize)
{
	dim = dimension;
	popsize = populationSize;
	generation = generationSize;
	NDim_GA::simulate();
}

void NDim_GA::simulate()
{
	// POPULATION INIT
	vector<Chromosome> population = vector<Chromosome>();
	populationInit(population);

	for (int i = 0; i < generation; i++)
	{
		// TODO
		// wheelInit

		// TODO
		// selection

		// TODO
		// selectSurvivors
	}
}


/**
*	Population initialization
*
*	//
*
*/
void NDim_GA::populationInit(vector<Chromosome> &population) {
	vector<double> point;
	double fitness;
	for (int i = 0; i < popsize; i++)
	{
		point = createNDimPoint();
		fitness = getFitness(point);
		population.push_back(Chromosome(point, fitness));
	}
}


/**
*	Creates an N dimension point
*/
vector<double> NDim_GA::createNDimPoint() {
	vector<double> point = vector<double>();
	for (int i = 0; i < dim; i++)
	{
		point.push_back(randomFloatNM(-4, 4));
	}
	return point;
}


/**
*	Returns fitness from test fucntions
*	
*/
double NDim_GA::getFitness(vector<double> points) {
	return TestFunctions::AckleyFunction(points);
}


/**
*	Generates a random float number
*	
*/
double NDim_GA::randomFloatNM(double xmin, double xmax) {
	return (xmin + (xmax - xmin)*((double)rand() / RAND_MAX));
}