#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

// ==========================================
// UTILITY FUNCTIONS
// ==========================================
// Helper function to convert a string to lowercase for case-insensitive search
string toLowerCase(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// ==========================================
// CLASS DEFINITIONS
// ==========================================
class Member {
private:
    int memberId;
    string name;

public:
    Member(int id, string n) : memberId(id), name(n) {}

    int getId() const { return memberId; }
    string getName() const { return name; }
};

class Book {
private:
    int bookId;
    string title;
    string author;
    bool isAvailable;
    int issuedToMemberId; // -1 if not issued

public:
    Book(int id, string t, string a) : bookId(id), title(t), author(a), isAvailable(true), issuedToMemberId(-1) {}

    int getId() const { return bookId; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool getAvailability() const { return isAvailable; }
    int getIssuedMemberId() const { return issuedToMemberId; }

    void issueBook(int memberId) {
        isAvailable = false;
        issuedToMemberId = memberId;
    }

    void returnBook() {
        isAvailable = true;
        issuedToMemberId = -1;
    }

    void displayBook() const {
        cout << left << setw(6) << bookId 
             << setw(30) << title 
             << setw(20) << author 
             << setw(15) << (isAvailable ? "Available" : "Issued");
        
        if (!isAvailable) {
            cout << "(Member ID: " << issuedToMemberId << ")";
        }
        cout << "\n";
    }
};

class Library {
private:
    vector<Book> books;
    vector<Member> members;

    // Helper to find a member index
    int findMemberIndex(int memberId) {
        for (size_t i = 0; i < members.size(); i++) {
            if (members[i].getId() == memberId) return i;
        }
        return -1;
    }

    // Helper to find a book index
    int findBookIndex(int bookId) {
        for (size_t i = 0; i < books.size(); i++) {
            if (books[i].getId() == bookId) return i;
        }
        return -1;
    }

public:
    // --- Management Operations ---
    void addBook(int id, string title, string author) {
        if (findBookIndex(id) != -1) {
            cout << "Error: A book with ID " << id << " already exists.\n";
            return;
        }
        books.push_back(Book(id, title, author));
        cout << "Success: Book '" << title << "' added to the library.\n";
    }

    void addMember(int id, string name) {
        if (findMemberIndex(id) != -1) {
            cout << "Error: A member with ID " << id << " already exists.\n";
            return;
        }
        members.push_back(Member(id, name));
        cout << "Success: Member '" << name << "' registered successfully.\n";
    }

    // --- Core Features ---
    void issueBook(int bookId, int memberId) {
        int bIndex = findBookIndex(bookId);
        int mIndex = findMemberIndex(memberId);

        if (bIndex == -1) {
            cout << "Error: Book ID not found.\n";
            return;
        }
        if (mIndex == -1) {
            cout << "Error: Member ID not found.\n";
            return;
        }

        if (books[bIndex].getAvailability()) {
            books[bIndex].issueBook(memberId);
            cout << "Success: '" << books[bIndex].getTitle() << "' issued to " << members[mIndex].getName() << ".\n";
        } else {
            cout << "Error: Book is currently checked out.\n";
        }
    }

    void returnBook(int bookId) {
        int bIndex = findBookIndex(bookId);

        if (bIndex == -1) {
            cout << "Error: Book ID not found.\n";
            return;
        }

        if (!books[bIndex].getAvailability()) {
            books[bIndex].returnBook();
            cout << "Success: '" << books[bIndex].getTitle() << "' has been returned.\n";
        } else {
            cout << "Notice: This book was not issued.\n";
        }
    }

    void searchBooks(string query) const {
        string lowerQuery = toLowerCase(query);
        bool found = false;

        cout << "\n--- Search Results ---\n";
        cout << left << setw(6) << "ID" << setw(30) << "Title" << setw(20) << "Author" << setw(15) << "Status" << "\n";
        cout << string(70, '-') << "\n";

        for (const auto& book : books) {
            if (toLowerCase(book.getTitle()).find(lowerQuery) != string::npos || 
                toLowerCase(book.getAuthor()).find(lowerQuery) != string::npos) {
                book.displayBook();
                found = true;
            }
        }

        if (!found) {
            cout << "No books found matching '" << query << "'.\n";
        }
    }

    void displayAllBooks() const {
        if (books.empty()) {
            cout << "The library has no books currently.\n";
            return;
        }
        cout << "\n--- Library Catalog ---\n";
        cout << left << setw(6) << "ID" << setw(30) << "Title" << setw(20) << "Author" << setw(15) << "Status" << "\n";
        cout << string(70, '-') << "\n";
        for (const auto& book : books) {
            book.displayBook();
        }
    }
};

// ==========================================
// MAIN APPLICATION LOOP
// ==========================================
int main() {
    Library lib;
    int choice;

    // Pre-populating with some sample data for testing
    lib.addBook(101, "The Great Gatsby", "F. Scott Fitzgerald");
    lib.addBook(102, "1984", "George Orwell");
    lib.addBook(103, "Clean Code", "Robert C. Martin");
    lib.addMember(1, "Alice Smith");
    lib.addMember(2, "Bob Johnson");

    do {
        cout << "\n====================================\n";
        cout << "     LIBRARY MANAGEMENT SYSTEM\n";
        cout << "====================================\n";
        cout << "1. Add a New Book\n";
        cout << "2. Register a New Member\n";
        cout << "3. Issue a Book\n";
        cout << "4. Return a Book\n";
        cout << "5. Search Books (Title/Author)\n";
        cout << "6. Display All Books\n";
        cout << "7. Exit\n";
        cout << "====================================\n";
        cout << "Enter your choice (1-7): ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            choice = 0;
        }

        switch (choice) {
            case 1: {
                int id;
                string title, author;
                cout << "Enter Book ID (integer): ";
                cin >> id;
                cin.ignore();
                cout << "Enter Book Title: ";
                getline(cin, title);
                cout << "Enter Author Name: ";
                getline(cin, author);
                lib.addBook(id, title, author);
                break;
            }
            case 2: {
                int id;
                string name;
                cout << "Enter Member ID (integer): ";
                cin >> id;
                cin.ignore();
                cout << "Enter Member Name: ";
                getline(cin, name);
                lib.addMember(id, name);
                break;
            }
            case 3: {
                int bookId, memberId;
                cout << "Enter Book ID to issue: ";
                cin >> bookId;
                cout << "Enter Member ID receiving the book: ";
                cin >> memberId;
                lib.issueBook(bookId, memberId);
                break;
            }
            case 4: {
                int bookId;
                cout << "Enter Book ID to return: ";
                cin >> bookId;
                lib.returnBook(bookId);
                break;
            }
            case 5: {
                string query;
                cin.ignore();
                cout << "Enter Search Query (Title or Author): ";
                getline(cin, query);
                lib.searchBooks(query);
                break;
            }
            case 6: {
                lib.displayAllBooks();
                break;
            }
            case 7: {
                cout << "\nExiting Library System. Goodbye!\n";
                break;
            }
            default: {
                cout << "\nInvalid choice. Please enter a number between 1 and 7.\n";
                break;
            }
        }
    } while (choice != 7);

    return 0;
}