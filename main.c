#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // this will be needed for date handling later
#include <ncurses.h> // For terminal control for UI
#include "otherFunctions.h"

int selectedItemIndexOriginal = 0; // Selected menu item in index form
const char *promptOriginal = "Library Management System: Welcome, Admin\nSelect and option using the arrow keys on the keyboard.\n"; // Prewritten instructions for user
char *optionsOriginal[6] = {"Search for Books", "Modify Book Data", "List Books", "Loan Book", "Save or Restore", "Exit"};
int optionsCountOriginal = sizeof(optionsOriginal) / sizeof(optionsOriginal[0]);

//Read From File Function
List* readBooksFromFile(const char* booklist) {
    FILE* file = fopen(booklist, "rb");
    if(file == NULL) {
        printf("Error reading file\n"); // Throw error if the file could not be opened
        return NULL;
    }
    
    int count; // The file will contain the number of books
    fread(&count, sizeof(int), 1, file); // Read number of books from file

    List* head = NULL; // Initialise head node
    List* current = NULL; // Initialise traversal node

    for(int i = 0; i < count; i++) {
        List* newNode = (List*)malloc(sizeof(List));
        if (newNode == NULL) {
            printf("Memory Allocation Failed!\n");
            fclose(file);
            return NULL;
        }

        newNode->book = (Book*)malloc(sizeof(Book)); // Allocate memory for book
        if (newNode->book == NULL) {
            printf("Memory Allocation for Book Failed!\n");
            free(newNode);
            fclose(file);
            return NULL;
        } 

        fread(newNode->book, sizeof(Book), 1, file); // Read the book from the file
        newNode->next = NULL;

        if(head == NULL) {
            head = newNode; // If the head is null, assign this book to be the head
            current = head; // Set the current position to the head 
        } else {
            current->next = newNode; // Set the next node to be the book
            current = current->next; // Traverse to the next book
        }
    }
    fclose(file); // Close the file
    printf("Books loaded successfully!\n");
    return head;
}

void writeBooksToFile(List* head, const char* booklist) {
    FILE* file = fopen(booklist, "wb"); // Open the file in write binary mode
    if (file == NULL) {
        printf("Error opening file for writing\n");
    }

    // Count the number of books in the list
   int count = 0;
    List* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    // Write the count value to the file
    fwrite(&count, sizeof(int), 1, file);

    // Write each books data to the file
    current = head;
    while (current != NULL) {
        fwrite(current->book, sizeof(Book), 1, file);
        current = current->next;
    }

    fclose(file);
    printf("Books Saved Successfully!\n");
}

// Free the memory of the books
void freeBooks(List *head) {
    List *current = head;
    List *next;
    while (current != NULL) {
        next = current->next;
        free(current->book);
        free(current);
        current = next;
    }
}


int main() {
    // =======BACKEND=======
    List* head = NULL; // Initialise the head of the list
    const char* booklist = "booklist.dat"; // File name of the booklist data file

    // Check if the file exists and is not empty
    FILE* file = fopen(booklist, "rb");
    if (file == NULL) {
        // If the file doesn't exist, create one with count set to 0
        file = fopen(booklist, "wb");
        if (file == NULL) {
            printf("Error creating file");
            return 1;
        }
        int count = 0;
        fwrite(&count, sizeof(int), 1, file);
        fclose(file);
    } else {
        fclose(file);
    }

    head = readBooksFromFile(booklist); // Parse the booklist file and set the head node

    //=======FRONTEND=======
    initscr(); // ncurses.h terminal
    cbreak();
    noecho();
    keypad(stdscr, TRUE); // Enable keypad
    curs_set(0);

    // Initialize colours
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support colours\n"); // IMPLIMENT SKIP LATER
        return 1;
    }
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);

    // Visual Stuff
    int selectedItemIndex = 0; // Selected menu item in index form
    const char *prompt = "Library Management System: Welcome, Admin\nSelect and option using the arrow keys on the keyboard.\n"; // Prewritten instructions for user
    char *options[6] = {"Search for Books", "Modify Book Data", "List Books", "Loan Book", "Save or Restore", "Exit"};
    int optionsCount = sizeof(options) / sizeof(options[0]);
    
    menuHome(&selectedItemIndex, optionsCount, options, prompt, &head, booklist);
}