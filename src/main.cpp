#include <iostream>
#include <exception>
#include "Library.h"

int main() {
    Library lib("data/library_data.txt");

    Book book("Книга", "Автор", 1451, "ISBN-TEST");
    User user("Иванов", "USR_001", 7);

    lib.addBook(book);
    lib.addUser(user);

    std::cout << "тест\n";
    lib.displayAllBooks();
    lib.displayAllUsers();

    std::cout << "\nтест\n";

    try {
        book.borrowBook(user.getName());
        user.addBook(book.getIsbn());

        std::cout << "\nПосле выдачи:\n";
        book.displayInfo();
        user.displayProfile();

        book.returnBook();
        user.removeBook(book.getIsbn());

        std::cout << "\nПосле возврата:\n";
        book.displayInfo();
        user.displayProfile();
    }
    catch (const std::exception& ex) {
        std::cout << "Возникла ошибка: " << ex.what() << std::endl;
    }
    Book* found = lib.findBookByISBN("ISBN-TEST");
    if (found != nullptr) {
        std::cout << "\nНайдена книга по ISBN: " << found->getTitle() << "\n";
    }

    return 0;
}
