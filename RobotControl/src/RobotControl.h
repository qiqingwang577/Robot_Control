/*
 * RobotControl.h
 *
 *  Created on: 30.09.2020
 *      Author: pi
 */

#ifndef ROBOTCONTROL_H_
#define ROBOTCONTROL_H_

#include "InterfaceSIM.h"
#include "Maneuver.h"
#include "PosEstimation.h"
#include "PIDController.h"

class RobotControl{
public:
	RobotControl();
	~RobotControl();
	static void transferFunction();
	bool isActive();
	void Step();
	void Communicate();
	static RobotControl* transferPointer;
	void printStatus();
private:
	InterfaceSIM interface;
	Maneuver maneuver;
	PosEstimation posEstimation;
	PIDController motorR;
	PIDController motorL;
	bool bIsActive;
	double dIstV[2];
	double* dSollV;
	int iMicros[2] = {1500, 1500};
};


#endif /* ROBOTCONTROL_H_ */
