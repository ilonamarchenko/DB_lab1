#pragma once
#include <cstdio>
#include <string>
#include<iostream>
#include <vector>
using namespace std;
class DeletedBooks
{
private:
	const char deletedSongsFilePath[20] = "DeletedBooksFile.fl";
public:
	void createFile()
	{
		FILE* deletedSongs;
		fopen_s(&deletedSongs, deletedSongsFilePath, "w+b");
		fclose(deletedSongs);
		return;
	}
	bool checkFileExists(string& error)
	{
		FILE* deletedSongs;
		fopen_s(&deletedSongs, deletedSongsFilePath, "r+b");
		if (deletedSongs == NULL) {
			error = "Library file does not exist";
			createFile();
			return 0;
		}
		fclose(deletedSongs);
		return 1;
	}
	void addDeletedAdress(int adress)
	{
		FILE* deletedSongs;
		fopen_s(&deletedSongs, deletedSongsFilePath, "a+b");
		fseek(deletedSongs, 0, SEEK_END);
		fwrite(&adress, sizeof(int), 1, deletedSongs);
		fclose(deletedSongs);
		return;
	}
	int readDeletedAdress(int adress)
	{
		int deletedAdress;
		FILE* deletedSongs;
		fopen_s(&deletedSongs, deletedSongsFilePath, "r+b");
		fseek(deletedSongs, adress, SEEK_SET);
		fread(&deletedAdress, sizeof(int), 1, deletedSongs);
		fclose(deletedSongs);
		return deletedAdress;
	}
	int getNumberOfAdresses()
	{
		FILE* deletedSongs;
		fopen_s(&deletedSongs, deletedSongsFilePath, "r+b");
		fseek(deletedSongs, 0, SEEK_END);
		int n = ftell(deletedSongs) / sizeof(int);
		fclose(deletedSongs);
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

