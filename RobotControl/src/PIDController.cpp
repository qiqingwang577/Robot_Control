/*
 * PIDController.cpp
 *
 *  Created on: 28.09.2020
 *      Author: pi
 */

#include "PIDController.h"

PIDController::PIDController(double Kp,double Ki,double Kd,double Ta){
	m_dKp = Kp;
	m_dKi = Ki;
	m_dKd = Kd;
	m_dTa = Ta;  //Abtastzeit
	m_dEsum = 0.0;
	m_deold = 0.0;
	m_dU = 0.0;
}

PIDController::~PIDController(){
}

void PIDController::CalculateU(double dW,double dY){
	double de_k = dW - dY;   //dW:Sollwert, dY:Istwert
	m_dEsum = m_dEsum + de_k;
	m_dU = m_dKp * de_k + m_dKi * m_dTa * m_dEsum + m_dKd * (de_k-m_deold)/m_dTa;
    m_deold = de_k;
}

double PIDController::GetU(){
	return m_dU;
}


