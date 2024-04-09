#pragma once
#include "Book.h"
#include <cstdio>
#include <string>
#include<iostream>
#include "LibraryFile.h"
#include "DeletedBooks.h"
#include <vector>
using namespace std;

class BookFile
{
private:
	const char songFilePath[12] = "BookFile.fl";
	int count = 0;
public:
	void createFile()
	{
		FILE* songFile;
		fopen_s(&songFile, songFilePath, "w+b");
		fclose(songFile);
		return;
	}

	bool checkFileExists(string& error)
	{
		FILE* songFile;
		fopen_s(&songFile, songFilePath, "r+b");
		if (songFile == NULL) {
			error = "Library file does not exist";
			createFile();
			return 0;
		}
		fclose(songFile);
		return 1;
	}

	Book readSong(int adress)
	{
		Book book;
		FILE* songFile;
		fopen_s(&songFile, songFilePath, "r+b");
		fseek(songFile, adress, SEEK_SET);
		fread(&book, sizeof(Book), 1, songFile);
		fclose(songFile);
		return book;
	}
	void changeSong(Book book, int adress)
	{
		FILE* songFile;
		fopen_s(&songFile, songFilePath, "r+b");
		fseek(songFile, adress, SEEK_SET);
		fwrite(&book, sizeof(Book), 1, songFile);
		fclose(songFile);
		return;
	}

	void writeSong(Book book)
	{
		FILE* songFile;
		fopen_s(&songFile, songFilePath, "a+b");
		fseek(songFile, 0, SEEK_END);
		fwrite(&book, sizeof(Book), 1, songFile);
		fclose(songFile);
		return;
	}

	int getSeekEnd()
	{
		int seekEnd;
		FILE* songFile;
		fopen_s(&songFile, songFilePath, "r+b");
		fseek(songFile, 0, SEEK_END);
		seekEnd = ftell(songFile);
		fclose(songFile);
		return seekEnd;
	}

	bool checkSongExists(int albumKey, int songKey, string& error)
	{
		LibraryFile aFl = LibraryFile();
		Library library = aFl.getLybrary(albumKey, error);
		if (error != "")return 0;
		int adress = library.firstBookAdress;
		while (adress != -1) {
			Book book = readSong(adress);
			if (book.songId == songKey) {
				if (book.isDeleted == 1) {
					error = "book with key = " + to_string(songKey) + " was deleted";
				}
				return !book.isDeleted;
			}
			adress = book.nextAdress;
		}
		error = "book with key = " + to_string(songKey) + " does not exist";
		return 0;
	}

	int getAdressOfSong(int albumKey, int songKey, string& error)
	{
		if (!checkSongExists(albumKey, songKey, error))return -1;
		LibraryFile aFl = LibraryFile();
		Library library = aFl.getLybrary(albumKey, error);
		if (error != "")return -1;
		int adress = library.firstBookAdress;
		while (adress != -1) {
			Book book = readSong(adress);
			if (book.songId == songKey) {
				return adress;
			}
			adress = book.nextAdress;
		}
		return -1;
	}

	Book getBook(int albumKey, int songKey, string& error)
	{
		int adress = getAdressOfSong(albumKey, songKey, error);
		if (error != "")return Book();
		Book book = readSong(adress);
		return book;
	}

	void updateBook(int albumKey, int songKey, Book book, string& error)
	{
		int adress = getAdressOfSong(albumKey, songKey, error);
		if (error != "")return;
		changeSong(book, adress);
		return;
	}

	void addNewSongAdress(int albumKey, int addingAdress, string& error)
	{
		LibraryFile aFl = LibraryFile();
		Library library = aFl.getLybrary(albumKey, error);
		if (error != "")return;
		if (library.firstBookAdress == -1) {
			library.firstBookAdress = addingAdress;
            aFl.updateLibrary(albumKey, library, error);
			return;
		}
		else {
			int adress = library.firstBookAdress;
			Book book = readSong(adress);
			while (book.nextAdress != -1) {
				adress = book.nextAdress;
				book = readSong(adress);
			}
			book.nextAdress = addingAdress;
			changeSong(book, adress);
			return;
		}
	}


	int addBook(int albumKey, Book book, string& error)
	{
		DeletedBooks dSongs = DeletedBooks();
		int adress = dSongs.getFirstDeletedAdress();
		book.songId = ++count;
		if (adress == -1) {
			addNewSongAdress(albumKey, getSeekEnd(), error);
			if (error != "")return 0;
			writeSong(book);
		}
		else {
			addNewSongAdress(albumKey, adress, error);
			if (error != "")return 0;
			changeSong(book, adress);
		}
		return book.songId;
	}

