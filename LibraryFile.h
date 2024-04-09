#pragma once
#include "Library.h"
#include "indexFile.h"
#include <cstdio>
#include <string>
#include<iostream>
#include "DeletedLibrarys.h"
using namespace std;

class LibraryFile
{
private:
	const char albumFilePath[15] = "LibraryFile.fl";
public:
	void createFile()
	{
		FILE* albumFile;
		fopen_s(&albumFile, albumFilePath, "w+b");
		fclose(albumFile);
		return;
	}

	bool checkFileExists(string& error)
	{
		FILE* albumFile;
		fopen_s(&albumFile, albumFilePath, "r+b");
		if (albumFile == NULL) {
			error = "Library file does not exist";
			createFile();
			return 0;
		}
		fclose(albumFile);
		return 1;
	}

	int getSeekEnd()
	{
		int seekEnd;
		FILE* albumFile;
		fopen_s(&albumFile, albumFilePath, "r+b");
		fseek(albumFile, 0, SEEK_END);
		seekEnd = ftell(albumFile);
		fclose(albumFile);
		return seekEnd;
	}

	Library readAlbum(int adress)
	{
		Library library;
		FILE* albumFile;
		fopen_s(&albumFile, albumFilePath, "r+b");
		fseek(albumFile, adress, SEEK_SET);
		fread(&library, sizeof(Library), 1, albumFile);
		fclose(albumFile);
		return library;
	}

	void changeAlbum(Library library, int adress)
	{
		FILE* albumFile;
		fopen_s(&albumFile, albumFilePath, "r+b");
		fseek(albumFile, adress, SEEK_SET);
		fwrite(&library, sizeof(Library), 1, albumFile);
		fclose(albumFile);
		return;
	}

	void writeAlbum(Library library)
	{
		FILE* albumFile;
		fopen_s(&albumFile, albumFilePath, "a+b");
		fseek(albumFile, 0, SEEK_END);
		fwrite(&library, sizeof(Library), 1, albumFile);
		fclose(albumFile);
		return;
	}

	Library getLybrary(int key, string& error)
	{
		IndexFile fl = IndexFile();
		if (!fl.checkKeyIsReal(key, error))return Library();
		int adress = fl.getAdressByKey(key, error);
		if (error != "")return Library();
		return readAlbum(adress);
	}

	int addLibrary(Library library, string& error)
	{
		DeletedLibrarys dAl = DeletedLibrarys();
		int adress = dAl.getFirstDeletedAdress();
		IndexFile iFl = IndexFile();
		library.id = iFl.getLastKey() + 1;
		if (adress == -1) {
			iFl.addIndex(getSeekEnd(), error);
			writeAlbum(library);
		}
		else {
			iFl.addIndex(adress, error);
			if (error != "")return 0;
			changeAlbum(library, adress);
		}
		return library.id;
	}


	void updateLibrary(int key, Library library, string& error)
	{
		IndexFile iFl = IndexFile();
		int adress = iFl.getAdressByKey(key, error);
		if (error != "")return;
		changeAlbum(library, adress);
		return;
	}

};

