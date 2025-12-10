#include "User.h"
#include <iostream>

User::User()
    : name(""),
      userId(""),
      maxBooksAllowed(3)
{
}

User::User(const std::string& nameValue,
           const std::string& idValue,
           int maxBooks)
    : name(nameValue),
      userId(idValue),
      maxBooksAllowed(maxBooks)
{
    if (maxBooksAllowed <= 0) {
        maxBooksAllowed = 1;
    }
}

std::string User::getName() const {
    return name;
}

std::string User::getUserId() const {
    return userId;
}

int User::getMaxBooksAllowed() const {
    return maxBooksAllowed;
}

const std::vector<std::string>& User::getBorrowedBooks() const {
    return borrowedBooks;
}

bool User::canBorrowMore() const {
    return static_cast<int>(borrowedBooks.size()) < maxBooksAllowed;
}

void User::addBook(const std::string& isbn) {
    borrowedBooks.push_back(isbn);
}

void User::removeBook(const std::string& isbn) {
    for (std::size_t i = 0; i < borrowedBooks.size(); ++i) {
        if (borrowedBooks[i] == isbn) {
            borrowedBooks.erase(borrowedBooks.begin() + static_cast<long>(i));
            break;
        }
    }
}

void User::displayProfile() const {
    std::cout << "Пользователь: " << name << " (" << userId << ")\n";
    std::cout << "Максимум книг: " << maxBooksAllowed << "\n";
    std::cout << "Взятые книги: ";
    if (borrowedBooks.empty()) {
        std::cout << "нет";
    } else {
        for (const auto& code : borrowedBooks) {
            std::cout << code << " ";
        }
    }
    std::cout << std::endl;
}
