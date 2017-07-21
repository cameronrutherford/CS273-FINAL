//Project: Simulate the operations of an Emergency Room
//File: Implement the functions defined in EmergencyRoom.h
//By: Cameron Rutherford 
//Last Updated: 21/07/2017

#include "EmergencyRoom.h"
#include <sstream>

bool EmergencyRoom::medicsFree() //if all the medics are busy returns false
{ 
	int check = 0; //counts the number of busy doctors
	for (unsigned i = 0; i < Medics.size(); i++) {
		if (Medics[i]->isBusy()) {
			check++;
		}
	}
	if (check == Medics.size()) //if all the doctors are busy
		return false; //return false
	else
		return true; //otherwise a doctor is free and return true
}

bool EmergencyRoom::docsFree() //if all the doctors are busy returns false
{
	int check = 0; //counts the number of busy doctors
	int docCount = 0; //counts the number of doctors
	for (unsigned i = 0; i < Medics.size(); i++) {
		if (typeid(*(Medics[i])) == typeid(Doctor)) {
			docCount++;
			if (Medics[i]->isBusy()) {
				check++;
			}
		}
	}
	
	if (check == docCount) //if all the doctors are busy
		return false; //return false
	else
		return true; //otherwise there is a free doctor so return true
}

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
void EmergencyRoom::updateDoctors()
{
	for (unsigned i = 0; i < Medics.size(); i++) { //for all the doctors

		if (Medics[i]->isBusy()) { //that currently have a patient

			std::pair<bool, Patient> check = Medics[i]->update(clock); //run the update function

			if (check.first) { //if the doctor is finished treating their patient

				//Update the records with the patient that was discharged
				Patient departing = check.second;
				numTreated++;
				totalWait += departing.getVisitTime();
				Record[departing.getName()].push_back(departing.getPriority()); //add them to the record
			}
		}
	}
	//while either line has people in it and a Medic is free
	while (medicsFree() && (!patientsOver10.empty() || !patientsUnder10.empty())) {

		//while there are >10 priority patients waiting and a doctor is free
		while (!patientsOver10.empty() && docsFree()) {

			for (unsigned i = 0; i < Medics.size(); i++) { //searching through the Medics
				if (typeid(Doctor) == typeid((*Medics[i])) && !Medics[i]->isBusy()) { //if we found a doctor that isn't busy

					//treat the new patient with that doctor
					Medics[i]->treatPatient(patientsOver10.top());
					patientsOver10.pop();
					break;
				}
			}

		}
		
		//re check the necessary parts of the while condition
		if (!medicsFree() || patientsUnder10.empty())
			break;

		//now we are just treating all under 10 patients with any medic
		for (unsigned i = 0; i < Medics.size(); i++) {

			//if a medic is free
			if (!Medics[i]->isBusy()) {
				Medics[i]->treatPatient(patientsUnder10.top());
				patientsUnder10.pop();
				break;
			}
		}
	}
}

EmergencyRoom::EmergencyRoom(int numDocs, int numNurses, int arrivalRate) : arrivalRate(arrivalRate), numTreated(0), totalWait(0), clock(0)
{
	buildDatabase(names); //fills the names vector with all the names in the file

	//we add the correct number of Nurses first as this means that when treating patients of <10 priority,
	//we make sure to check for Nurses first as they are more likely to be more efficient than doctors

	for (int i = 0; i < numNurses; i++) {
		Medics.push_back(new Nurse());
	}

	for (int i = 0; i < numDocs; i++) {
		Medics.push_back(new Doctor());
	}
}

void EmergencyRoom::update() //Adds a new patient if necessary, and then calls updateDoctors()
{
	clock++; //updates the clock

	if (needNewPatient(arrivalRate)) { //if we are ready to add a new patient (from RandomGeneration.h)

		//Generate a new patient
		Patient newPatient(clock, names);

		//add then to the right queue
		if (newPatient.getPriority() > 10) {
			patientsOver10.push(newPatient);
		}
		else {
			patientsUnder10.push(newPatient);
		}

	}
	//call the ugly update doctors function
	updateDoctors(); 
}

void EmergencyRoom::listNames()
{
	//iterate through the Record and output all the names (which are the keys to the map in this case)
	std::map<std::string, std::list<int>>::iterator it = Record.begin();
	int i = 0;
	for (;it != Record.end(); i++, ++it) {
		std::cout << it->first << ", ";
		if (i % 10 == 0 && i != 0)
			std::cout << std::endl;
	}
}

std::string EmergencyRoom::getRecord(std::string name) //searched the Record for a person by name and returns a nicely formatted string of their record
{
	std::map<std::string, std::list<int>>::iterator it = Record.find(name);

	if (it == Record.end()) //if the name was not found
		return std::string("No records were found under that name\n");

	else { //if the name was found...
		
		//Create a nicely formatted string with the Record in it
		std::stringstream ss;
		ss << "Name: " << it->first << "\n";
		std::list<int> illnesses = it->second;
		ss << "Illness severities: ";
		int illnesscount = 0; //counts the number of illnesses

		for (std::list<int>::iterator it = illnesses.begin(); it != illnesses.end();) {
			ss << *it;
			illnesscount++;
			if (++it != illnesses.end())
				ss << ", ";
		}
		ss << "\n" << "Total number of illnesses: " << illnesscount << "\n";
		return ss.str();
	}
}
