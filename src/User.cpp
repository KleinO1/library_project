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
    if (name.empty()) {
        throw std::invalid_argument("Имя пользователя не может быть пустым");
    }

    if (userId.empty()) {
        throw std::invalid_argument("userId не может быть пустым");
    }

    if (maxBooksAllowed <= 0) {
        throw std::invalid_argument("Максимум книг должен быть больше 0");
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
    if (!canBorrowMore()) {
        throw std::runtime_error("Пользователь достиг лимита книг");
    }

    if (isbn.empty()) {
        throw std::invalid_argument("ISBN не может быть пустым");
    }

    borrowedBooks.push_back(isbn);
}

void User::removeBook(const std::string& isbn) {
    bool found = false;

    for (std::size_t i = 0; i < borrowedBooks.size(); ++i) {
        if (borrowedBooks[i] == isbn) {
            borrowedBooks.erase(borrowedBooks.begin() + static_cast<long>(i));
            found = true;
            break;
        }
    }

    if (!found) {
        throw std::runtime_error("Пользователь не брал книгу с таким ISBN");
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
