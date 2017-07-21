//Project: Simulate the operations of an Emergency Room
//File: 
//By: Cameron Rutherford 
//Last Updated: 21/07/2017

#include "EmergencyRoom.h"
#include "Menu.h"
#include <iostream>
#include <algorithm>

using namespace std;

int main() {

	cout << "Welcome to the Emergency Room simulator!\n\n";
	string prompt = "What would you like the arrival rate of patients to be (patients/hr): ";
	int rate = read_int(prompt, 1, 60);
	prompt = "How many doctors would you like there to be (1-20): ";
	int docs = read_int(prompt, 1, 20);
	prompt = "How many nurses would you like there to be (1-20): ";
	int nurses = read_int(prompt, 1, 20);

	EmergencyRoom theRoom(docs, nurses, rate); //create the emergency room based on the user's input

	long long int FINISHTIME = 7 * 24 * 60; //simulating for one week

	while (theRoom.getTime() < FINISHTIME) { //run the simulation until the time is up
		theRoom.update();
	}

	double averageWait = theRoom.getTotalWait() / theRoom.getNumTreated();
		
	cout << "The average visit time for each patient was " << averageWait << " minutes.\n\n";

	string	menu_string = "Would you like to: \n";
	menu_string += "   0 - See the list of patient names\n";
	menu_string += "   1 - Search for the details of a patient by name\n";
	menu_string += "   2 - Exit\n";
	menu_string += "Enter: ";

	while (true) {
		int choice = read_int(menu_string, 0, 2);
		if (choice == 2)
			//EXIT
			break;
		else if (choice == 0)
			//List all the names
			theRoom.listNames();
		else {
			//Search the Record for a particular name
			cout << "What name would you like to search for: ";
			string name;
			cin >> name;
			transform(name.begin(), name.end(), name.begin(), toupper);
			cout << theRoom.getRecord(name);
		}
	}

	return 0;
}