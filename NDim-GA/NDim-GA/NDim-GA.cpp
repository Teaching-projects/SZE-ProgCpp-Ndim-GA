#include "stdafx.h"
#include "NDim-GA.h"


NDim_GA::NDim_GA(int dimension, int populationSize, int generationSize, int crossoverMax, int testFunctionId, double min, double max) : EvoAlgs(generationSize, populationSize, dimension, testFunctionId, min, max)
{
	
	crossover = crossoverMax;
	NDim_GA::simulate();
}

void NDim_GA::simulate()
{
	// POPULATION INIT
	vector<Chromosome> population = vector<Chromosome>();
	populationInit(population);

	for (int i = 0; i < generation; i++)
	{
		//INIT vars
		vector<Wheel> w = vector<Wheel>();
		vector<Chromosome> popbuff = vector<Chromosome>();

		NDim_GA::wheelInit(population, w);
		/*for (int j = 0; j < popsize; j++)
		{
		printf("orderv: %f fitness: %f\n", w[j].getOrderValue(), w[j].getChromosome().getFitness());
		}*/
		double roulettemaxval = w[0].getOrderValue();
		double rouletteminval = w[populationSize - 2].getOrderValue();
		//TODO
		//Valammiért túl megy a tömbön 
		popbuff = selection(rouletteminval, roulettemaxval, population, w);
		/*
		*/
		for (int j = 0; j < populationSize; j++)
		{
			printf("fitness: %f\n", popbuff[j].getFitness());
		}
		// TODO
		// selectSurvivors
		population = popbuff;
	}
	for (int i = 0; i < populationSize; i++)
	{
		printf("fitness: %f\n", population[i].getFitness());
	}
}


/**
*	Roulette Wheel Init
*
*	Used for roulette wheel selection. Generate a wheel based on the current population.
*	@param population is the current population
*	@param w is the roulette wheel
*/
void NDim_GA::wheelInit(vector<Chromosome> population, vector<Wheel> &w) {
	double minval, maxval;

	for (Chromosome ch : population)
	{
		w.push_back(Wheel(ch));
	}

	sort(w.begin(), w.end());
	minval = w.begin()->getOrderValue();
	maxval = w.rbegin()->getOrderValue();
	for (vector<Wheel>::iterator it = w.begin(); it != w.end(); it++) {
		it->setOrderValue(abs(1000 * it->getOrderValue() - (abs(1000 * minval) + abs(1000 * maxval) + 1)));
	}
	for (int j = EvoAlgs::populationSize - 2; j >= 0; j--)
	{
		w[j].setOrderValue(w[j].getOrderValue() + w[j + 1].getOrderValue());
	}
}

vector<Chromosome> NDim_GA::selection(double rouletteminval, double roulettemaxval, vector<Chromosome> population, vector<Wheel> w) {

	vector<Chromosome> popbuff = vector<Chromosome>();
	crossover = (int)randomFloatNM(1, populationSize*((double)crossover / 100.0));
	for (int j = 0; j < crossover; j++)
	{
		Chromosome parent1;
		Chromosome parent2;

		double randombuff = randomFloatNM(rouletteminval, roulettemaxval - 1);
		double randombuff2 = randomFloatNM(rouletteminval, roulettemaxval - 1);
		bool p1 = false;
		bool p2 = false;

		//	starts from the wheel's end, and runs until both parents are selected
		for (vector<Wheel>::reverse_iterator it = w.rbegin(); !p1 || !p2; it++)
		{
			if ((randombuff <= it->getOrderValue()) && !p1)
			{

				parent1 = it->getChromosome();
				p1 = true;;
			}
			if ((randombuff2 <= it->getOrderValue()) && !p2)
			{

				parent2 = it->getChromosome();
				p2 = true;;
			}

		}
		popbuff.push_back((parent1, parent2));
	}
	return popbuff;
}

void NDim_GA::selectingSurvivors(double rouletteminval, double roulettemaxval, vector<Chromosome> population, vector<Wheel> w, vector<Chromosome> &popbuff) {
	for (int j = 0; j < populationSize - crossover; j++)
	{
		Chromosome survivor = population.front();

		double randombuff = randomFloatNM(rouletteminval, roulettemaxval + 1);
		for (vector<Wheel>::reverse_iterator it = w.rbegin(); it != w.rend(); it++) {
			if ((int)round(randombuff <= (int)round(it->getOrderValue()))) {
				survivor = it->getChromosome();
				break;
			}
		}
		popbuff.push_back(survivor);
	}
}

/**
*	Single-point: A single crossover point on both parents' organism strings is selected. All data beyond that point in either organism string is swapped between the two parent organisms.
*
*	@params parent1 is the first parent
*	@params parent2 is the second parent
*/
Chromosome NDim_GA::single_point(Chromosome parent1, Chromosome parent2)
{
	Chromosome child = Chromosome();
	if ((int)randomFloatNM(1, 100) % 2 == 0)
	{
		child.setCoordinate(parent2.getCoordinate());
		for (int k = 0; k < (int)randomFloatNM(1, dimension); k++)
		{
			child.setCoordinateById(k, parent1.getCoordinate()[k]);
		}
	}
	else
	{
		child.setCoordinate(parent1.getCoordinate());
		for (int k = 0; k < (int)randomFloatNM(1, dimension); k++)
		{
			child.setCoordinateById(k, parent2.getCoordinate()[k]);
		}
	}
	child.setFitness(getFitness(child.getCoordinate()));
	return child;
}