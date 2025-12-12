#include "Library.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <algorithm>

static std::string trimOneLeftSpace(std::string s) {
    if (!s.empty() && s[0] == ' ') {
        s.erase(0, 1);
    }
    return s;
}

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

void Library::addBook(const Book& book) {
    if (book.getIsbn().empty()) {
        throw std::invalid_argument("ISBN не может быть пустым");
    }
    if (findBookByISBN(book.getIsbn()) != nullptr) {
        throw std::invalid_argument("ISBN должен быть уникальным: такая книга уже существует");
    }
    books.push_back(book);
}

void Library::addUser(const User& user) {
    if (user.getName().empty()) {
        throw std::invalid_argument("Фамилия пользователя не может быть пустой");
    }
    if (findUserByName(user.getName()) != nullptr) {
        throw std::invalid_argument("Пользователь с такой фамилией уже зарегистрирован");
    }
    users.push_back(user);
}

void Library::borrowBook(const std::string& userName, const std::string& isbn) {
    Book* book = findBookByISBN(isbn);
    if (book == nullptr) {
        throw std::runtime_error("Книга с таким ISBN не найдена");
    }

    User* user = findUserByName(userName);
    if (user == nullptr) {
        throw std::runtime_error("Пользователь с такой фамилией не найден");
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

    std::string who = book->getBorrowedBy();
    if (who.empty()) {
        throw std::runtime_error("Не удалось определить пользователя, у которого книга");
    }

    User* user = findUserByName(who);
    if (user == nullptr) {
        throw std::runtime_error("Пользователь, на которого числится книга, не найден");
    }

    user->removeBook(isbn);
    book->returnBook();
}

void Library::displayAllBooks() const {
    if (books.empty()) {
        std::cout << "Книг пока нет" << std::endl;
        return;
    }

    std::vector<const Book*> order;
    order.reserve(books.size());
    for (const auto& b : books) {
        order.push_back(&b);
    }

    std::sort(order.begin(), order.end(), [](const Book* a, const Book* b) {
        if (a->getYear() != b->getYear()) return a->getYear() < b->getYear();
        return a->getTitle() < b->getTitle();
    });

    for (const auto* ptr : order) {
        ptr->displayInfo();
    }
}

void Library::displayAllUsers() const {
    if (users.empty()) {
        std::cout << "Пользователей пока нет" << std::endl;
        return;
    }
    for (const auto& u : users) {
        u.displayProfile();
    }
}

std::vector<Book*> Library::findBooksByAuthor(const std::string& author) {
    std::vector<Book*> res;
    for (auto& b : books) {
        if (b.getAuthor() == author) {
            res.push_back(&b);
        }
    }
    return res;
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
    bool inUsers = false;

    std::string title;
    std::string author;
    int year = 0;
    std::string isbn;
    bool available = true;
    std::string borrowedBy;
    bool hasBook = false;

    std::string userName;
    std::string userId;
    std::vector<std::string> borrowedList;
    int maxBooks = 3;
    bool hasUser = false;

    auto flushBook = [&]() {
        if (!hasBook) return;
        try {
            if (!isbn.empty()) {
                Book b(title, author, year, isbn);
                if (!available && !borrowedBy.empty()) {
                    b.borrowBook(borrowedBy);
                }
                if (findBookByISBN(b.getIsbn()) == nullptr) {
                    books.push_back(b);
                }
            }
        } catch (...) {
        }
        hasBook = false;
    };

    auto flushUser = [&]() {
        if (!hasUser) return;
        try {
            if (!userName.empty() && !userId.empty()) {
                User u(userName, userId, maxBooks);
                for (const auto& code : borrowedList) {
                    if (!code.empty()) {
                        try {
                            u.addBook(code);
                        } catch (...) {
                        }
                    }
                }
                users.push_back(u);
            }
        } catch (...) {
        }
        hasUser = false;
    };

    while (std::getline(in, line)) {
        if (line == "---USERS---") {
            flushBook();
            inUsers = true;
            continue;
        }

        if (!inUsers) {
            if (line == "BOOK") {
                flushBook();
                title.clear();
                author.clear();
                year = 0;
                isbn.clear();
                available = true;
                borrowedBy.clear();
                hasBook = true;
                continue;
            }

            if (line.rfind("Title:", 0) == 0) {
                title = trimOneLeftSpace(line.substr(6));
            } else if (line.rfind("Author:", 0) == 0) {
                author = trimOneLeftSpace(line.substr(7));
            } else if (line.rfind("Year:", 0) == 0) {
                std::string v = trimOneLeftSpace(line.substr(5));
                try { year = std::stoi(v); } catch (...) { year = 0; }
            } else if (line.rfind("ISBN:", 0) == 0) {
                isbn = trimOneLeftSpace(line.substr(5));
            } else if (line.rfind("Available:", 0) == 0) {
                std::string v = trimOneLeftSpace(line.substr(10));
                available = (v == "yes");
            } else if (line.rfind("BorrowedBy:", 0) == 0) {
                borrowedBy = trimOneLeftSpace(line.substr(11));
            }
        } else {
            if (line == "USER") {
                flushUser();
                userName.clear();
                userId.clear();
                borrowedList.clear();
                maxBooks = 3;
                hasUser = true;
                continue;
            }

            if (line.rfind("Name:", 0) == 0) {
                userName = trimOneLeftSpace(line.substr(5));
            } else if (line.rfind("UserID:", 0) == 0) {
                userId = trimOneLeftSpace(line.substr(7));
            } else if (line.rfind("BorrowedBooks:", 0) == 0) {
                std::string v = trimOneLeftSpace(line.substr(14));
                borrowedList.clear();
                if (!v.empty()) {
                    std::stringstream ss(v);
                    std::string part;
                    while (std::getline(ss, part, '|')) {
                        if (!part.empty()) {
                            borrowedList.push_back(part);
                        }
                    }
                }
            } else if (line.rfind("MaxBooks:", 0) == 0) {
                std::string v = trimOneLeftSpace(line.substr(9));
                try { maxBooks = std::stoi(v); } catch (...) { maxBooks = 3; }
            }
        }
    }

    flushBook();
    flushUser();
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
        out << "BorrowedBy: " << b.getBorrowedBy() << "\n";
        out << "\n";
    }

    out << "---USERS---\n";
    out << "\n";

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

        out << "MaxBooks: " << u.getMaxBooksAllowed() << "\n";
        out << "\n";
    }
}
