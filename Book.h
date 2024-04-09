#pragma once
#include<string>
#include "shortString.h"
using namespace std;

class Book
{
public:
	int pages;
	shortString name;
	shortString genre;
	bool isDeleted;
	int nextAdress;
	int libraryId;
	int songId;
	Book(int pages, string name, string genre, bool isDeleted, int nextAdress, int albumId, int songId)
	{
		this->pages = pages;
		this->name = name;
		this->genre = genre;
		this->isDeleted = isDeleted;
		this->nextAdress = nextAdress;
		this->libraryId = albumId;
		this->songId = songId;
	}
	Book()
	{
		pages = 0;
		isDeleted = 0;
		nextAdress = -1;
        libraryId = 0;
		songId = 0;
	}
};

