#pragma once
#include <string>
#include "shortString.h"
using namespace std;
class Library
{
public:
	int id;
	shortString name;
	shortString publisher;
	shortString date;
	int firstBookAdress;
	bool isDeleted;
	Library(int id, string name, string recordStudio, string date, int firstSongAdress, bool isDeleted)
	{
		this->id = id;
		this->name = name;
		this->publisher = recordStudio;
		this->date = date;
		this->firstBookAdress = firstSongAdress;
		this->isDeleted = isDeleted;
	}
	Library(string name, string recordStudio, string date)
	{
		this->name = name;
		this->publisher = recordStudio;
		this->date = date;
        firstBookAdress = -1;
		isDeleted = 0;
		id = -1;
	}
	Library()
	{
		id = 0;
        firstBookAdress = -1;
		isDeleted = 0;
	}
};

