#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "otherFunctions.h"

void displayOptions(int optionsCount, char *options[optionsCount], const char *prompt, int selectedItemIndex) {
    printw("%s", prompt);
    for (int i = 0; i < optionsCount; i++) {
        char *currentOption = options[i];
        char prefix;
        if (selectedItemIndex == i) {
            prefix = '*';
            attron(COLOR_PAIR((2)));
        } else {
            prefix = ' ';
            attron(COLOR_PAIR((1)));
        }
        printw("\n%c << %s >>", prefix, currentOption);
    }
    printf("\n");
    attroff(COLOR_PAIR(1));
    attroff(COLOR_PAIR(2));
}

int Run(int optionsCount, int selectedItemIndex, char *options[optionsCount], const char *prompt) {
    int ch = 0; // Data to store keypad stuff
    do {
        clear();
        switch (ch) {
            case KEY_UP:
                if (selectedItemIndex > 0) {
                    selectedItemIndex--;
                }
                break;
            case KEY_DOWN:
            if (selectedItemIndex < optionsCount - 1) {
                selectedItemIndex++;
            }
            break;
        }
        displayOptions(optionsCount, options, prompt, selectedItemIndex);
    } while ((ch = getch()) != '\n');
    return selectedItemIndex;
}

void goBackHome(List* head, const char* booklist) {
    int selectedItemIndex = 0; // Selected menu item in index form
    const char *prompt = "Library Management System: Welcome, Admin\nSelect and option using the arrow keys on the keyboard.\n"; // Prewritten instructions for user
    char *options[5] = {"Search for Books", "Modify Book Data", "List Books", "Save or Restore", "Exit"};
    int optionsCount = sizeof(options) / sizeof(options[0]);


    menuHome(selectedItemIndex, optionsCount, options, prompt, head, booklist);
}

void optionOne(int selectedItemIndex) {
    clear();
}

void optionTwo(int selectedItemIndex, List** head, const char* booklist) {
    clear();
    char *options[3] = {"Add Books", "Delete Books", "Return Home"};
    const char *prompt = "Home/Modify Book Data/\n";
    int optionsCount = sizeof(options) / sizeof(options[0]);
    selectedItemIndex = 0;
    selectedItemIndex = Run(optionsCount, selectedItemIndex, options, prompt);
    
    switch (selectedItemIndex) {
        case 0:
            addBook(head, booklist);
            break;
        case 1:
            deleteBook(head, booklist);
            break;
        case 2:
            clear();
            goBackHome(head, booklist);
            break;
    }
}

void optionThree(int selectedItemIndex, List* head, const char* booklist) {
    clear();
    // TODO: Make a better list function
    List *current = head;
    printw("Book list:\n\n");
    refresh();
    while (current != NULL) {
        printw("ID: %d, Title: %s, Author: %s\n", current->book->ID, current->book->Title, current->book->Author);
        refresh();
        current = current->next;
    }

    printw("Press any key to continue...");
    refresh();
    getch();
}

void optionFour(int selectedItemIndex, List** head, const char* booklist) {
    clear();
    char *options[2] = {"Apply Changes", "Restore Changes"};
    const char *prompt = "Home/Save or Restore\n";
    int optionsCount = sizeof(options) / sizeof(options[0]);
    selectedItemIndex = 0;
    selectedItemIndex = Run(optionsCount, selectedItemIndex, options, prompt);

    switch (selectedItemIndex) {
        case 0:
            writeBooksToFile(*head, booklist);
            break;
        case 1:
            *head = readBooksFromFile(booklist);
            break;
        }
}

void exitFunction(List* head) {
    freeBooks(head);
    endwin();
}

void menuHome(int* selectedItemIndex, int optionsCount, char* options[optionsCount], const char *prompt, List** head, const char* booklist) {
    *selectedItemIndex = Run(optionsCount, *selectedItemIndex, options, prompt);
    switch (*selectedItemIndex) {
        case 0:
            optionOne(*selectedItemIndex);
            menuHome(selectedItemIndex, optionsCount, options, prompt, head, booklist);
            break;
        case 1:
            optionTwo(*selectedItemIndex, head, booklist);
            menuHome(selectedItemIndex, optionsCount, options, prompt, head, booklist);
            break;
        case 2:
            optionThree(*selectedItemIndex, *head, booklist);
            menuHome(selectedItemIndex, optionsCount, options, prompt, head, booklist);
            break;
        case 3:
            optionFour(*selectedItemIndex, head, booklist);
            menuHome(selectedItemIndex, optionsCount, options, prompt, head, booklist);
            break;
        case 4:
            exitFunction(*head);
            break;
    }
    refresh();
}