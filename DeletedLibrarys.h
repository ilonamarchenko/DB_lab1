#pragma once
#include <cstdio>
#include <string>
#include<iostream>
#include <vector>
using namespace std;
class DeletedLibrarys
{
private:
	const char deletedAlbumsFilePath[24] = "DeletedLibrarysFile.fl";
public:
	void createFile()
	{
		FILE* deletedAlbums;
		fopen_s(&deletedAlbums, deletedAlbumsFilePath, "w+b");
		fclose(deletedAlbums);
		return;
	}
	bool checkFileExists(string& error)
	{
		FILE* deletedAlbums;
		fopen_s(&deletedAlbums, deletedAlbumsFilePath, "r+b");
		if (deletedAlbums == NULL) {
			error = "Library file does not exist";
			createFile();
			return 0;
		}
		fclose(deletedAlbums);
		return 1;
	}
	void addDeletedAdress(int adress)
	{
		FILE* deletedAlbumsFile;
		fopen_s(&deletedAlbumsFile, deletedAlbumsFilePath, "a+b");
		fseek(deletedAlbumsFile, 0, SEEK_END);
		fwrite(&adress, sizeof(int), 1, deletedAlbumsFile);
		fclose(deletedAlbumsFile);
		return;
	}
	int readDeletedAdress(int adress)
	{
		int deletedAdress;
		FILE* deleteAlbumsFile;
		fopen_s(&deleteAlbumsFile, deletedAlbumsFilePath, "r+b");
		fseek(deleteAlbumsFile, adress, SEEK_SET);
		fread(&deletedAdress, sizeof(int), 1, deleteAlbumsFile);
		fclose(deleteAlbumsFile);
		return deletedAdress;
	}
	int getNumberOfAdresses()
	{
		FILE* deletedAlbumsFile;
		fopen_s(&deletedAlbumsFile, deletedAlbumsFilePath, "r+b");
		fseek(deletedAlbumsFile, 0, SEEK_END);
		int n = ftell(deletedAlbumsFile) / sizeof(int);
		fclose(deletedAlbumsFile);
		return n;
	}
	int getFirstDeletedAdress()
	{
		int n = getNumberOfAdresses();
		if (n == 0) {
			return -1;
		}
		vector<int>adresses(n);
		for (int i = 0; i < n; ++i) {
			adresses[i] = readDeletedAdress(i * sizeof(int));
		}

		createFile();
		for (int i = 1; i < n; ++i) {
			addDeletedAdress(adresses[i]);
		}
		int firstAdress = adresses[0];
		return firstAdress;
	}
};

