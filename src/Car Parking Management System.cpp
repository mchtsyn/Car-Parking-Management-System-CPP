#include <iostream>
#include <string>
#include <random>
#include <cmath>
#include <vector>
#include <ctime>
#include <iomanip>
#include "carClass.h"
using namespace std;

int currMenu = 0;
double moneyCollected = 0;

void clear();
char* currentDateAndTime();
int mainMenu();
void addCarMenu(vector<myCars>& cars, int& totalCars, int& deletedCars);
void parkedCars(vector<myCars>& cars);
void deleteCarMenu(vector<myCars>& cars, int& totalCars, int& deletedCars);
double calculateMoney(string arrivalTime, string arrivalDate);

int main()
{
	currentDateAndTime();
	vector<myCars> cars;
	int currentCarsParked = 0;
	int deletedCars = 0;
	currMenu = mainMenu();

	while (currMenu != 5)
	{
		switch (currMenu)
		{
			case(1): //Add car
			{
				clear();
				addCarMenu(cars, currentCarsParked, deletedCars);
				break;
			}
			case(2): //Delete car
			{
				clear();
				deleteCarMenu(cars, currentCarsParked, deletedCars);
				break;
			}
			case(3): //Disp parked cars
			{
				clear();
				parkedCars(cars);
				break;
			}
			case(4): //Disp total money
			{
				clear();
				char choice = ' ';
				cout << "Today, " << deletedCars << " customers paid a total of " << moneyCollected << "TL." << endl;
				cout << "Enter (m) to return main menu: ";
				cin >> choice;
				if (choice == 'm') currMenu = 6;
				clear();
				break;
			}
			case(6):
			{
				//Default menu
				currMenu = mainMenu();
			}
		}
	}
}
int mainMenu()
{
	int choice = -1;
	cout << "************************************************" << endl;
	cout << "      CAR PARKING MANAGEMENT SYSTEM" << endl<<endl;
	cout << "1. Add a car to the system (Arrival) " << endl;
	cout << "2. Delete a car from the system (Departure) " << endl;
	cout << "3. Parked cars" << endl;
	cout << "4. Total money earned" << endl;
	cout << "5. Exit" << endl;
	cout << "************************************************" << endl;

	cout << "Enter your choice: ";
	cin >> choice;
	return choice;

}

char* currentDateAndTime()
{
	time_t ttime = time(0);

	char* dt = ctime(&ttime);
	cout << dt << endl;
	return dt;
}

void clear()
{
#if defined _WIN32
	system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	system("clear");
#elif defined (__APPLE__)
	system("clear");
#endif
}

void addCarMenu(vector<myCars>& cars, int& totalCars, int& deletedCars)
{
	string time = "";
	string hh, mm, ss;
	string date = "";
	string day, month,year;
	string plateNumber = "";
	while (time.size() != 8 || date.size() != 10 || plateNumber.size()<5 || plateNumber.size() > 7) //invalid time handling
	{
		cout << "Enter the plate number: ";
		cin >> plateNumber;
		cout << "Enter arrival date (as day/month/year): ";
		cin >> date;
		cout << "Enter arrival time (as hh:mm:ss): ";
		cin >> time;
		if (time.size() != 8) cout << "Invalid time, please try again.\n";
		if (date.size() != 10) cout << "Invalid date, please try again.\n";
		if(plateNumber.size() < 5 || plateNumber.size() > 7) cout << "Invalid plate number, please try again.\n";
	}
	totalCars++;

	for (int i = 0;i < plateNumber.size();i++)
	{
		plateNumber[i] = toupper(plateNumber[i]); //Plate number to uppercase
	}
	myCars myCar(plateNumber,time,date);
	cars.push_back(myCar);
	cout << "\nA car is added to the system with plate number: " << plateNumber <<endl;

	char choice = ' ';
	cout << "\nEnter (m) to return main menu: ";
	cin >> choice;
	if (choice == 'm')
	{
		currMenu = 6;
		clear();
	}
}

void deleteCarMenu(vector<myCars>& cars, int& totalCars, int& deletedCars)
{
	string deletePlateNum = ""; 
	int searchIDinCars = cars.size();

	while (searchIDinCars == cars.size())
	{
		//ID search error handling
		searchIDinCars = 0;
		cout << "Enter the plate number of the parked car you want to delete from system: ";
		cin >> deletePlateNum;

		for (int i = 0; i < cars.size(); i++)
		{
			if (deletePlateNum != cars[i].id) searchIDinCars++; // Try to find the ID == deleteIndex
		}
		if (searchIDinCars == cars.size()) cout << "Invalid ID, please try again." << endl; //If ID does not exist
	}

	for (int i = 0;i < deletePlateNum.size();i++)
	{
		deletePlateNum[i] = toupper(deletePlateNum[i]); //Plate number to uppercase
	}
	int deleteIndex;
	for (int i = 0; i < cars.size(); i++)
	{
		if (cars[i].id == deletePlateNum) deleteIndex = i;
	}

	string timeDeleted = cars[deleteIndex].time;
	string dateDeleted = cars[deleteIndex].date;

	double getMoneyFromCustomer = calculateMoney(timeDeleted, dateDeleted);
	moneyCollected += getMoneyFromCustomer;
	cout << "You need to get " << getMoneyFromCustomer << "TL from the customer\n";
	cars.erase(cars.begin() + deleteIndex); //Delete car
	deletedCars++;

	//Exit deleteCarMenu
	char finalChoice = ' ';
	cout << "Success. Enter (m) to return to main menu, enter (c) to cancel the deletion and return to main menu: ";
	cin >> finalChoice;


	if (finalChoice == 'm')
	{
		clear();
		currMenu = 6;
	}
	else if (finalChoice == 'c')
	{
		myCars deletedCarRecreate(deletePlateNum, timeDeleted, dateDeleted);
		cars.push_back(deletedCarRecreate);
		moneyCollected -= getMoneyFromCustomer;
		deletedCars--;
		clear();
		currMenu = 6;
	}
}

