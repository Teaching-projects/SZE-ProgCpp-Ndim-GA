#include "stdafx.h"
#include "NDim-GA.h"


NDim_GA::NDim_GA(int dimension, int populationSize, int generationSize, int crossoverId, int crossoverMax, int testFunctionId, double mutationMax, double min, double max) : EvoAlgs(generationSize, populationSize, dimension, testFunctionId, min, max)
{
	crossoverType = crossoverId;
	crossover = crossoverMax;
	mutation = mutationMax;
	NDim_GA::simulate();
}

void NDim_GA::simulate()
{
	// INIT population
	vector<Chromosome> population = vector<Chromosome>();
	populationInit(population);

	for (int i = 0; i < generation; i++)
	{
		//INIT vars
		vector<Wheel> w = vector<Wheel>();
		vector<Chromosome> popbuff = vector<Chromosome>();

		// INIT wheel
		NDim_GA::wheelInit(population, w);
		double roulettemaxval = w[0].getOrderValue();
		double rouletteminval = w[populationSize - 2].getOrderValue();

		// Functions
		popbuff = selection(rouletteminval, roulettemaxval, population, w);
		selectingSurvivors(rouletteminval, roulettemaxval, population, w, popbuff);
		doMutation(popbuff);
		population = popbuff;
		sort(population.begin(), population.end());
		for (int i = 0; i < populationSize; i++)
		{
			printf("fitness: %f\n", population[i].getFitness());
		}
		printf("\n\n");
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
		popbuff.push_back(doCrossover(parent1, parent2));
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
*
*	Mutation is a genetic operator used to maintain genetic diversity from one generation of a population of genetic algorithm chromosomes to the next.
*
*	@param popbuff is the next population
*/
void NDim_GA::doMutation(vector<Chromosome> &popbuff) {
	for (vector<Chromosome>::iterator it = popbuff.begin(); it != popbuff.end(); it++) {
		if (randomFloatNM(1, 10000) <= (int)((((double)mutation)*100.0)))
		{
			it->setCoordinateById((int)randomFloatNM(0, dimension - 1), randomFloatNM(minimum, maximum));
			it->setFitness(getFitness(it->getCoordinate()));
		}
	}
}


/**
*
*	Crossover is a genetic operator used to vary the programming of a chromosome or chromosomes from one 
*	generation to the next.
*	There are several type of crossover and CROSSOVERTYPE constans decides which one will be used.
*
*/
Chromosome NDim_GA::doCrossover(Chromosome parent1, Chromosome parent2)
{
	switch (crossoverType) {
	case 0:
		return single_point(parent1, parent2);
	case 1:
		return two_point(parent1, parent2);
	case 2:
		return uniform(parent1, parent2);
	default:
		return parent1;
	}
}

/**
*	Single-point: A single crossover point on both parents' organism strings is selected. 
*	All data beyond that point in either organism string is swapped between the two parent organisms.
* 
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

/**
*	Two-point: Two-point crossover calls for two points to be selected on the parent organism strings. 
*	Everything between the two points is swapped between the parent organisms.
*
*/
Chromosome NDim_GA::two_point(Chromosome parent1, Chromosome parent2)
{
	Chromosome child = Chromosome();
	if ((int)randomFloatNM(1, 100) % 2 == 0)
	{
		child.setCoordinate(parent1.getCoordinate());
		for (int i = (int)randomFloatNM(1, (int)dimension / 2); i < dimension - (int)randomFloatNM(1, (int)dimension / 2); i++)
		{
			child.setCoordinateById(i, parent2.getCoordinate()[i]);
		}
	}
	else {
		child.setCoordinate(parent2.getCoordinate());
		for (int i = (int)randomFloatNM(1, (int)dimension / 2); i < dimension - (int)randomFloatNM(1, (int)dimension / 2); i++)
		{
			child.setCoordinateById(i, parent1.getCoordinate()[i]);
		}
	}
	child.setFitness(getFitness(child.getCoordinate()));
	return child;
}

/**
*	Uniform: The uniform crossover uses a fixed mixing ratio between two parents. 
*	Unlike single- and two-point crossover, the uniform crossover enables the parent chromosomes to 
*	contribute the gene level rather than the segment level.
*
*/
Chromosome NDim_GA::uniform(Chromosome parent1, Chromosome parent2)
{
	Chromosome child = Chromosome(parent1.getCoordinate());
	for (int i = 0; i < dimension; i++)
	{
		child.setCoordinateById(i, (int)randomFloatNM(1, 100) % 2 ? parent1.getCoordinate()[i] : parent2.getCoordinate()[i]);
	}
	child.setFitness(getFitness(child.getCoordinate()));
	return child;
}