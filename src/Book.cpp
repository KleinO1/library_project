#include "Book.h"
#include <iostream>

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
    if (userName.empty()) {
        return;
    }
    isAvailable = false;
    borrowedBy = userName;
}

void Book::returnBook() {
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
