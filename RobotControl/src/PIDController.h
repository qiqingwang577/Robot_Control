/*
 * PID_Regler.h
 *
 *  Created on: 28.09.2020
 *      Author: pi
 */

#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_

#include <iostream>

class PIDController{
public:
	PIDController(double Kp,double Ki,double Kd,double Ta);
	~PIDController();
	void CalculateU(double dW,double dY);
	double GetU();

private:
	double m_dKp, m_dKi, m_dKd, m_dTa;
	double m_dEsum, m_deold, m_dU;
};


#endif /* PIDCONTROLLER_H_ */
