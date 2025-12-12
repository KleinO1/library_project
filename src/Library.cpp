#include "Library.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>

Library::Library()
    : dataFile("data/library_data.txt")
{
    loadFromFile();
}

Library::Library(const std::string& fileName)
    : dataFile(fileName)
{
    loadFromFile();
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

void Library::loadFromFile() {
    books.clear();
    users.clear();

    if (dataFile.empty()) {
        return;
    }

    std::ifstream in(dataFile);
    if (!in.is_open()) {
        std::ofstream out(dataFile);
        if (out.is_open()) {
            out << "---USERS---\n";
        }
        return;
    }

    std::string line;
    bool inUsersSection = false;
    std::string title;
    std::string author;
    int year = 0;
    std::string isbn;
    bool available = true;
    std::string borrowedBy;
    bool hasBookData = false;
    std::string userName;
    std::string userId;
    std::vector<std::string> borrowedList;
    int maxBooks = 3;
    bool hasUserData = false;

    while (std::getline(in, line)) {
        if (line == "---USERS---") {
            if (hasBookData) {
                try {
                    Book book(title, author, year, isbn);
                    if (!available && !borrowedBy.empty()) {
                        book.borrowBook(borrowedBy);
                    }
                    books.push_back(book);
                } catch (...) {
                }
                hasBookData = false;
            }
            inUsersSection = true;
            continue;
        }

        if (!inUsersSection) {
            if (line == "BOOK") {
                if (hasBookData) {
                    try {
                        Book book(title, author, year, isbn);
                        if (!available && !borrowedBy.empty()) {
                            book.borrowBook(borrowedBy);
                        }
                        books.push_back(book);
                    } catch (...) {
                    }
                }
                title.clear();
                author.clear();
                year = 0;
                isbn.clear();
                available = true;
                borrowedBy.clear();
                hasBookData = true;
            } else if (line.rfind("Title:", 0) == 0) {
                std::string value = line.substr(6);
                if (!value.empty() && value[0] == ' ') {
                    value.erase(0, 1);
                }
                title = value;
            } else if (line.rfind("Author:", 0) == 0) {
                std::string value = line.substr(7);
                if (!value.empty() && value[0] == ' ') {
                    value.erase(0, 1);
                }
                author = value;
            } else if (line.rfind("Year:", 0) == 0) {
                std::string value = line.substr(5);
                if (!value.empty() && value[0] == ' ') {
                    value.erase(0, 1);
                }
                year = std::stoi(value);
            } else if (line.rfind("ISBN:", 0) == 0) {
                std::string value = line.substr(5);
                if (!value.empty() && value[0] == ' ') {
                    value.erase(0, 1);
                }
                isbn = value;
            } else if (line.rfind("Available:", 0) == 0) {
                std::string value = line.substr(10);
                if (!value.empty() && value[0] == ' ') {
                    value.erase(0, 1);
                }
                available = (value == "yes");
            } else if (line.rfind("BorrowedBy:", 0) == 0) {
                std::string value = line.substr(11);
                if (!value.empty() && value[0] == ' ') {
                    value.erase(0, 1);
                }
                borrowedBy = value;
            }
        } else {
            if (line == "USER") {
                if (hasUserData) {
                    try {
                        User user(userName, userId, maxBooks);
                        for (const auto& code : borrowedList) {
                            if (!code.empty()) {
                                user.addBook(code);
                            }
                        }
                        users.push_back(user);
                    } catch (...) {
                    }
                }

                userName.clear();
                userId.clear();
                borrowedList.clear();
                maxBooks = 3;
                hasUserData = true;
            } else if (line.rfind("Name:", 0) == 0) {
                std::string value = line.substr(5);
                if (!value.empty() && value[0] == ' ') {
                    value.erase(0, 1);
                }
                userName = value;
            } else if (line.rfind("UserID:", 0) == 0) {
                std::string value = line.substr(7);
                if (!value.empty() && value[0] == ' ') {
                    value.erase(0, 1);
                }
                userId = value;
            } else if (line.rfind("BorrowedBooks:", 0) == 0) {
                std::string value = line.substr(14);
                if (!value.empty() && value[0] == ' ') {
                    value.erase(0, 1);
                }

                borrowedList.clear();
                if (!value.empty()) {
                    std::stringstream ss(value);
                    std::string part;
                    while (std::getline(ss, part, '|')) {
                        if (!part.empty()) {
                            borrowedList.push_back(part);
                        }
                    }
                }
            } else if (line.rfind("MaxBooks:", 0) == 0) {
                std::string value = line.substr(9);
                if (!value.empty() && value[0] == ' ') {
                    value.erase(0, 1);
                }
                maxBooks = std::stoi(value);
            }
        }
    }
    if (hasBookData) {
        try {
            Book book(title, author, year, isbn);
            if (!available && !borrowedBy.empty()) {
                book.borrowBook(borrowedBy);
            }
            books.push_back(book);
        } catch (...) {
        }
    }

    if (hasUserData) {
        try {
            User user(userName, userId, maxBooks);
            for (const auto& code : borrowedList) {
                if (!code.empty()) {
                    user.addBook(code);
                }
            }
            users.push_back(user);
        } catch (...) {
        }
    }
}

    void Library::saveToFile() {
        if (dataFile.empty()) {
            return;
        }

        std::ofstream out(dataFile);
        if (!out.is_open()) {
            std::cout << "Не удалось открыть файл для записи: " << dataFile << std::endl;
            return;
        }
        for (const auto& b : books) {
            out << "BOOK\n";
            out << "Title: " << b.getTitle() << "\n";
            out << "Author: " << b.getAuthor() << "\n";
            out << "Year: " << b.getYear() << "\n";
            out << "ISBN: " << b.getIsbn() << "\n";
            out << "Available: " << (b.getIsAvailable() ? "yes" : "no") << "\n";
            out << "BorrowedBy: " << b.getBorrowedBy() << "\n\n";
        }

        out << "---USERS---\n\n";
        for (const auto& u : users) {
            out << "USER\n";
            out << "Name: " << u.getName() << "\n";
            out << "UserID: " << u.getUserId() << "\n";

            out << "BorrowedBooks: ";
            const auto& list = u.getBorrowedBooks();
            for (std::size_t i = 0; i < list.size(); ++i) {
                out << list[i];
                if (i + 1 < list.size()) {
                    out << "|";
                }
            }
            out << "\n";

            out << "MaxBooks: " << u.getMaxBooksAllowed() << "\n\n";
        }
    }

}
