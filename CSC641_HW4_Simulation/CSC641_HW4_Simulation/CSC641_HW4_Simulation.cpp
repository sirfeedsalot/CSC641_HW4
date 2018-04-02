#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <random>

using namespace std;
// Main program provided by Jozo Dujmovic, Fall 2010

/* Uniform Distribution*/
double uniRNG(double TMIN, double TMAX) {
	default_random_engine generator;
	uniform_real_distribution<double> distribution(TMIN, TMAX); // uniform distribution with lower and upper bounds (tMin, tMax)
	return distribution(generator);
}

/* Exponential Distribution*/
double expRNG(double lambda) {
	default_random_engine generator;
	exponential_distribution<double> distribution(lambda); // exponential distribution with mean time (lambda)
	return distribution(generator);
}

/* Constant Distribution*/
double constRNG(double a) {
	return a;
}

/* Queue Max Size = 1,000,000
	Calculation interval on powers of 10*/
void queueSim(void) {
	cout << "\n\n===== G/G/1 Simulator =====";
	double QA[500]; // Arrival Queue
	double QS[500]; // Service Queue

}

int main()
{
	char adist, sdist;
	double S, U, vs, va, tMin, tMax, a, X, R;
	
	cout << "\n========== G/G/1 Calculator ==========";
	while (1) {
		cout << "\n\n-------------------------------------------------------"
			<< "\n\nThe following choices for interarrival time distributions are available: "
			<< "\n	(1) Constant"
			<< "\n	(2) Exponential"
			<< "\n	(3) Uniform"
			<< "\nEnter your choice: ";
		cin >> adist;
		if (adist == '1') { va = 0.; cout << "Constant interarrival time = "; cin >> a; X = 1. / a; }
		else if (adist == '2') { va = 1.; cout << "Mean interarrival time = "; cin >> a; X = 1. / a; }
		else if (adist == '3') {
			cout << "Enter min and max interarrival time, separate the values with spaces: ";
			cin >> tMin >> tMax;
			a = (tMax + tMin) / 2;
			X = 1. / a;
			va = (tMax - tMin) / ((tMax + tMin)*sqrt(3.));
		}	else { return 1; }
		cout << "\n\nThe following choices for service time distributions are available: "
			<< "\n	(1) Constant"
			<< "\n	(2) Exponential"
			<< "\n	(3) Uniform"
			<< "\nEnter your choice: ";
		cin >> sdist;
		if (sdist == '1') { vs = 0.; cout << "Constant service time = "; cin >> S; }
		else if (sdist == '2') { vs = 1.; cout << "Mean service time = "; cin >> S; }
		else if (sdist == '3') {
			cout << "Enter min and max service time, separate the values with spaces: ";
			cin >> tMin >> tMax;
			S = (tMax + tMin) / 2;
			vs = (tMax - tMin) / ((tMax + tMin)*sqrt(3.));
		}	else { return 1; }
		U = S * X;
		R = (S / (1. - U))*(1 - 0.5*U*(1. - vs * vs - ((vs*vs + 1.)*(va*va - 1.) / (U*U*vs*vs + 1.))));
		cout << "\nServer utilization U = " << U
			<< "\nMean response time R = " << R
			<< "\nMean queue length Q = " << ((adist == '1'&&sdist == '1') ? U : U / (1 - U))
			<< "\nTime spent waiting W = " << R-S;
	}


	system("pause");
	return 0;
}

