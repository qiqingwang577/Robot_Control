/*
 * Maneuver.cpp
 *
 *  Created on: 29.09.2020
 *      Author: pi
 */

#include <iostream>
#include "Maneuver.h"
#include <string>
#include <fstream>
#include <cmath>
using namespace std;

Maneuver::Maneuver():coord(Coord(0,0,0)){
	bIsRunning = false;
	adWishSpeed[0] = 0.0;
	adWishSpeed[1] = 0.0;
	dMaxSpeed = 0.5;        // max. Wunschgeschwindigkeit in m/s
	dPosDifference = 0.02;  // max. Abweichung zwischen Soll- und Ist-Position in m
}

Maneuver::~Maneuver(){
}

void Maneuver::CalcCircle(double dRadius, double dSpeed, double dTimestep){
	CoordList.clear();
	for(int counter = 1; counter < (int)((2*M_PI)/((dSpeed/dRadius)*dTimestep)); counter++){
		coord.m_dX = dRadius*sin(counter*(dSpeed/dRadius)*dTimestep);
		coord.m_dY = dRadius*(1-cos(counter*(dSpeed/dRadius)*dTimestep));
		//hinzufuegen fuer RoboterControl
		coord.m_dV = dSpeed;
		CoordList.push_back(coord);
	}
	iter = CoordList.begin();
	LogList("LogFileCircle.txt");
}

void Maneuver::CalcEight(double dRadius, double dSpeed, double dTimestep){
	CoordList.clear();
	for(int counter = 1; counter < (int)((2*M_PI)/((dSpeed/dRadius)*dTimestep)); counter++){
			coord.m_dX = dRadius*sin(counter*(dSpeed/dRadius)*dTimestep);
			coord.m_dY = dRadius*(1-cos(counter*(dSpeed/dRadius)*dTimestep));
			//hinzufuegen fuer RoboterControl
			coord.m_dV = dSpeed;
			CoordList.push_back(coord);
		}
	for(int counter = 1; counter < (int)((2*M_PI)/((dSpeed/dRadius)*dTimestep)); counter++){
			coord.m_dX = dRadius*sin(counter*(dSpeed/dRadius)*dTimestep);
			coord.m_dY = -dRadius*(1-cos(counter*(dSpeed/dRadius)*dTimestep));
			//hinzufuegen fuer RoboterControl
			coord.m_dV = dSpeed;
			CoordList.push_back(coord);
		}
	iter = CoordList.begin();
	LogList("LogFileEight.txt");

}

void Maneuver::LogList(string sDatei){
	ofstream ofile;
	ofile.open(sDatei,ios::out|ios::trunc);
	//list<Coord>::iterator iter;
	for(iter = CoordList.begin(); iter!= CoordList.end(); iter++){
		ofile << iter->m_dX << "\t" << iter->m_dY << endl;
	}
}

bool Maneuver::isRunning(){
	return bIsRunning;
}

void Maneuver::Start(){
	iter = CoordList.begin();
	bIsRunning = true;
}

void Maneuver::Stop(){
	bIsRunning = false;
}

void Maneuver::Proceed(){
	bIsRunning = true;
}

void Maneuver::CalcManeuverSpeed(double dX, double dY, double dW){
	double dPosDifferenceX = fabs(iter->m_dX - dX); //absolut fuer float
	double dPosDifferenceY = fabs(iter->m_dY - dY);

	if(dPosDifferenceX < dPosDifference && dPosDifferenceY < dPosDifference){
		iter++;
	}

		if(iter == CoordList.end()){
			adWishSpeed[0] = 0.0;
			adWishSpeed[1] = 0.0;
			Stop();
		}
		else{
			double dWinkel = atan2((iter->m_dY-dY),(iter->m_dX-dX));
			double dDeltaWinkel = dWinkel - dW;

			if(dDeltaWinkel <= -M_PI){
				dDeltaWinkel += 2*M_PI;
			}
			if(dDeltaWinkel > M_PI){
				dDeltaWinkel -= 2*M_PI;
			}

			double dRot = 2 * dDeltaWinkel;
			if(dRot < -0.5){
				dRot = -0.5;
			}
			if(dRot > 0.5){
				dRot = 0.5;
			}

			double dTra = iter->m_dV;
			if(dTra * dRot > 0.0){
				if(dTra + dRot > dMaxSpeed){
					dTra = dMaxSpeed - dRot;
				}
				if(dTra + dRot < -dMaxSpeed){
					dTra = -dMaxSpeed - dRot;
				}
			}
		    if(dTra * dRot < 0.0){
				if(dTra - dRot > dMaxSpeed){
					dTra = dMaxSpeed + dRot;
				}
				if(dTra - dRot < -dMaxSpeed){
					dTra = -dMaxSpeed + dRot;
				}
			}

		    adWishSpeed[0] = dTra + dRot;
		    adWishSpeed[1] = dTra - dRot;
	    }

}

double* Maneuver::GetManeuverSpeed(){
	return adWishSpeed;
}
