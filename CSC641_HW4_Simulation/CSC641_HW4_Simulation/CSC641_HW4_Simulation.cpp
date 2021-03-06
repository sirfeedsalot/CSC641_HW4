//File: CSC641_HW4_Simulation.cpp
//Author: Richard Chhay
//Institution: San Francisco State University
//Date: April 2, 2018
//Course: CSC 641 COMPUTER PERFORMANCE EVALUATION
//Summary: This program seeks to provide two features both involving G/G/1 Queues. 
//			In the QueueSimulation, 
//				a simulation of G/G/1 queues is provided with queue sizes of size 1000. The function returns
//				for a number of jobs: total time, avg interarrival time, stddev interarrival time, average
//				service time, stddev service time, average response time, stddev response time, average # of jobs,
//				stddev # of jobs, utilization time, and throughput rate.
//			In the QueueCalculator,
//				the function asks for the desired distribution of interarrival time and service time. Parameters
//				for each distribution are asked for. The function then returns server utilization, mean response
//				time, mean queue length, and time spent waiting in queue (before service).
//Sources:
//			Queue Calculator Function
//			Author: Jozo Dujmovic
//			Title: G/G/1 Calculator
//			Retrieved from iLearn

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <random>
#include <chrono>
#include <deque>
#include <queue>

using namespace std;

/* Uniform Distribution*/
double uniRNG(double TMIN, double TMAX) {
	return TMAX*(rand() / RAND_MAX) + TMIN;
}

/* Exponential Distribution*/
double expRNG(double a) {
	default_random_engine generator();
	exponential_distribution<double> distribution(a); // exponential distribution with mean time (lambda)
	return distribution(generator);
}

/* Constant Distribution*/
double constRNG(double a) {
	return a;
}

/* Select Distribution*/
double makeRN(char id, double a, double TMIN, double TMAX) {
	switch (id) {
	case '1':
		return constRNG(a);
	case '2':
		return expRNG(a);
	case '3':
		return uniRNG(TMIN, TMAX);
	default:
		return 0;
	}
}

/* Queue Max Size = 1000
	Calculation interval on powers of 10
	Distribution Choices: (1) Constant, (2) Exponential, (3) Uniform*/