	void deleteBook(int albumKey, int songKey, string& error)
	{
		if (!checkSongExists(albumKey, songKey, error))return;
		LibraryFile aFl = LibraryFile();
		Library library = aFl.getLybrary(albumKey, error);
		DeletedBooks dSongs = DeletedBooks();
		if (error != "")return;
		int adress = library.firstBookAdress;
		vector< pair<int, Book> > books;
		while (adress != -1) {
			Book book = readSong(adress);
			books.push_back({ adress,book });
			adress = book.nextAdress;
		}
		if (books.size() == 1) {
			dSongs.addDeletedAdress(books[0].first);
			books[0].second.isDeleted = 1;
			changeSong(books[0].second, books[0].first);
			library.firstBookAdress = -1;
            aFl.updateLibrary(albumKey, library, error);
			return;
		}
		else {
			if (books[0].second.songId == songKey) {
				dSongs.addDeletedAdress(books[0].first);

				books[0].second.isDeleted = 1;
				changeSong(books[0].second, books[0].first);

				library.firstBookAdress = books[1].first;
                aFl.updateLibrary(albumKey, library, error);
				return;
			}
			else {
				for (int i = 1; i < books.size(); ++i) {
					if (books[i].second.songId == songKey) {
						dSongs.addDeletedAdress(books[i].first);
						if (i == books.size() - 1) {
							books[i - 1].second.nextAdress = -1;
						}
						else {
							books[i - 1].second.nextAdress = books[i + 1].first;
						}
						changeSong(books[i - 1].second, books[i - 1].first);
						books[i].second.isDeleted = 1;
						changeSong(books[i].second, books[i].first);
						return;
					}
				}
			}
		}
	}


	void deleteLibrary(int albumKey, string& error)
	{
		LibraryFile aFl = LibraryFile();
		IndexFile fl = IndexFile();
		DeletedBooks dSongs = DeletedBooks();
		DeletedLibrarys dAl = DeletedLibrarys();
		Library library = aFl.getLybrary(albumKey, error);
		if (error != "")return;

		int deletedAlbumAdress = fl.getAdressByKey(albumKey, error);
		library.isDeleted = 1;
		dAl.addDeletedAdress(deletedAlbumAdress);
        aFl.updateLibrary(albumKey, library, error);
		fl.deleteIndex(albumKey, error);

		int adress = library.firstBookAdress;

		while (adress != -1) {
			Book book = readSong(adress);
			book.isDeleted = 1;
			changeSong(book, adress);
			dSongs.addDeletedAdress(adress);
			adress = book.nextAdress;
		}
	}

	void writeLibraryAndBookInfo(string& error)
	{
		IndexFile fl = IndexFile();
		vector<int>albumKeys = fl.getRealKeys();
		cout << "There are " + to_string(albumKeys.size()) + " libraries in database" << endl;
		LibraryFile aFl = LibraryFile();
		for (int i = 0; i < albumKeys.size(); ++i) {
			int albumKey = albumKeys[i];
			Library library = aFl.getLybrary(albumKey, error);
			cout << '\t' << "The library number " << i + 1 << " with key = " << library.id << ":" << endl;
			cout << '\t' << "Name: " << library.name << endl;
			cout << '\t' << "Address: " << library.date << endl;
			cout << '\t' << "Recordind studio: " << library.publisher << endl;
			int adress = library.firstBookAdress;
			vector<Book>books;
			while (adress != -1) {
				Book book = readSong(adress);
				books.push_back(book);
				adress = book.nextAdress;
			}
			cout << '\t' << '\t' << "Number of books: " << books.size() << endl;
			for (int j = 0; j < books.size(); ++j) {
				Book book = books[j];
				cout << '\t' << '\t' << "The book number " << j + 1 << " with key = " << book.songId << endl;
				cout << '\t' << '\t' << "Name: " << book.name << endl;
				cout << '\t' << '\t' << "Pages: " << book.pages << endl;
				cout << '\t' << '\t' << "Genre: " << book.genre << endl;
				cout << "----------------------------------------------------------" << endl;
			}
			cout<<"<<<<<------------------------------------------------>>>>>" << endl;
		}
	}
};