void parkedCars(vector<myCars>& cars)
{
	vector<string> idAndSizeSort;
	for (int i = 0; i < cars.size(); i++)
	{
		idAndSizeSort.push_back(cars[i].id);
	}
	sort(idAndSizeSort.begin(), idAndSizeSort.end());

	cout << "-------------------------------------------------------------------------------------\n";
	cout << setw(4) << "ID" << setw(23) << "ARRIVAL DATE" << setw(27) << "ARRIVAL TIME" << endl;
	cout << "-------------------------------------------------------------------------------------\n";
	for (int i = 0; i < cars.size(); i++)
	{
		for (int j = 0; j < cars.size(); j++)
		{
			if(cars[j].id == idAndSizeSort[i]) cout << setw(0) << idAndSizeSort[i] << setw(19) << cars[j].date << setw(26)<<cars[j].time << endl;
		}
	}

	char toMainMenu = ' ';
	while (toMainMenu != 'm')
	{
		cout << "\nEnter (m) to return main menu: ";
		cin >> toMainMenu;
	}
	clear();
	currMenu = 6; //Navigate to main menu
}

double calculateMoney(string arrivalTime, string arrivalDate)
{
	string deptime="";
	string depDate = "";
	while ((depDate.substr(0,2)==arrivalDate.substr(0,2) && depDate.substr(3, 2) == arrivalDate.substr(3, 2)) &&  (stoi(deptime.substr(0,2)) < stoi(arrivalTime.substr(0, 2))) || deptime == "") //deptime == "" for taking inputs initially
	{
		//Same day but departure time is before the arrival time
		cout << "Enter the departure date (day/month/year): ";
		cin >> depDate;
		cout << "Enter the departure time (hh:mm:ss): ";
		cin >> deptime;
		if ((depDate.substr(0, 2) == arrivalDate.substr(0, 2) && depDate.substr(3, 2) == arrivalDate.substr(3, 2)) && (stoi(deptime.substr(0, 2)) < stoi(arrivalTime.substr(0, 2))))
			cout << "Invalid date or time, please try again.\n";
	}


	if (((depDate.substr(0, 2) == arrivalDate.substr(0, 2) && (stoi(deptime.substr(0,2)) >= stoi(arrivalTime.substr(0, 2)))) || (stoi(depDate.substr(0,2)) > stoi(arrivalDate.substr(0, 2)) && (stoi(deptime.substr(0,2))-stoi(arrivalTime.substr(0,2))<0 )) )) //same day departure
	{
		int mmTotDep = stoi(deptime.substr(0, 2)) * 60 + stoi(deptime.substr(3, 2));
		int mmTotArr = stoi(arrivalTime.substr(0, 2)) * 60 + stoi(arrivalTime.substr(3, 2));
		int mmDiff;
		if (mmTotDep < mmTotArr)
		{
			mmDiff = (24*60) - (mmTotArr - mmTotDep);
		}
		else if (mmTotDep >= mmTotArr)
		{
			mmDiff = mmTotDep - mmTotArr;
		}

		//10TL(turkish liras) for half an hour parking
		double money = mmDiff/ 30. * 10.;
		return money;

	}
	else if (stoi(depDate.substr(0, 2)) > stoi(arrivalDate.substr(0, 2)) && stoi(deptime.substr(0,2))>=stoi(arrivalTime.substr(0, 2)))  //more than 24hrs passed departure
	{
		int dayToMins = abs(stoi(depDate.substr(0, 2)) - stoi(arrivalDate.substr(0, 2))) *24*60 /2; // day-to-mins: divide by 2 for discounted price
		int mmTotDep = stoi(deptime.substr(0, 2)) * 60 + stoi(deptime.substr(3, 2));
		int mmTotArr = stoi(arrivalTime.substr(0, 2)) * 60 + stoi(arrivalTime.substr(3, 2));
		int mmDiff;
		if (mmTotDep < mmTotArr)
		{
			mmDiff = (24 * 60) - mmTotArr - mmTotDep;
		}
		else if (mmTotDep >= mmTotArr)
		{
			mmDiff = mmTotDep - mmTotArr;
		}
		double money = (mmDiff+dayToMins) / 30 * 10;
		return money;
	}

}

