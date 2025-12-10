#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book {
private:
    std::string title;
    std::string author;
    int year;
    std::string isbn;
    bool isAvailable;
    std::string borrowedBy;

public:
    Book();
    Book(const std::string& title,
         const std::string& author,
         int year,
         const std::string& isbn);

    std::string getTitle() const;
    std::string getAuthor() const;
    int getYear() const;
    std::string getIsbn() const;
    bool getIsAvailable() const;
    std::string getBorrowedBy() const;

    void borrowBook(const std::string& userName);
    void returnBook();

    void displayInfo() const;
};

#endif