#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

class User {
private:
    std::string name;
    std::string userId;
    std::vector<std::string> borrowedBooks;
    int maxBooksAllowed;

public:
    User();
    User(const std::string& name,
         const std::string& id,
         int maxBooks = 3);

    std::string getName() const;
    std::string getUserId() const;
    int getMaxBooksAllowed() const;
    const std::vector<std::string>& getBorrowedBooks() const;

    bool canBorrowMore() const;
    void addBook(const std::string& isbn);
    void removeBook(const std::string& isbn);

    void displayProfile() const;
};

#endif
