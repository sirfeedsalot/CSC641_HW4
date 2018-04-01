#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <random>

// Normal Distribution::Box Muller Method
double rand_nromal(double mean, double stddev)
{
	static double n2 = 0.0;
	static int n2_cached = 0;
	if (!n2_cached)
	{
		double x, y, r;
		do
		{
			x = 2.0*rand() / RAND_MAX - 1;
			y = 2.0*rand() / RAND_MAX - 1;

			r = x * x + y * y;
		} while (r == 0.0 || r > 1.0);
		{
			double d = sqrt(-2.0*log(r) / r);
			double n1 = x * d;
			n2 = y * d;
			double result = n1 * stddev + mean;
			n2_cached = 1;
			return result;
		}
	}
	else
	{
		n2_cached = 0;
		return n2 * stddev + mean;
	}
}

int main()
{
	// System Parameters
	int KIN, KOUT, QSIZE; // Input Dist, Output Dist, Queue Size
						  // Input Parameters
	int JMAX, JPRINT; // # of job departures, # of job departures betwen results printout
	double IDA; // interarrival time distribution
	double AA, SA; // Parameters of IDA
	double IDS; // Identifier of service time distribution
	double AS, SS; // Parameters of IDS
				   // Queue Variables
	QA(I);
	QS(I);

	system("pause");
	return 0;
}

