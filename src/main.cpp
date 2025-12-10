#include "Library.h"

int main() {
    Library lib("data/library_data.txt");

    Book book("Пример книги", "Автор", 2000, "ISBN-TEST");
    User user("Иванов", "USR_001", 3);

    lib.addBook(book);
    lib.addUser(user);

    lib.displayAllBooks();
    lib.displayAllUsers();

    return 0;
}
