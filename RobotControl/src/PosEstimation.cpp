/*
 * PosEstimation.cpp
 *
 *  Created on: 28.09.2020
 *      Author: pi
 */

#include "PosEstimation.h"

PosEstimation::PosEstimation(){
	x[0] = 0.0;
	x[1] = 0.0;
	x[2] = 0.0;
	dVelAverage = 0.0;
}

PosEstimation::~PosEstimation(){
}

void PosEstimation::Reset(){
	x[0] = 0.0;
	x[1] = 0.0;
	x[2] = 0.0;
}

// aktuelle Position durch vorherige Position, Geschw. und Winkel berechnen
void PosEstimation::PredictPosition(double dSpeedR, double dSpeedL, double dTimestep){
    // x[0] : x-Koordinate
	// x[1] : y-Koordinate
	// x[2] : Raumrichtung
	x[0] = x[0] + dVelAverage * dTimestep * cos(x[2]);
    x[1] = x[1] + dVelAverage * dTimestep * sin(x[2]);

    double deltaWinkel = dTimestep * (dSpeedR - dSpeedL)/0.3229;//0.23
    x[2] = x[2] + fmod(deltaWinkel,2 * M_PI);
  /*  if(x[2] > M_PI){
    	x[2] -= 2 * M_PI;
    }
    if(x[2] < -M_PI){
    	x[2] += 2 * M_PI;
    }*/
    dVelAverage = (dSpeedR + dSpeedL)/2;
}

double* PosEstimation::GetPosition(){
	return x;
}
