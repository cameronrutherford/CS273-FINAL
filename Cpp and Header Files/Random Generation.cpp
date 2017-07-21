//Project: Simulate the operations of an Emergency Room
//File: Implement the functions defined in RandomGeneration.h
//By: Cameron Rutherford 
//Last Updated: 21/07/2017

#include "Random Generation.h"
#include <time.h>
#include <fstream>
#include <iostream>

void buildDatabase(std::vector<std::string>& names) //gets all the names from the text file and stores them to the vector
{
	srand(time(NULL));
	std::ifstream surnames;
	surnames.open("C:\\Users\\Cameron Rutherford\\Desktop\\Uni\\Summer 2017\\CS 273 - Data Structures\\FINAL PROJECT\\surnames_of_273ville.txt");
	if (surnames) {
		std::string name;
		while (getline(surnames, name)) {
			names.push_back(name);
		}
		surnames.close();
	}
	else {
		//This could be better in terms of exception handling...
		names.push_back("Default Patient Name - Error reading from file");
		std::cout << "Failed to open the file\n";
	}
}

int randomPriority() //generates a random illness priority using the probabilities defined in the outline
					 //being 70% to be 1-10, 20% to be 11-15, 10% to be 15-20
{
	int x = rand() % 10;
	if (x <= 6)
		return rand() % 10;
	else if (x <= 8)
		return 11 + rand() % 5;
	else
		return 16 + rand() % 5;
}

std::string randomSurname(std::vector<std::string>& names) //picks a random name from the list
{
	int i = rand() % names.size();
	return names[i];
}

bool needNewPatient(int ratePerHour) //Uses the inputted rate to randomly decide if a new patient needs to be generated
{
	int ranNum = rand() % 60;
	if (ranNum < ratePerHour)
		return true;
	else
		return false;
}
