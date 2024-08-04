#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include "otherFunctions.h"

void addBook(List** head, const char* booklist) {
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

    char addAnother;
    printw("\nAdd Another? (y/N): ");
    refresh();
    echo();
    curs_set(1);
    scanw("%c", &addAnother);
    noecho();
    curs_set(0);
    if (addAnother == 'Y' || addAnother == 'y') {
        addBook(head, booklist);
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

    printw("\nPress any key to continue...");
    refresh();
    getch();

    return;
}