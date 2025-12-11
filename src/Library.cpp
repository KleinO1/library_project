#include "Library.h"
#include <iostream>

Library::Library()
    : dataFile("")
{
}

Library::Library(const std::string& fileName)
    : dataFile(fileName)
{
}

void Library::addBook(const Book& book) {
    books.push_back(book);
}

void Library::addUser(const User& user) {
    users.push_back(user);
}

void Library::displayAllBooks() const {
    if (books.empty()) {
        std::cout << "Книг пока нет\n";
    } else {
        for (const auto& b : books) {
            b.displayInfo();
        }
    }
}

void Library::displayAllUsers() const {
    if (users.empty()) {
        std::cout << "Пользователей пока нет\n";
    } else {
        for (const auto& u : users) {
            u.displayProfile();
        }
    }
    Book* Library::findBookByISBN(const std::string& isbn) {
        for (auto& b : books) {
            if (b.getIsbn() == isbn) {
                return &b;
            }
        }
        return nullptr;
    }

    User* Library::findUserByName(const std::string& name) {
        for (auto& u : users) {
            if (u.getName() == name) {
                return &u;
            }
        }
        return nullptr;
    }
}
