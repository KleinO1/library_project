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

    try {
        std::cout << "\nПосле выдачи:\n";
        lib.borrowBook("Иванов", "ISBN-TEST");
        lib.displayAllBooks();
        lib.displayAllUsers();

        std::cout << "\nПосле возврата:\n";
        lib.returnBook("ISBN-TEST");
        lib.displayAllBooks();
        lib.displayAllUsers();
    }
    catch (const std::exception& ex) {
        std::cout << "Ошибка: " << ex.what() << std::endl;
    }

    return 0;
}
