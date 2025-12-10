#include "Book.h"
#include <iostream>
#include <stdexcept>
#include <ctime>

Book::Book()
    : title(""),
      author(""),
      year(0),
      isbn(""),
      isAvailable(true),
      borrowedBy("")
{
}

Book::Book(const std::string& titleValue,
           const std::string& authorValue,
           int yearValue,
           const std::string& isbnValue)
{
    title = titleValue;
    author = authorValue;
    year = yearValue;
    isbn = isbnValue;
    isAvailable = true;
    borrowedBy.clear();

    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    int currentYear = 0;
    if (localTime != nullptr) {
        currentYear = 1900 + localTime->tm_year;
    } else {
        currentYear = 2025;
    }

    if (year < 1450 || year > currentYear) {
        throw std::invalid_argument("Некорректный год издания книги");
    }

    if (isbn.empty()) {
        throw std::invalid_argument("ISBN не может быть пустым");
    }
}


std::string Book::getTitle() const {
    return title;
}

std::string Book::getAuthor() const {
    return author;
}

int Book::getYear() const {
    return year;
}

std::string Book::getIsbn() const {
    return isbn;
}

bool Book::getIsAvailable() const {
    return isAvailable;
}

std::string Book::getBorrowedBy() const {
    return borrowedBy;
}

void Book::borrowBook(const std::string& userName) {
    if (!isAvailable) {
        throw std::runtime_error("Книга уже выдана");
    }

    if (userName.empty()) {
        throw std::invalid_argument("Имя пользователя не может быть пустым");
    }

    isAvailable = false;
    borrowedBy = userName;
}

void Book::returnBook() {
    if (isAvailable) {
        throw std::runtime_error("Книга уже находится в библиотеке");
    }

    isAvailable = true;
    borrowedBy.clear();
}

void Book::displayInfo() const {
    std::cout << "Название: " << title << "\n";
    std::cout << "Автор: " << author << "\n";
    std::cout << "Год: " << year << "\n";
    std::cout << "ISBN: " << isbn << "\n";
    std::cout << "Доступна: " << (isAvailable ? "да" : "нет") << "\n";
    if (!isAvailable) {
        std::cout << "Выдана пользователю: " << borrowedBy << "\n";
    }
    std::cout << "------------------------" << std::endl;
}
