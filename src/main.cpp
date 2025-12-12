#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include "Library.h"

static int readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string s;
        std::getline(std::cin, s);

        try {
            size_t pos = 0;
            int v = std::stoi(s, &pos);
            while (pos < s.size() && (s[pos] == ' ' || s[pos] == '\t')) pos++;
            if (pos != s.size()) {
                throw std::invalid_argument("лишние символы");
            }
            return v;
        } catch (...) {
            std::cout << "Некорректный ввод. Введите число." << std::endl;
        }
    }
}

static std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

static void printMenu() {
    std::cout << "=== БИБЛИОТЕКА ===" << std::endl;
    std::cout << "1. Просмотреть все книги" << std::endl;
    std::cout << "2. Просмотреть всех пользователей" << std::endl;
    std::cout << "3. Добавить новую книгу" << std::endl;
    std::cout << "4. Зарегистрировать пользователя" << std::endl;
    std::cout << "5. Выдать книгу пользователю" << std::endl;
    std::cout << "6. Принять книгу от пользователя" << std::endl;
    std::cout << "7. Поиск книги по ISBN" << std::endl;
    std::cout << "8. Просмотреть профиль пользователя" << std::endl;
    std::cout << "9. Сохранить данные в файл" << std::endl;
    std::cout << "10. Выход" << std::endl;
}

int main() {
    Library lib("data/library_data.txt");

    while (true) {
        printMenu();
        int choice = readInt("Ваш выбор: ");

        try {
            if (choice == 1) {
                lib.displayAllBooks();
                std::cout << "Готово." << std::endl;
            } else if (choice == 2) {
                lib.displayAllUsers();
                std::cout << "Готово." << std::endl;
            } else if (choice == 3) {
                std::string title = readLine("Введите название: ");
                std::string author = readLine("Введите автора: ");
                int year = readInt("Введите год издания: ");
                std::string isbn = readLine("Введите ISBN: ");

                Book b(title, author, year, isbn);
                lib.addBook(b);

                std::cout << "Книга добавлена." << std::endl;
            } else if (choice == 4) {
                std::string name = readLine("Введите фамилию пользователя: ");
                std::string userId = readLine("Введите ID (например USR_001): ");
                std::string maxStr = readLine("Максимум книг (Enter = 3): ");

                int maxBooks = 3;
                if (!maxStr.empty()) {
                    maxBooks = std::stoi(maxStr);
                }

                User u(name, userId, maxBooks);
                lib.addUser(u);

                std::cout << "Пользователь зарегистрирован." << std::endl;
            } else if (choice == 5) {
                std::string userName = readLine("Фамилия пользователя: ");
                std::string isbn = readLine("ISBN книги: ");

                lib.borrowBook(userName, isbn);

                std::cout << "Книга выдана." << std::endl;
            } else if (choice == 6) {
                std::string isbn = readLine("ISBN книги: ");

                lib.returnBook(isbn);

                std::cout << "Книга принята." << std::endl;
            } else if (choice == 7) {
                std::string isbn = readLine("Введите ISBN для поиска: ");

                Book* b = lib.findBookByISBN(isbn);
                if (b == nullptr) {
                    std::cout << "Книга не найдена." << std::endl;
                } else {
                    b->displayInfo();
                }
            } else if (choice == 8) {
                std::string name = readLine("Введите фамилию пользователя: ");

                User* u = lib.findUserByName(name);
                if (u == nullptr) {
                    std::cout << "Пользователь не найден." << std::endl;
                } else {
                    u->displayProfile();
                }
            } else if (choice == 9) {
                lib.saveToFile();
                std::cout << "Данные сохранены." << std::endl;
            } else if (choice == 10) {
                std::cout << "Выход." << std::endl;
                break;
            } else {
                std::cout << "Некорректный пункт меню." << std::endl;
            }
        } catch (const std::exception& ex) {
            std::cout << "Ошибка: " << ex.what() << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}
