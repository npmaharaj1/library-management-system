#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include "otherFunctions.h"

void addBook(List** head, const char* booklist, int isRecursed) {
    clear();
    List* newNode = (List*)malloc(sizeof(List)); // Initialise new book node

    if(newNode == NULL) {
        printw("\nMemory allocation failed!\n");
        refresh();
        return;
    }

    // Initialise book
    newNode->book = (Book*)malloc(sizeof(Book)); // Initialise book structure
    if(newNode->book == NULL) {
        printw("\nMemory allocation for book failed\n");
        refresh();
        free(newNode);
        return;
    }

    // Set unused date / borrower fields to -1. 
    newNode->book->issueDate.day = -1;
    newNode->book->issueDate.month = -1;
    newNode->book->issueDate.year = -1;
    newNode->book->dueDate.day= -1;
    newNode->book->dueDate.month= -1;
    newNode->book->dueDate.month= -1;
    newNode->book->borrowedTo.ID= -1;
    newNode->book->borrowedTo.email[0]= 0; 
    newNode->book->borrowedTo.name[0]= 0; 

    // Book id
    List* current = *head;
    if (current != NULL) {
        while(current->next != NULL) {
            current = current->next;
        }
        newNode->book->ID = current->book->ID + 1;
    } else {
        newNode->book->ID = 1;
    }

    // Book Title
    printw("\nEnter Book Title: ");
    // fgets(newNode->book->Title, MAXTITLELENGTH, stdin); // Accept Book Title Input
    refresh();
    echo();
    curs_set(1);
    wgetnstr(stdscr, newNode->book->Title, sizeof(newNode->book->Title) - 1); // Accept Book Title Input
    noecho();
    curs_set(0);
    // newNode->book->Title[strcspn(newNode->book->Title, "\n")] = 0; // Remove newline from end of title

    if (strlen(newNode->book->Title) == 0) {
        printw("\nBook Title cannot be empty!\n");
        refresh();
        free(newNode->book);
        free(newNode);
        return;
    }

    // Book Author
    printw("Enter Author Name: ");
    // fgets(newNode->book->Author, MAXAUTHORLENGTH, stdin); // Accept Author Input
    refresh();
    echo();
    curs_set(1);
    wgetnstr(stdscr, newNode->book->Author, sizeof(newNode->book->Author) - 1);
    noecho();
    curs_set(0);
    // newNode->book->Author[strcspn(newNode->book->Author, "\n")] = 0; // Remove newline from end of Author

    if (strlen(newNode->book->Author) == 0) {
        printw("\nAuthor Name cannot be empty!\n");
        refresh();
        free(newNode->book);
        free(newNode);
        return;
    }

    // Append book to end of list
    newNode->next = NULL;
    if (*head == NULL) {
        *head = newNode;
    } else {
        current->next = newNode;
    }
    
    printw("Book added successfully!\n");
    refresh();

    char addAnother; // Store yes/no
    printw("\nAdd Another? (y/N): ");
    refresh();
    echo();
    curs_set(1);
    scanw("%c", &addAnother);
    noecho();
    curs_set(0);
    if (addAnother == 'Y' || addAnother == 'y') {
        addBook(head, booklist, 1);
    }

    if (isRecursed == 0) {
        char applyChanges; // Store yes/no
        printw("\nApply Changes? (y/N): ");
        refresh();
        echo();
        curs_set(1);
        scanw("%c", &applyChanges);
        noecho();
        curs_set(0);
        if (applyChanges == 'Y' || applyChanges == 'y') {
            writeBooksToFile(*head, booklist);
        }
    }

    return;
}

