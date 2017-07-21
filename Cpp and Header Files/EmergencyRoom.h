//Project: Simulate the operations of an Emergency Room
//File: Define the emergency room class that is the basis for the simulation
//By: Cameron Rutherford 
//Last Updated: 21/07/2017

#ifndef EMERGENCYROOM_H_
#define EMERGENCYROOM_H_

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <list>
#include "Patient.h"
#include "Physician.h"
#include "Random Generation.h"

class EmergencyRoom {
private:
	int arrivalRate; //in terms of patients per hour - determines the arrival rate
	long long int clock; //used to keep track of how long the sim has been running for
						 //A long long int is used, when in reality maybe just a long int could be used.
						 //It was kept this way as it can be useful to test for longer periods of time to investigate behaviour.
						 //if this is changed, it will also cause alot of other things to change
	double numTreated; //used to keep track of the number of patients successfully treated
	double totalWait; //used in conjunction with numTreated to find average wait time
	std::vector<std::string> names; //used in producing a random name for a patient

	//The two pqueues need to be kept seperate as it is important to be able to find patients with priority of above and below 10 quickly.
	//It does complicate some sections of the code in terms of checking to see if there are patients needed to be treated,
	//but overall it makes things easier.

	std::priority_queue<Patient> patientsOver10; //used to keep track of the patients with a priority of over 10
	std::priority_queue<Patient> patientsUnder10; //used to keep track of the patients with a priority of below 10


	std::vector<Physician *> Medics; //A vector to hold all of the Physicians in the emergency room
	std::map<std::string, std::list<int>> Record; //Used to store the records of all the patients that have been treated

	bool medicsFree(); //if all the medics are busy returns false
	bool docsFree(); //if all the doctors are busy returns false
	/*
	This function does the following things in order:
	1. Updates and checks to see if any Medics are finished treating their patients
			a) if they are, the appropriate records are updated
	2. Assigns any >10 priority patients (if there are any) to the available doctors (if there are any)
	3. Assigns and <10 priority patients (if there are any) to the available medics (if there are any)
	 
	This has a side effect of having doctors be able to finish with a patient and begin treating another patient all in the same minute.

	Also note that the function cycles through the medics in the order they were added, and so by adding nurses first when the object is created,
	it avoids a situation where a doctor unecessarily treats a patient if an available nurse could have instead, at a potentially faster rate.
	*/
	void updateDoctors();

public:
	double getNumTreated() { return numTreated; }
	double getTotalWait() { return totalWait; }
	EmergencyRoom(int numDocs, int numNurses, int arrivalRate); //Custom Constructor. There is no need for a default constructor.
	void update(); //Adds a new patient if necessary, and then calls updateDoctors()
	long long int getTime() { return clock; }
	void listNames(); //displays all the names of the people in the Record
	std::string getRecord(std::string name); //searched the Record for a person by name and returns a nicely formatted string of their record
};

#endif