void queueSim(void) {
	char adist, sdist; // identifier for distributions
	double meanA = 0., meanS = 0., sTMIN = 0., sTMAX = 0., aTMIN = 0., aTMAX = 0.; // mean interarrival time, minimum time, maximum time
	int JOBA = 0, JOBS = 0, NJOB = 0, JMAX; // counts number of events occured
	double TNA = 0., TND = 0., TBUSY = 0.; // time next arrival, time next departure, current simulation time, total server busy time
	double TS, TA, DELTAT; // Random service time, random interarrival time, time increment between events
	const int QL = 10;
	vector <int> JPRINT; // Number of job departures between result printout

	/* Statistical Indicators*/
	double TIME = 0., ATA = 0., SDTA = 0., ATS = 0., SDTS = 0., ART = 0., SDRT = 0., AJOB = 0., SDJOB = 0., UTIL = 0., Y = 0., JOBSEC = 0.;
	vector <double> xTIME, xATA, xSDTA, xATS, xSDTS, xART, xSDRT, xAJOB, xSDJOB, xUTIL, xY, xJOBSEC;

	/* total time, average time arrival, stddev time arrival, average time service, 
		stddev time service, average response time, stddev response time, 
		average # of jobs, stddev # of jobs, utilization time, throughput rate*/

	cout << "\n\n========== G/G/1 Simulator ===========\n\n\n";
	cout << "Please enter the number of job departures you wish to be simulated:  ";
	cin >> JMAX;
	cout << endl;
	for (int i = 0; i < JMAX % 10000; i++) {
		JPRINT.push_back(i*10000);
	}
	int N = JPRINT.size(); // N is the number of elements in JPRINT
	queue <double> QA; // Arrival Queue
	queue <double> QS; // Service Queue

	// Begin interarrival
	cout << "\n\n-------------------------------------------------------"
		<< "\n\nThe following choices for interarrival time distributions are available: "
		<< "\n	(1) Constant"
		<< "\n	(2) Exponential"
		<< "\n	(3) Uniform"
		<< "\nEnter your choice: ";
	cin >> adist;
	if (adist == '1') {	cout << "You chose Constant Distribution. Enter the mean interarrival time: "; cin >> meanA;	}
	else if (adist == '2') { cout << "You chose Exponential Distribution. Enter the mean interarrival time: "; cin >> meanA;	}
	else if (adist == '3') { cout << "You chose Uniform Distribution. Enter the minimum and maximum arrival time: "; cin >> aTMIN >> aTMAX;	}
	// end interarrival
	
	// begin service
	cout << "\n\n-------------------------------------------------------"
		<< "\n\nThe following choices for service time distributions are available: "
		<< "\n	(1) Constant"
		<< "\n	(2) Exponential"
		<< "\n	(3) Uniform"
		<< "\nEnter your choice: ";
	cin >> sdist;
	if (sdist == '1'){ cout << "You chose Constant Distribution. Enter the mean service time: "; cin >> meanS;	}
	else if (sdist == '2') { cout << "You chose Exponential Distribution. Enter the mean service time: "; cin >> meanS;		}
	else if (sdist == '3') { cout << "You chose Uniform Distribution. Enter the minimum and maximum service time: "; cin >> sTMIN >> sTMAX;	}
	// end service
	// Distributions for interarrival and service time are noted as adist and sdist.

	int EVENT = 0; TIME = 0.;
	/*		Main Simulation Loop		*/
	// When # of departures == JMAX, end loop
	
	
	for (int JOBS = 0; JOBS < JMAX;) {
		// TND Section, Job Departure
		if ((TND < TNA)&&(TND!=TNA)) {
			NJOB = JOBA - JOBS;
			DELTAT = TND - TIME;
			TBUSY += DELTAT;
			JOBSEC = JOBSEC + DELTAT * NJOB;
			SDJOB += DELTAT * NJOB*NJOB;
			TIME = TND;
			JOBS++;
			ATS += QS.front();
			SDTS += QS.front()*QS.front();
			ART += TIME - QA.front();
			SDRT += (TIME - QA.front())*(TIME - QA.front());
		}
		if (JOBS == JOBA){	TND = TNA;	}
		if ( ((JOBS % 10000) == 0) ) { // JOBS is not a multiple of 10000 ie) the set {10k, 20k, 30k, ... nk} where k is 10000
			// Store statistics for printing
			// xTIME, xATA, xSDTA, xATS, xSDTS, xART, xSDRT, xAJOB, xSDJOB, xUTIL, xY, xJOBSEC
			xTIME.push_back(TIME);
			xATA.push_back(ATA);
			xSDTA.push_back(SDTA);
			xATS.push_back(ATS);
			xSDTS.push_back(SDTS);
			xART.push_back(ART);
			xSDRT.push_back(SDRT);
			xAJOB.push_back(AJOB);
			xSDJOB.push_back(SDJOB);
			xUTIL.push_back(UTIL);
			xY.push_back(Y);
			xJOBSEC.push_back(JOBSEC);
		}	else {
			continue;
		}

		// TNA Section, Job Arrival
		// Generate two events, every arrival will have some service time
		// Update current time, which is the current time + new arrive time
		TA = makeRN(adist, meanA, aTMIN, aTMAX); // generate arrival event
		TS = makeRN(sdist, meanS, sTMIN, sTMAX); // generate service event
		/* Updating Statistics*/
		JOBA++;
		ATA = ATA + TA;
		SDTA = SDTA + (TA*TA);
		if (NJOB > 0) {
			DELTAT = TNA - TIME;
			TBUSY += DELTAT;
			JOBSEC += (DELTAT*NJOB);
			SDJOB += DELTAT * NJOB*NJOB;
		} else { 
			TIME = TNA; 
			TNA = TIME + TA;
			QA.push(TIME);
			QS.push(TS);
		}
	}

	/* print results */
	cout << "\n\n";
	cout << left
		<< setw(10) << "JOBS"
		<< setw(10) << "TIME"
		<< setw(10) << "ATA"
		<< setw(10) << "SDTA"
		<< setw(10) << "ATS"
		<< setw(10) << "SDTS"
		<< setw(10) << "ART"
		<< setw(10) << "SDRT"
		<< setw(10) << "AJOB"
		<< setw(10) << "SDJOB"
		<< setw(10) << "UTIL"
		<< setw(10) << "X";
	cout << "\n=================================================================================================================\n\n";
	/*
	for (int i = 0; i < 12; i++) {
		cout << left
			<< setw(12) << JPRINT[i]
			<< setw(12) << xTIME[i]
			<< setw(12) << xATA[i]
			<< setw(12) << xSDTA[i]
			<< setw(12) << xATS[i]
			<< setw(12) << xSDTS[i]
			<< setw(12) << xART[i]
			<< setw(12) << xSDRT[i]
			<< setw(12) << xAJOB[i]
			<< setw(12) << xSDJOB[i]
			<< setw(12) << xUTIL[i]
			<< setw(12) << xY[i] << endl;
	}
	*/
}

// Program provided by Jozo Dujmovic, Fall 2010
void queueCalc(void) {
	char adist, sdist;
	double S, U, vs, va, tMin, tMax, a, X, R;

	cout << "\n========== G/G/1 Calculator ==========";
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
		}
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
		}
		U = S * X;
		R = (S / (1. - U))*(1 - 0.5*U*(1. - vs * vs - ((vs*vs + 1.)*(va*va - 1.) / (U*U*vs*vs + 1.))));
		cout << "\nServer utilization U = " << U
			<< "\nMean response time R = " << R
			<< "\nMean queue length Q = " << ((adist == '1'&&sdist == '1') ? U : U / (1 - U))
			<< "\nTime spent waiting W = " << R - S;
}

int main()
{
	queueSim();
	queueCalc();
	
	system("pause");
	return 0;
}

