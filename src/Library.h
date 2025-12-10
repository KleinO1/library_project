#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <string>
#include "Book.h"
#include "User.h"

class Library {
private:
    std::vector<Book> books;
    std::vector<User> users;
    std::string dataFile;

public:
    Library();
    explicit Library(const std::string& fileName);

    void addBook(const Book& book);
    void addUser(const User& user);

    void displayAllBooks() const;
    void displayAllUsers() const;
};

#endif