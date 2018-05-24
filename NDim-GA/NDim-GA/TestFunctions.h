#pragma once

#include "stdafx.h"
#include "vector"
#define _USE_MATH_DEFINES
#include <cmath>

#define MPI 3.14159265358979323846
#define ACKLEY_A 20
#define ACKLEY_B 0.2
#define ACKLEY_C MPI*2

using namespace std;

class TestFunctions
{
public:
	static double AckleyFunction(vector<double> points);
private:
};