void deleteBook(List** head, const char* booklist) {

    if (head == NULL || *head == NULL) {
        printw("No nodes to search\n");
        refresh();
        return;
    }
    
    // Temporary Search -- To be replaced by the search function
    List* current = *head;
    List* previous = NULL;
    if (current == NULL) {
        printw("No nodes to search");
        refresh();
        return;
    }

    // Searchfor book to be deleted
    char searchedBook[MAXTITLELENGTH];
    printw("\nEnter the book title to find: ");
    refresh();
    echo();
    curs_set(1);
    scanw("%99s", searchedBook);
    noecho();
    curs_set(0);
    searchedBook[strcspn(searchedBook, "\n")] = 0; // Remove Newline

    // List all books with matching title
    List* deleteHead = NULL; // New list for books with matching title
    List* deleteCurrent = deleteHead; // New traversal node

    while (current != NULL) {
        if (strcmp(current->book->Title, searchedBook) == 0){
            List* newNode = (List *)malloc(sizeof(List));
            if (newNode == NULL) {
                printw("Memory Allocation Error");
                refresh();
                return;
            }

            newNode->book = current->book;
            newNode->next = NULL;

            if (deleteHead == NULL) { // If the list is empty
                deleteHead = newNode; // Set head to the match
                deleteCurrent = deleteHead; // Traverse delete list
            } else {
                deleteCurrent->next = newNode; // Otherwise set the next node in the list
                deleteCurrent = deleteCurrent->next; // Traverse delete list
            }
        } 
        previous = current;
        current = current->next;
    }

    // Print the book/s ID and Title
    deleteCurrent = deleteHead; // Reset delete traversal node
    if (deleteHead == NULL) {
        printw("Book not found!\n");
        refresh();
        return;
    } else {
        // Print all of the books found
        while (deleteCurrent != NULL) {
            printw("%d, %s\n", deleteCurrent->book->ID, deleteCurrent->book->Title);
            refresh();
            deleteCurrent = deleteCurrent->next;
        }
    }


    // Confirm which book is to be deleted
    int deleteBookID;
    printw("Enter Book ID to be Deleted: ");
    refresh();
    echo();
    curs_set(1);
    scanw("%d", &deleteBookID);
    noecho();
    curs_set(0);


    // Iterate through Book again to find the ID
    current = *head;
    previous = NULL;

    while (current != NULL && current->book->ID != deleteBookID) {
        previous = current;
        current = current->next;
    }

    // Check if book ID is valid, print deleting message
    if (current != NULL && strcmp(current->book->Title, searchedBook) == 0) {
        printw("Deleting book %d: %s by %s ...\n", current->book->ID, current->book->Title, current->book->Author);
        refresh();
    } else {
        printw("Invalid Input!\n");
        refresh();
        return;
    }

    // Delete the book from the List
    if (previous == NULL) {
        *head = current->next; // If the first node was deleted, new head
    } else {
        previous->next = current->next; // Otherwise set the previous's next to the next node
    }
    free(current->book);
    free(current);

    // Free the delete list
    deleteCurrent = deleteHead;
    while (deleteCurrent != NULL) {
        List* temp = deleteCurrent;
        deleteCurrent = deleteCurrent->next;
        free(temp);
    }

    char applyChanges;
    printw("\nApply Changes? (Y/n): ");
    refresh();
    echo();
    curs_set(1);
    scanw("%c", &applyChanges);
    noecho();
    curs_set(0);
    if (applyChanges != 'N' && applyChanges != 'n') {
        writeBooksToFile(*head, booklist);
    }
    
    printw("\nPress any key to continue...");
    refresh();
    getch();

    return;
}

// Search by title / author
List* searchBooks(List* head, const char* searchTerm) { 
    List* rHead = NULL; // Initialising head of the result list
    List* rTail = NULL;
    List* current = head;

    // Loops through booklist
    while(current != NULL) { 
        // If substring match found in title and author (not case sensitive) 
        if (strcasestr(current->book->Title, searchTerm) || strcasestr(current->book->Author, searchTerm)) {
            // Assign values to temporary match 
            List* match = (List*)malloc(sizeof(List));
            match->book = (Book*)malloc(sizeof(Book));
            memcpy(match->book->Title, current->book->Title, sizeof(current->book->Title));
            memcpy(match->book->Author, current->book->Author, sizeof(current->book->Author));
            match->book->ID = current->book->ID;
            match->next = NULL;

            // Updating Head + Tail if applicable
            if (rHead == NULL) { 
                rHead = match; 
                rTail = match;
            } else {
                rTail->next = match;
                rTail = rTail->next;
            }
        }
        current = current->next;
    }
    return rHead; // Returns head for the linked list of matches
}

