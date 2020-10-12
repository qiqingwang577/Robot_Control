//============================================================================
// Name        : RobotControl.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

using namespace std;
#include <iostream>
#include "RobotControl.h"

int main(){
	RobotControl robotControl;
	do{
		robotControl.Communicate(); //parallel Step()-Funktion aufgerufen
	}
	while(robotControl.isActive()); //Bedingung: .isActive() auf false gesetzt
	return 0;
}
