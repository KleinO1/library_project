#include "Library.h"
#include <iostream>
#include <stdexcept>

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

    void Library::borrowBook(const std::string& userName, const std::string& isbn) {
        Book* book = findBookByISBN(isbn);
        if (book == nullptr) {
            throw std::runtime_error("Книга с таким ISBN не найдена");
        }

        User* user = findUserByName(userName);
        if (user == nullptr) {
            throw std::runtime_error("Пользователь с таким именем не найден");
        }

        if (!user->canBorrowMore()) {
            throw std::runtime_error("Пользователь достиг лимита книг");
        }
        book->borrowBook(userName);
        user->addBook(isbn);
    }

    void Library::returnBook(const std::string& isbn) {
        Book* book = findBookByISBN(isbn);
        if (book == nullptr) {
            throw std::runtime_error("Книга с таким ISBN не найдена");
        }

        if (book->getIsAvailable()) {
            throw std::runtime_error("Книга уже находится в библиотеке");
        }

        std::string userName = book->getBorrowedBy();
        if (userName.empty()) {
            throw std::runtime_error("Не удалось определить, кто взял книгу");
        }

        User* user = findUserByName(userName);
        if (user == nullptr) {
            throw std::runtime_error("Пользователь, на которого числится книга, не найден");
        }
        user->removeBook(isbn);
        book->returnBook();
    }
}
