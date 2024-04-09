cmake_minimum_required(VERSION 3.23)
project(DataBaseLab1)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(DataBaseLab1
        Library.cpp
        Library.h
        LibraryFile.cpp
        LibraryFile.h
        DataBaseLab1.cpp
        DeletedLibrarys.cpp
        DeletedLibrarys.h
        DeletedBooks.cpp
        DeletedBooks.h
        Index.cpp
        Index.h
        indexFile.cpp
        indexFile.h
        ShortString.cpp
        ShortString.h
        Book.cpp
        Book.h
        BooksFIle.cpp
        BooksFIle.h)
