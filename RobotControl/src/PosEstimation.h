/*
 * PosEstimation.h
 *
 *  Created on: 28.09.2020
 *      Author: pi
 */

#ifndef POSESTIMATION_H_
#define POSESTIMATION_H_
#include <iostream>
#include <cmath>

class PosEstimation{
public:
	PosEstimation();
	~PosEstimation();
	void Reset();
	void PredictPosition(double dSpeedR, double dSpeedL, double dTimestep);
	double* GetPosition();

private:
	double x[3];
	double dVelAverage;
};

#endif /* POSESTIMATION_H_ */
