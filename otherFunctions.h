#ifndef OTHERFUNCTIONS_H
#define OTHERFUNCTIONS_H

#define MAXTITLELENGTH 100 // Max length a books title can be
#define MAXAUTHORLENGTH 50 // Max length an Authors name can be
#define MAXNAMELENGTH 50 // Max length a name can be
#define MAXEMAILLENGTH 100 // Max length an email can be

typedef struct date { // this might need to go in a separate time.c file so that we can separate out the date handling later.
    int day;
    int month;
    int year;
} Date;

typedef struct person {
    int ID;
    char name[MAXNAMELENGTH];
    char email[MAXEMAILLENGTH];
} Person;

typedef struct books {
    int ID;
    char Title[MAXTITLELENGTH];
    char Author[MAXAUTHORLENGTH];
    Date* issueDate;
    Date* dueDate; 
    Person* borrowedTo; // name of person who has borrowed the book.   
} Book;

typedef struct list {
    Book* book;
    struct list* next;
} List;

// main.c
void addBook(List** head, const char* booklist, int isRecursed);
void freeBooks(List *head);
List* readBooksFromFile(const char* booklist);

// menu.c
void displayOptions(int optionsCount, char *options[optionsCount], const char *prompt, int selectedItemIndex);
int Run(int optionsCount, int selectedItemIndex, char *options[optionsCount], const char *prompt);
void optionOne(int selectedItemIndex, List* head);
void optionTwo(int selectedItemIndex, List** head, const char* booklist);
void optionThree(int selectedItemIndex, List* head, const char* booklist);
void optionFour(int selectedItemIndex, List** head, const char* booklist);
void exitFunction(List* head);
void menuHome(int* selectedItemIndex, int optionsCount, char *options[optionsCount], const char *prompt, List** head, const char* booklist);

void writeBooksToFile(List* head, const char* booklist);
void deleteBook(List** head, const char* booklist);
List* searchBooks (List* head, const char* searchTerm);
#endif