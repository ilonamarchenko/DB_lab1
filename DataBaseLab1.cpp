#include <iostream>
#include <limits>
#include "BooksFIle.h"

BookFile sFl = BookFile();
LibraryFile aFl = LibraryFile();
IndexFile iFl = IndexFile();
DeletedLibrarys dAl = DeletedLibrarys();
DeletedBooks dBook = DeletedBooks();

bool intValidate()
{
	if (cin.fail())
	{
		cout << "Input is incorrect" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return false;
	}
	else return true;
}

bool inputInt(int& x)
{
	cin >> x;
	return intValidate();
}

Library inputLibrary()
{
	Library library;
	cout << "Input name of library: ";
	cin >> library.name;
	cout << "Input address of library: ";
	cin >> library.date;
	cout << "Input publisher of library: ";
	cin >> library.publisher;
	return library;
}

Book inputBooks(string& error)
{
	Book book;
	cout << "Input name of book: ";
	cin >> book.name;
	cout << "Input pages of book: ";
	if (!inputInt(book.pages)) {
		error = "Input is incorrect";
		return {};
	}
	cout << "Input genre of book: ";
	cin >> book.genre;
	return book;
}

void addLibrary()
{
	string error;
	Library library = inputLibrary();
	library.firstBookAdress = -1;
	library.isDeleted = false;
	int key = aFl.addLibrary(library, error);
	if (error.empty()) {
		cout << "Library successfully added with key = " << key << endl;
	}
	else {
		cout << error << endl;
	}
}



void addBook()
{
	string error;
	int libraryId;
	cout << "Input key of library: ";
	if (!inputInt(libraryId))return;
	Book book = inputBooks(error);
	if (!error.empty()) {
		cout << error << endl;
		return;
	}
	book.libraryId = libraryId;
	book.isDeleted = false;
	book.nextAdress = -1;
	int bookKey = sFl.addBook(libraryId, book, error);
	if (error.empty()) {
		cout << "Book successfully added with key = " << bookKey << endl;
	}
	else {
		cout << error << endl;
	}
}

void updateLibrary()
{
	string error;
	int libraryId;
	cout << "Input key of library: ";
	if (!inputInt(libraryId))return;
	Library library = inputLibrary();
	Library oldLibrary = aFl.getLybrary(libraryId, error);
	if (!error.empty()) {
		cout << error << endl;
		return;
	}

    oldLibrary.name = library.name;
    oldLibrary.date = library.date;
    oldLibrary.publisher = library.publisher;
    aFl.updateLibrary(libraryId, oldLibrary, error);

	if (error.empty())
	{
		cout << "Library successfully updated" << endl;
	}
	else {
		cout << error << endl;
	}
}

void updateBook()
{
	string error;
	int libraryId, bookId;
	cout << "Input key of library: ";
	if (!inputInt(libraryId))return;
	cout << "Input key of book: ";
	if (!inputInt(bookId))return;
	Book book = inputBooks(error);
	if (!error.empty()) {
		cout << error << endl;
		return;
	}
	Book oldBook = sFl.getBook(libraryId, bookId, error);
	if (!error.empty()) {
		cout << error << endl;
		return;
	}
    oldBook.genre = book.genre;
    oldBook.name = book.name;
    oldBook.pages = book.pages;

    sFl.updateBook(libraryId, bookId, oldBook, error);
	if (error.empty()) {
		cout << "Book successfully updated" << endl;
	}
	else {
		cout << error << endl;
	}
}

void deleteLibrary()
{
	string error;
	int libraryId;
	cout << "Input key of library: ";
	if (!inputInt(libraryId))return;
    sFl.deleteLibrary(libraryId, error);
	if (error.empty()) {
		cout << "Library successfully deleted" << endl;
	}
	else {
		cout << error << endl;
	}
}

void deleteBook()
{
	string error;
	int libraryId, bookId;
	cout << "Input key of library: ";
	if (!inputInt(libraryId))return;
	cout << "Input key of book: ";
	if (!inputInt(bookId))return;
    sFl.deleteBook(libraryId, bookId, error);
	if (error.empty()) {
		cout << "Book successfully deleted" << endl;
	}
	else {
		cout << error << endl;
	}
}

void getLibrary()
{
	string error;
	int libraryId;
	cout << "Input key of library: ";
	if (!inputInt(libraryId))return;
	Library library = aFl.getLybrary(libraryId, error);
	if (error.empty()) {
		cout << "Name of library: " << library.name << endl;
		cout << "Address of library: " << library.date << endl;
		cout << "Publisher organisation of library: " << library.publisher << endl;
	}
	else {
		cout << error << endl;
	}
}

void getBook()
{
	string error;
	int libraryId, bookId;
	cout << "Input key of library: ";
	if (!inputInt(libraryId))return;
	cout << "Input key of book: ";
	if (!inputInt(bookId))return;
	Book book = sFl.getBook(libraryId, bookId, error);
	if (error.empty()) {
		cout << "Name of book: " << book.name << endl;
		cout << "Pages of book: " << book.pages << endl;
		cout << "Genre of book: " << book.genre << endl;
	}
	else {
		cout << error << endl;
	}
}

void writeDatabase()
{
	string error;
    sFl.writeLibraryAndBookInfo(error);
	if (!error.empty()) {
		cout << error << endl;
	}
}

int main()
{
	iFl.createFile();
	aFl.createFile();
	sFl.createFile();
	dAl.createFile();
	dBook.createFile();
	while (true) {
		cout << "Choose option:" << endl;
		cout << "1. Add library " << endl;
		cout << "2. Delete library" << endl;
		cout << "3. Modify library" << endl;
		cout << "4. Get library" << endl;
		cout << "5. Add book " << endl;
		cout << "6. Delete book" << endl;
		cout << "7. Modify book" << endl;
		cout << "8. Get book" << endl;
		cout << "9. Get all database" << endl;
		int x;
		cin >> x;
		if (!intValidate())continue;
		if (x < 1 || x>9) {
			cout << "Option is incorrect" << endl;
		}
		else {
			if (x == 1) {
                addLibrary();
			}
			else if (x == 2) {
                deleteLibrary();
			}
			else if (x == 3) {
                updateLibrary();
			}
			else if (x == 4) {
                getLibrary();
			}
			else if (x == 5) {
                addBook();
			}
			else if (x == 6) {
                deleteBook();
			}
			else if (x == 7) {
                updateBook();
			}
			else if (x == 8) {
                getBook();
			}
			else {
				writeDatabase();
			}
		}
	}
}


