/*
 * Maneuver.h
 *
 *  Created on: 29.09.2020
 *      Author: pi
 */

#ifndef MANEUVER_H_
#define MANEUVER_H_

#include <cmath>
#include <string>
#include <list>

using namespace std;

struct Coord{
	double m_dX;
	double m_dY;
	double m_dV;
	Coord(double dX,double dY,double dV){
		m_dX=dX;
		m_dY=dY;
		m_dV=dV;
	}
};

class Maneuver{
public:
	Maneuver();
	~Maneuver();
	void CalcCircle(double dRadius, double dSpeed, double dTimestep);
	void CalcEight(double dRadius, double dSpeed, double dTimestep);
	void LogList(string sDatei);
	bool isRunning();
	void Start();
	void Stop();
	void Proceed();
	void CalcManeuverSpeed(double dX, double dY, double dW);
	double* GetManeuverSpeed();

private:
	struct Coord coord;
	list<Coord> CoordList;
	list<Coord>::iterator iter;
	bool bIsRunning;
	double adWishSpeed[2];
	double dMaxSpeed;
	double dPosDifference;
};



#endif /* MANEUVER_H_ */
