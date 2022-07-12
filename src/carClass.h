#pragma once
#include <iostream>
#include <string>
using namespace std;

class myCars
{
public:
	string id;
	string time, hh, mm, ss, date, day, month, year;

	myCars(string idd,string time1, string date1)
	{
		date = date1;
		time = time1;
		id = idd;
		hh = time1.substr(0, 2);
		mm = time1.substr(3, 2);
		ss = time1.substr(6, 2);

		day = date1.substr(0, 2);
		month = date1.substr(3, 2);
		year = date1.substr(6, 4);
	}
};