void getFutureDate (Date* futureDate, int daysIntoFuture) {
    // Get the current time
    time_t t = time(NULL);
    struct tm *currentTime = localtime(&t);
    
    //Add specified number of days to the current date
    currentTime->tm_mday += daysIntoFuture;

    // Normalise the time structure (month/year overflow)
    mktime(currentTime);

    // Populate the futureDate struct with the new date
    futureDate->day = currentTime->tm_mday;
    futureDate->month = currentTime->tm_mon + 1; // tm_mon is 0 indexed
    futureDate->year = currentTime->tm_year + 1900; // tm_year is years since 1900
}

// Assigns borrower and due date values to book.
void loanBook(List* head, const char* input) {

    // Check if input is empty
    if (input == NULL || strlen(input) == 0) {
        printw("\nBook name cannot be empty!\n");
        refresh();
        return;
    }

    List* current = head;

    // Loops through booklist to find first title match for input
    while(current != NULL) { 
        // If substring match found in title (not case sensitive) 
        if (strcasestr(current->book->Title, input)) {

            // Input borrower name
            printw("\nEnter Name of the Borrower: ");
            refresh();
            echo();
            curs_set(1);
            wgetnstr(stdscr, current->book->borrowedTo.name, sizeof(current->book->borrowedTo.name) - 1); // Accept Book Title input
            noecho();
            curs_set(0);

            printw("\nPlease enter your perferred due time format.");
            echo();
            printw("\nEnter 1 for due by a specified date (dd/mm/yyyy).");
            printw("\nEnter 2 for due by loan length (no. of days).\n");
            refresh();
            curs_set(1);
            int choice = 0; 
            scanw("%d", &choice);  

            // Due date by specified date.
            if (choice == 1) { 
                noecho();
                curs_set(0);
                // Input due date
                echo();
                printw("\nEnter Due Date (dd/mm/yyyy): ");
                refresh();
                curs_set(1);   

                // Input checking
                if (scanw("%d/%d/%d", &current->book->dueDate.day, &current->book->dueDate.month, &current->book->dueDate.year)!=3) { // Accept Due Date input
                    printw("\nInvalid Date!\n");
                    refresh();
                    return;
                }
                if (make_time(current->book->dueDate.day, current->book->dueDate.month, current->book->dueDate.year) == -1) {
                    printw("\nInvalid Date!\n");
                    refresh();
                    return;
                }
                if (time(NULL)>make_time(current->book->dueDate.day, current->book->dueDate.month, current->book->dueDate.year)) {
                    printw("\nInvalid Date: Due date has already occured!\n");
                    refresh();
                    return;
                }

                // Return if successful
                noecho();
                curs_set(0);
                printw("\nSuccessfully Updated!\n");
                refresh();
                return;

            // Due date by specified no. of days.
            } else if (choice == 2) {
                noecho();
                curs_set(0);
                
                time_t secsInDay = 86400;
                time_t now = time(NULL);

                echo();
                printw("\nEnter loan length (no. of days) from today: ");
                refresh();
                curs_set(1); 

                int loanLength;

                // Input checking
                if (scanw("%d", &loanLength)!=1) { // Accept Due Date input
                    printw("\nInvalid Input!\n");
                    refresh();
                    return;
                }
                if (loanLength <= 0) { 
                    printw("\nInvalid Input!\n");
                    refresh();
                    return;
                }

                // Assigns date values to book.
                getFutureDate(&current->book->dueDate, loanLength);

                // Return if successful
                noecho();
                curs_set(0);
                printw("\nSuccessfully Updated!\n");
                refresh();
                return;


            // Invalid option.
            } else {
                noecho();
                curs_set(0);

                printw("\nInvalid Option!\n");
                refresh();
                return;
            }

        }
        current = current->next;
    }
    
    // If book not found in while loop:
    printw("\nBook not found!\n");
    refresh();
    return;
}

// Function to convert date format to time_t (seconds since 1900)
time_t make_time(int day, int month, int year) {
    struct tm date = {0};
    date.tm_mday = day;
    date.tm_mon = month - 1;
    date.tm_year = year - 1900;

    time_t result = mktime(&date);
    return result;
}