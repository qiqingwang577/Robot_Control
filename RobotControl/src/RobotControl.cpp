/*
 * RobotControl.cpp
 *
 *  Created on: 30.09.2020
 *      Author: pi
 */
#include "RobotControl.h"
#include <iostream>
#include <cmath>
using namespace std;

RobotControl* RobotControl::transferPointer;    //Der Pointer wird referenziert

double dTimestep = 0.04;

//PIDController(Kp, Ki, Kd, Ta)
RobotControl::RobotControl():motorR(PIDController(500, 100, 0, 0.04)), motorL(PIDController(500, 100, 0, 0.04)) {
	interface.Initialize(0.04, transferFunction);
	transferPointer = this;
	bIsActive = true;
	dSollV = maneuver.GetManeuverSpeed();
}

RobotControl::~RobotControl(){
}

void RobotControl::transferFunction() {
	transferPointer -> Step();

}
bool RobotControl::isActive() {
	return bIsActive;
}
void RobotControl::Step() {
	//Ist-Geschwindigkeit einlesen
	double* pIstV = interface.GetInput();
	dIstV[0] = pIstV[0];
	dIstV[1] = pIstV[1];

	if (maneuver.isRunning() == false) {
		//Roboter stoppen
		iMicros[0] = 1500;
		iMicros[1] = 1500;
		//motorR.Reset();
		//motorL.Reset();
	} else {
		//Positionsschaetzung, x- und y-Koodinaten, Winkel berechnen
		posEstimation.PredictPosition(dIstV[0], dIstV[1], dTimestep);
		double dX = posEstimation.GetPosition()[0];
		double dY = posEstimation.GetPosition()[1];
		double dW = posEstimation.GetPosition()[2];
		/*die Soll-Geschwindigkeit berechnen
		 fuer beiden Motoren durch der Daten der Liste(Circle,Eight)
		 und der Positionsschaetzung*/
		maneuver.CalcManeuverSpeed(dX, dY, dW);
		dSollV = maneuver.GetManeuverSpeed();
		//PID-Regler, Stellgroessen berechnen
		motorR.CalculateU(dSollV[0], dIstV[0]); //rechts
		motorL.CalculateU(dSollV[1], dIstV[1]); //links
		iMicros[0] = motorR.GetU() + 1500;
		iMicros[1] = motorL.GetU() + 1500;
		//Begrenzung
		if (iMicros[0] < 1000)
			iMicros[0] = 1000;
		if (iMicros[1] < 1000)
			iMicros[1] = 1000;
		if (iMicros[0] > 2000)
			iMicros[0] = 2000;
		if (iMicros[1] > 2000)
			iMicros[1] = 2000;
	}
	//Setzen der Geschwindigkeitswerte
	interface.SetOutputs(iMicros);
	printStatus();
}
void RobotControl::Communicate() {
	char cEingabe;
	double dRadius;
	double dSpeed;
	int iKreisOderAcht;
	cout << "Ob ein neues Manoever gefahren werden soll, Y/N?" << endl;
	cin >> cEingabe;
	if (cEingabe == 'n' || cEingabe == 'N')
		bIsActive = false;
	else if (cEingabe == 'y' || cEingabe == 'Y') {
		cout << "Bitte das gewunschte Radius in Meter eingeben" << endl;
		cin >> dRadius;
		cout << "Bitte die gewunschte Geschwindigkeit in m/s eingeben" << endl;
		cin >> dSpeed;
		cout << "Wie wollen Sie fahren? 8 fuer Acht-Manoever, 0 fuer Kreis-Manover" << endl;
		cin >> iKreisOderAcht;
		//Kurve berechnen
		if (iKreisOderAcht == 8)
			maneuver.CalcEight(dRadius, dSpeed, 0.1); //Timestep 0.04
		else if (iKreisOderAcht == 0)
			maneuver.CalcCircle(dRadius, dSpeed, 0.1);

		//x- und y- Koordinaten und Raumrichtung auf NULL zurueckgesetzt
		posEstimation.Reset();
		//ncurses starten
		initscr();
		nodelay(stdscr, TRUE);
		noecho();
		//Step()-Funktion starten
		sigprocmask(SIG_UNBLOCK, &interface.mask, nullptr);
		//maneuver's Introduction
		printw("Introduction: \n");
		printw("  w: start");
		printw("  b:stop");
		printw("  p:proceed");
		//
		char Eingabe = getch();
		while (Eingabe != 'q') {
			Eingabe = getch();
			if (Eingabe != -1) {
				switch (Eingabe) {
				case 'w':
					maneuver.Start();   //bIsRunning = true
					posEstimation.Reset();
					break;
				case 'b':
					maneuver.Stop();    //bIsRunning = false
					break;
				case 'p':
					maneuver.Proceed(); //bIsRunning = true
					break;
				}
			}
		}
		maneuver.Stop();
		if (Eingabe == 'q') {
			while (fabs(dIstV[0]) > 0.0001 || fabs(dIstV[1]) > 0.0001) {
				printw("Roboter faehrt noch!");
			}
		}
		//ncurses zumachen
		endwin();
		//Step()-Funktion stoppen
		sigprocmask(SIG_BLOCK, &interface.mask, nullptr);
	}
}

void RobotControl::printStatus(){
	clear();
	printw(maneuver.isRunning() ? "Running\n" : "Stopped\n");
	printw("IstV: %f, %f\n", dIstV[0], dIstV[1]);
	printw("Position: %f, %f, %f\n", posEstimation.GetPosition()[0], posEstimation.GetPosition()[1], posEstimation.GetPosition()[2]);
	//Coord;
	//
	printw("Maneuver Speed: %f, %f\n", dSollV[0], dSollV[1]);
	printw("Motor PID: %f, %f\n", motorR.GetU(), motorL.GetU());
	printw("Motor OUT: %d, %d\n", iMicros[0], iMicros[1]);
}


